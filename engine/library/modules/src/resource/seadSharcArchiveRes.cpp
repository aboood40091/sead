#include <container/seadBuffer.h>
#include <prim/seadSafeString.h>
#include <resource/seadSharcArchiveRes.h>

#include <cstring>

namespace
{

u32 calcHash32(const sead::SafeString& str, u32 key)
{
    const char* buffer = str.cstr();

    u32 ret = 0;
    for (s32 i = 0; buffer[i] != '\0'; i++)
        ret = ret * key + buffer[i];

    return ret;
}

s32 binarySearch_(
    u32 hash, const sead::SharcArchiveRes::FATEntry* buffer,
    s32 start, s32 end
)
{
    s32 middle;

    for (;;)
    {
        middle = (start + end) / 2;

        u32 buf_hash = buffer[middle].hash;
        if (buf_hash == hash)
            return middle;

        else if (buf_hash < hash)
        {
            if (start == middle)
                return -1;

            start = middle;
        }
        else
        {
            if (end == middle)
                return -1;

            end = middle;
        }
    }
}

} // namespace {no-name}

namespace sead {

SharcArchiveRes::SharcArchiveRes()
    : ArchiveRes()
    , mArchiveBlockHeader(nullptr)
    , mFATBlockHeader(nullptr)
    , mFNTBlock(nullptr)
    , mDataBlock(nullptr)
#ifdef cafe
    , mEndianType(Endian::cBig)
#else
    #error "Unknown platform"
#endif // cafe
{
}

SharcArchiveRes::~SharcArchiveRes()
{
}

void*
SharcArchiveRes::getFileImpl_(
    const SafeString& file_path,
    FileInfo* file_info
)
{
    s32 id = convertPathToEntryIDImpl_(file_path);
    if (id < 0)
        return nullptr;

    return getFileFastImpl_(id, file_info);
}

void*
SharcArchiveRes::getFileFastImpl_(
    s32 entry_id, FileInfo* file_info
)
{
    if (entry_id < 0 || entry_id >= mFATEntrys.size())
        return nullptr;

    u32 start_offset = mFATEntrys.unsafeGet(entry_id)->data_start_offset;

    if (file_info != nullptr)
    {
        u32 end_offset = mFATEntrys.unsafeGet(entry_id)->data_end_offset;
        if (start_offset > end_offset)
            return nullptr;

        u32 length = end_offset - start_offset;

        setFileInfo(file_info, start_offset, length);
    }

    return PtrUtil::addOffset(mDataBlock, start_offset);
}

s32
SharcArchiveRes::convertPathToEntryIDImpl_(
    const SafeString& file_path
)
{
    u32 hash = calcHash32(file_path, mFATBlockHeader->hash_key);

    s32 start = 0;
    s32 end = mFATEntrys.size();

    s32 id = binarySearch_(hash, mFATEntrys.getBufferPtr(), start, end);
    if (id == -1)
        return -1;

    u32 offset = mFATEntrys.unsafeGet(id)->name_offset;
    if (offset != 0)
    {
        id -= (offset >> 24) - 1;

        while (id < end)
        {
            const FATEntry* entry = mFATEntrys.unsafeGet(id);
            if (entry->hash != hash)
                return -1;

            else
            {
                u32 name_offset = entry->name_offset;

                if (PtrUtil::addOffset(mFNTBlock, (name_offset & 0xffffff)) > mDataBlock)
                {
                  //SEAD_WARNING(false, "Invalid data start offset");
                    return -1;
                }

                if (file_path.isEqual(mFNTBlock + (name_offset & 0xffffff) * cFileNameTableAlign))
                    return id;
            }

            id++;
        }
    }

    return id;
}

bool
SharcArchiveRes::setCurrentDirectoryImpl_(const SafeString&)
{
  //SEAD_ASSERT_MSG(false, "Not support.");
    return false;
}

bool
SharcArchiveRes::openDirectoryImpl_(
    HandleBuffer* handle_buffer, const SafeString&
)
{
    u32* handle = reinterpret_cast<u32*>(handle_buffer);
    *handle = 0;
    return true;
}

bool
SharcArchiveRes::closeDirectoryImpl_(HandleBuffer* handle_buffer)
{
    return true;
}

u32
SharcArchiveRes::readDirectoryImpl_(
    HandleBuffer* handle_buffer, DirectoryEntry* entry, u32 num
)
{
    u32* handle = reinterpret_cast<u32*>(handle_buffer);

    u32 count = 0;

    while (*handle + count < mFATBlockHeader->file_num && count < num)
    {
        u32 id = *handle + count;
      //SEAD_ASSERT(id >= (*handle));

        u32 offset = mFATEntrys.unsafeGet(id)->name_offset;
        if (offset == 0)
            entry[count].name.format("%08x", mFATEntrys.unsafeGet(id)->hash);

        else
        {
            if (PtrUtil::addOffset(mFNTBlock, (offset & 0xffffff)) > mDataBlock)
            {
              //SEAD_WARNING(false, "Invalid data start offset");
                entry[count].name.clear();
            }
            else
            {
                entry[count].name.copy(mFNTBlock + (offset & 0xffffff) * cFileNameTableAlign);
            }
        }

        entry[count].is_directory = false;
        count++;
    }

    *handle += count;
    return count;
}

bool
SharcArchiveRes::prepareArchive_(const void* archive)
{
    if (archive == nullptr)
    {
      //SEAD_ASSERT_MSG(false, "archive must not be nullptr.");
        return false;
    }

    const u8* archive8 = reinterpret_cast<const u8*>(archive);

    mArchiveBlockHeader = reinterpret_cast<const ArchiveBlockHeader*>(archive8);
    if (MemUtil::compare(mArchiveBlockHeader->signature, "SARC", 4) != 0)
    {
      //SEAD_ASSERT_MSG(false, "Invalid ArchiveBlockHeader");
        return false;
    }

    mEndianType = Endian::markToEndian(mArchiveBlockHeader->byte_order);
    if (mEndianType != Endian::getHostEndian())
    {
      //SEAD_ASSERT_MSG(false, "Invalid Endian Type");
        return false;
    }

    if (mArchiveBlockHeader->version != cArchiveVersion)
    {
      //SEAD_ASSERT_MSG(false, "unmatching version ( expect: %x, actual: %x )", cArchiveVersion, mArchiveBlockHeader->version);
        return false;
    }

    if (mArchiveBlockHeader->header_size != sizeof(ArchiveBlockHeader))
    {
      //SEAD_ASSERT_MSG(false, "Invalid ArchiveBlockHeader");
        return false;
    }

    mFATBlockHeader = reinterpret_cast<const FATBlockHeader*>(archive8 + mArchiveBlockHeader->header_size);
    if (MemUtil::compare(mFATBlockHeader->signature, "SFAT", 4) != 0)
    {
      //SEAD_ASSERT_MSG(false, "Invalid FATBlockHeader");
        return false;
    }

    if (mFATBlockHeader->header_size != sizeof(FATBlockHeader))
    {
      //SEAD_ASSERT_MSG(false, "Invalid FATBlockHeader");
        return false;
    }

    if (mFATBlockHeader->file_num > cArchiveEntryMax)
    {
      //SEAD_ASSERT_MSG(false, "Invalid FATBlockHeader");
        return false;
    }

    mFATEntrys.setBuffer(
        mFATBlockHeader->file_num,
        const_cast<FATEntry*>(reinterpret_cast<const FATEntry*>(archive8 + mArchiveBlockHeader->header_size + mFATBlockHeader->header_size))
    );

    const FNTBlockHeader* fnt_header = reinterpret_cast<const FNTBlockHeader*>(
        archive8 + mArchiveBlockHeader->header_size +
        mFATBlockHeader->header_size + mFATBlockHeader->file_num * sizeof(FATEntry)
    );
    if (MemUtil::compare(fnt_header->signature, "SFNT", 4) != 0)
    {
      //SEAD_ASSERT_MSG(false, "Invalid FNTBlockHeader");
        return false;
    }

    if (fnt_header->header_size != sizeof(FNTBlockHeader))
    {
      //SEAD_ASSERT_MSG(false, "Invalid FNTBlockHeader");
        return false;
    }

    mFNTBlock = reinterpret_cast<const char*>(fnt_header) + fnt_header->header_size;
    if (mArchiveBlockHeader->data_block_offset < PtrUtil::diff(mFNTBlock, mArchiveBlockHeader))
    {
      //SEAD_ASSERT_MSG(false, "Invalid data block offset");
        return false;
    }

    mDataBlock = archive8 + mArchiveBlockHeader->data_block_offset;
    return true;
}

} // namespace sead
