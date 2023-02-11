#ifndef SEAD_ARCHIVE_RES_H_
#define SEAD_ARCHIVE_RES_H_

#include <filedevice/seadFileDevice.h>
#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>
#include <resource/seadResource.h>

namespace sead {

class ArchiveRes : public DirectResource
{
    SEAD_RTTI_OVERRIDE(ArchiveRes, DirectResource)

public:
    class FileInfo
    {
    public:
        explicit FileInfo()
            : mStartOffset(0)
            , mLength(0)
        {
        }

        u32 getStartOffset() const { return mStartOffset; }
        u32 getLength() const { return mLength; }

    private:
        u32 mStartOffset;
        u32 mLength;

        friend class ArchiveRes;
    };
#ifdef cafe
    static_assert(sizeof(FileInfo) == 8, "sead::ArchiveRes::FileInfo size mismatch");
#endif // cafe

public:
    ArchiveRes()
        : DirectResource()
        , mEnable(false)
    {
    }

    virtual ~ArchiveRes() { }

    virtual u32 getLoadDataAlignment() { return 0x80; }

protected:
    virtual void doCreate_(u8* buf, u32, Heap*);
    virtual void* getFileImpl_(const SafeString& file_path, FileInfo* file_info) = 0;
    virtual void* getFileFastImpl_(s32 entry_id, FileInfo* file_info) = 0;
    virtual s32 convertPathToEntryIDImpl_(const SafeString& file_path) = 0;
    virtual bool setCurrentDirectoryImpl_(const SafeString& file_path) = 0;
    virtual bool openDirectoryImpl_(HandleBuffer* handle_buffer, const SafeString& dir_path) = 0;
    virtual bool closeDirectoryImpl_(HandleBuffer* handle_buffer) = 0;
    virtual u32 readDirectoryImpl_(HandleBuffer* handle_buffer, DirectoryEntry* entry, u32 num) = 0;
    virtual bool prepareArchive_(const void* archive) = 0;

public:
    Resource* load(ResourceMgr::LoadArg& arg);

    u8* getFile(const SafeString& file_path, FileInfo* file_info = nullptr)
    {
        // SEAD_ASSERT(mEnable);
        return static_cast<u8*>(getFileImpl_(file_path, file_info));
    }

    u8* getFileFast(s32 entry_id, FileInfo* file_info = nullptr)
    {
        // SEAD_ASSERT(mEnable);
        return static_cast<u8*>(getFileFastImpl_(entry_id, file_info));
    }

    s32 convertPathToEntryID(const SafeString& file_path)
    {
        // SEAD_ASSERT(mEnable);
        return convertPathToEntryIDImpl_(file_path);
    }

    bool setCurrentDirectory(const SafeString& file_path)
    {
        // SEAD_ASSERT(mEnable);
        return setCurrentDirectoryImpl_(file_path);
    }

    bool openDirectory(HandleBuffer* handle_buffer, const SafeString& dir_path)
    {
        // SEAD_ASSERT(handle_buffer);
        return openDirectoryImpl_(handle_buffer, dir_path);
    }

    bool closeDirectory(HandleBuffer* handle_buffer)
    {
        // SEAD_ASSERT(handle_buffer);
        return closeDirectoryImpl_(handle_buffer);
    }

    u32 readDirectory(HandleBuffer* handle_buffer, DirectoryEntry* entry, u32 num)
    {
        // SEAD_ASSERT(handle_buffer);
        // SEAD_ASSERT(entry);
        return readDirectoryImpl_(handle_buffer, entry, num);
    }

protected:
    void setFileInfo(FileInfo* file_info, u32 start_offset, u32 length)
    {
        //SEAD_ASSERT(file_info);
        file_info->mStartOffset = start_offset;
        file_info->mLength      = length;
    }

protected:
    bool mEnable;
};
#ifdef cafe
static_assert(sizeof(ArchiveRes) == 0x28, "sead::ArchiveRes size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_ARCHIVE_RES_H_
