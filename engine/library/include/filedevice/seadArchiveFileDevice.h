#ifndef SEAD_ARCHIVE_FILEDEVICE_H_
#define SEAD_ARCHIVE_FILEDEVICE_H_

#include <filedevice/seadFileDevice.h>
#include <resource/seadArchiveRes.h>

namespace sead {

class ArchiveFileDevice : public FileDevice
{
public:
    ArchiveFileDevice(ArchiveRes* archive = nullptr)
        : FileDevice("arc")
        , mArchive(archive)
    {
    }

    virtual ~ArchiveFileDevice()
    {
    }

    void setArchive(ArchiveRes* archive)
    {
        mArchive = archive;
    }

    ArchiveRes* getArchive() const
    {
        return mArchive;
    }

    FileDevice* tryOpenWithEntryID(FileHandle* handle, s32 entry_id, FileOpenFlag flag, u32 div_size);
    u8* tryLoadWithEntryID(s32 entry_id, LoadArg& arg);
    s32 tryConvertPathToEntryID(const SafeString& filename);
    bool setCurrentDirectory(const SafeString& path);

protected:
    virtual bool doIsAvailable_() const { return true; }
    virtual u8* doLoad_(LoadArg& arg);
    virtual u8* doLoadWithEntryID_(s32 entry_id, LoadArg& arg);
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag);
    virtual FileDevice* doOpenWithEntryID_(FileHandle* handle, s32 entry_id, FileOpenFlag flag);
    virtual s32 doConvertPathToEntryID_(const SafeString& filename);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size);
    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size) { return false; }
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* size, const SafeString& path);
    virtual bool doGetFileSize_(u32* size, FileHandle* handle);
    virtual bool doIsExistFile_(bool* is_exist, const SafeString& path);
    virtual bool doIsExistDirectory_(bool* is_exist, const SafeString& path);
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname);
    virtual bool doCloseDirectory_(DirectoryHandle* handle);
    virtual bool doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num);
    virtual bool doMakeDirectory_(const SafeString& path, u32 permission);
    virtual RawErrorCode doGetLastRawError_() const;
    virtual bool doSetCurrentDirectory_(const SafeString& path);

protected:
    struct ArchiveFileHandle
    {
        const u8* image;
        ArchiveRes::FileInfo file_info;
        u32 cur_pos;
    };

protected:
    ArchiveFileHandle* getArchiveFileHandle_(FileHandle* handle);

protected:
    ArchiveRes* mArchive;
};
#ifdef cafe
static_assert(sizeof(ArchiveFileDevice) == 0x58, "sead::ArchiveFileDevice size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_ARCHIVE_FILEDEVICE_H_
