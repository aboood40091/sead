#include <filedevice/seadArchiveFileDevice.h>
#include <math/seadMathCalcCommon.h>

namespace sead {

u8* ArchiveFileDevice::tryLoadWithEntryID(s32 entry_id, LoadArg& arg)
{
    // SEAD_ASSERT_MSG(hasPermission(), "Device permission error.");
    if (!hasPermission())
        return nullptr;

    return doLoadWithEntryID_(entry_id, arg);
}

FileDevice* ArchiveFileDevice::tryOpenWithEntryID(FileHandle* handle, s32 entry_id, FileOpenFlag flag, u32 div_size)
{
    // SEAD_ASSERT_MSG(hasPermission(), "Device permission error.");
    if (!hasPermission())
        return nullptr;

    setFileHandleDivSize_(handle, div_size);
    FileDevice* device = doOpenWithEntryID_(handle, entry_id, flag);
    setHandleBaseFileDevice_(handle, device);
    return device;
}

s32 ArchiveFileDevice::tryConvertPathToEntryID(const SafeString& filename)
{
    return doConvertPathToEntryID_(filename);
}

bool ArchiveFileDevice::setCurrentDirectory(const SafeString& path)
{
    // SEAD_ASSERT_MSG(hasPermission(), "Device permission error.");
    if (!hasPermission())
        return false;

    return doSetCurrentDirectory_(path);
}

bool ArchiveFileDevice::doGetFileSize_(u32* size, const SafeString& path)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    if (path.cstr() == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid path");
        return false;
    }

    ArchiveRes::FileInfo finfo;
    u8* ret = mArchive->getFile(path, &finfo);
    if (ret == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "Can\'t open %s", path.cstr());
        return false;
    }

    *size = finfo.getLength();
    return true;
}

bool ArchiveFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
{
    if (handle == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid handle");
        return false;
    }

    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    ArchiveFileHandle* arc_handle = getArchiveFileHandle_(handle);
    *size = arc_handle->file_info.getLength();
    return true;
}

bool ArchiveFileDevice::doIsExistFile_(bool* is_exist, const SafeString& path)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    if (path.cstr() == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid path");
        return false;
    }

    u8* ret = mArchive->getFile(path);
    *is_exist = ret != nullptr;
    return true;
}

bool ArchiveFileDevice::doIsExistDirectory_(bool* is_exist, const SafeString& path)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    if (path.cstr() == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid path");
        return false;
    }

    *is_exist = false; // LOL
    return true;
}

ArchiveFileDevice::ArchiveFileHandle* ArchiveFileDevice::getArchiveFileHandle_(FileHandle* handle)
{
    return reinterpret_cast<ArchiveFileHandle*>(&(getHandleBaseHandleBuffer_(handle)[0]));
}

u8* ArchiveFileDevice::doLoadWithEntryID_(s32 entry_id, LoadArg& arg)
{
    if (entry_id == -1)
    {
        // SEAD_ASSERT_MSG(false, "Invalid entry_id");
        return nullptr;
    }

    if (arg.buffer != nullptr || arg.heap != nullptr)
    {
        FileHandle handle;
        FileDevice* dev = nullptr;

        dev = tryOpenWithEntryID(&handle, entry_id, cFileOpenFlag_ReadOnly, arg.div_size);
        if (dev == nullptr)
            return nullptr;

        u32 buffer_size = arg.buffer_size;
        if (buffer_size == 0)
        {
            u32 file_size = 0;
            bool success = tryGetFileSize(&file_size, &handle);
            if (!success)
                return nullptr;

            // SEAD_ASSERT(file_size != 0);

            u32 roundup_size = Mathi::roundUpPow2(file_size, 32);
            buffer_size = roundup_size;
        }

        u8* buffer = arg.buffer;
        bool need_unload = false;
        if (buffer == nullptr)
        {
            s32 alignment = Mathi::sign(arg.alignment) * Mathi::max(Mathi::abs(arg.alignment), 32);
            buffer = /* NewArray<u8>(buffer_size, arg.heap, alignment) */ new (arg.heap, alignment) u8[buffer_size];
            need_unload = true;
        }

        u32 read_size = 0;
        bool success = tryRead(&read_size, &handle, buffer, buffer_size);
        if (!success)
        {
            if (need_unload)
                /* DeleteArray<u8>(buffer) */ delete[] buffer;
            return nullptr;
        }
        success = tryClose(&handle);
        if (!success)
        {
            if (need_unload)
                /* DeleteArray<u8>(buffer) */ delete[] buffer;
            return nullptr;
        }

        arg.read_size = read_size;
        arg.roundup_size = buffer_size;
        arg.need_unload = need_unload;

        return buffer;
    }
    else
    {
        ArchiveRes::FileInfo info;
        u8* ret = mArchive->getFileFast(entry_id, &info);
        if (ret == nullptr)
            return nullptr;

        // SEAD_ASSERT(arg.alignment == 0 || PtrUtil::isAligned( ret, Mathi::abs( arg.alignment )));

        arg.read_size = info.getLength();
        arg.roundup_size = info.getLength();
        arg.need_unload = false;

        return ret;
    }
}

u8* ArchiveFileDevice::doLoad_(LoadArg& arg)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return nullptr;
    }

    if (arg.buffer != nullptr || arg.heap != nullptr)
        return FileDevice::doLoad_(arg);

    else
    {
        ArchiveRes::FileInfo info;
        u8* ret = mArchive->getFile(arg.path, &info);
        if (ret == nullptr)
            return nullptr;

        // SEAD_ASSERT(arg.alignment == 0 || PtrUtil::isAligned( ret, Mathi::abs( arg.alignment )));

        arg.read_size = info.getLength();
        arg.roundup_size = info.getLength();
        arg.need_unload = false;

        return ret;
    }
}

FileDevice* ArchiveFileDevice::doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag)
{
    if (handle == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid handle");
        return nullptr;
    }

    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return nullptr;
    }

    if (filename.cstr() == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid filename");
        return nullptr;
    }

    ArchiveFileHandle* arc_handle = getArchiveFileHandle_(handle);
    u8* ret = mArchive->getFile(filename, &arc_handle->file_info);
    if (ret == nullptr)
        return nullptr;

    arc_handle->image = ret;
    arc_handle->cur_pos = 0;

    return this;
}

FileDevice* ArchiveFileDevice::doOpenWithEntryID_(FileHandle* handle, s32 entry_id, FileOpenFlag flag)
{
    if (handle == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid handle");
        return nullptr;
    }

    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return nullptr;
    }

    ArchiveFileHandle* arc_handle = getArchiveFileHandle_(handle);
    u8* ret = mArchive->getFileFast(entry_id, &arc_handle->file_info);
    if (ret == nullptr)
        return nullptr;

    arc_handle->image = ret;
    arc_handle->cur_pos = 0;

    return this;
}

s32 ArchiveFileDevice::doConvertPathToEntryID_(const SafeString& filename)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return 0;
    }

    return mArchive->convertPathToEntryID(filename);
}

bool ArchiveFileDevice::doClose_(FileHandle* handle)
{
    return true;
}

bool ArchiveFileDevice::doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size)
{
    ArchiveFileHandle* arc_handle = getArchiveFileHandle_(handle);
    if (arc_handle->file_info.getLength() < arc_handle->cur_pos + size)
        size = arc_handle->file_info.getLength() - arc_handle->cur_pos;

    sead::MemUtil::copy(buf, arc_handle->image + arc_handle->cur_pos, size);
    arc_handle->cur_pos += size;

    if (read_size != nullptr)
        *read_size = size;

    return true;
}

bool ArchiveFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    ArchiveFileHandle* arc_handle = getArchiveFileHandle_(handle);
    s32 pos = arc_handle->cur_pos;

    switch (origin)
    {
    case cSeekOrigin_Begin:
        pos = offset;
        break;
    case cSeekOrigin_Current:
        pos += offset;
        break;
    case cSeekOrigin_End:
        pos = arc_handle->file_info.getLength() + offset;
        break;
    default:
        // SEAD_ASSERT_MSG(false, "Unexpected origin.");
        return false;
    }

    if (pos > arc_handle->file_info.getLength())
        return false;

    arc_handle->cur_pos = pos;
    return true;
}

bool ArchiveFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    if (handle == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid handle");
        return false;
    }

    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    *pos = getArchiveFileHandle_(handle)->cur_pos;
    return true;
}

FileDevice* ArchiveFileDevice::doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname)
{
    if (handle == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid handle");
        return nullptr;
    }

    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return nullptr;
    }

    FileDevice* ret = mArchive->openDirectory(&getHandleBaseHandleBuffer_(handle), dirname) ? this : nullptr;
    return ret;
}

bool ArchiveFileDevice::doCloseDirectory_(DirectoryHandle* handle)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    return mArchive->closeDirectory(&getHandleBaseHandleBuffer_(handle));
}

bool ArchiveFileDevice::doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    u32 tmp_read_num = mArchive->readDirectory(&getHandleBaseHandleBuffer_(handle), entry, num);
    if (read_num != nullptr)
        *read_num = tmp_read_num;

    return true;
}

bool ArchiveFileDevice::doSetCurrentDirectory_(const SafeString& path)
{
    if (mArchive == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "no archive mounted");
        return false;
    }

    if (path.cstr() == nullptr)
    {
        // SEAD_ASSERT_MSG(false, "invalid filename");
        return false;
    }

    return mArchive->setCurrentDirectory(path);
}

bool ArchiveFileDevice::doMakeDirectory_(const SafeString& path, u32 permission)
{
    return false;
}

RawErrorCode ArchiveFileDevice::doGetLastRawError_() const
{
    // SEAD_ASSERT_MSG(false, "not impremented");
    return 0;
}

} // namespace sead
