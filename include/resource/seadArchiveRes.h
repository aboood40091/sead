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

    virtual void doPostCreate_();
    virtual u32 getLoadDataAlignment() { return 0x80; }
    virtual void doCreate_(u8* buf, u32, Heap*);
    virtual void* getFileImpl_(const SafeString& file_path, FileInfo* file_info = NULL) = 0;
    virtual void* getFileFastImpl_(s32 entry_id, FileInfo* file_info) = 0;
    virtual s32 convertPathToEntryIDImpl_(const SafeString& file_path) = 0;
    virtual bool setCurrentDirectoryImpl_(const SafeString&) = 0;
    virtual bool openDirectoryImpl_(u32* handle, const SafeString&) = 0;                    // openDirectoryImpl_(SafeArray<u8, 32>*, const SafeString&)
    virtual bool closeDirectoryImpl_(u32* handle) = 0;                                      // closeDirectoryImpl_(SafeArray<u8, 32>*)
    virtual u32 readDirectoryImpl_(u32* handle, DirectoryEntry* entry, u32 num) = 0;        // readDirectoryImpl_(SafeArray<u8, 32>*, DirectoryEntry*, u32)
    virtual bool prepareArchive_(const void* archive) = 0;

    Resource* load(ResourceMgr::LoadArg& arg);

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
