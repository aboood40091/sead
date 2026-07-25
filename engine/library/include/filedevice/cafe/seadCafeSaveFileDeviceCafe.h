#ifndef SEAD_CAFE_FSA_FILEDEVICE_H_
#define SEAD_CAFE_FSA_FILEDEVICE_H_

#include <filedevice/seadFileDevice.h>

#include <cafe/fs.h>
#include <nn/save.h>

namespace sead {

class CafeSaveFileDevice : public FileDevice
{
public:
    CafeSaveFileDevice(u8 account_slot_no);
    virtual ~CafeSaveFileDevice() { }

    SEAD_RTTI_OVERRIDE(CafeSaveFileDevice, FileDevice)

public:
    u8 getAccountSlotNo() const
    {
        return mAccountSlotNo;
    }

    void setFSClient(FSClient* client)
    {
        mFSClient = client;
    }

    FSClient* getFSClient() const
    {
        return mFSClient;
    }

    bool tryInitSaveDir();
    bool tryGetFreeSpaceSize(FSFreeSpaceSize* free_space_size);
    bool tryDelete(const SafeString& path);
    bool tryRename(const SafeString& old_path, const SafeString& new_path);
    bool tryFlush();
    bool tryRollback();

protected:
    virtual bool doIsAvailable_() const
    {
        return true;
    }

    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size);
    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size);
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
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    FSClient* getUsableFSClient_() const;

protected:
    struct FileHandleInner
    {
        FSFileHandle mHandle;
        FSFilePosition mPosition;
    };

    struct DirHandleInner
    {
        FSDirHandle mHandle;
    };

protected:
    FileHandleInner* getFileHandleInner_(FileHandle* handle);
    DirHandleInner* getDirHandleInner_(DirectoryHandle* handle);

protected:
    SAVEStatus mLastRawError;
    FSRetFlag mFSOpenRetFlag;
    FSRetFlag mFSCloseRetFlag;
    FSRetFlag mFSReadRetFlag;
    FSClient* mFSClient;
    u8 mAccountSlotNo;
};
static_assert(sizeof(CafeSaveFileDevice) == 0x6C, "sead::CafeSaveFileDevice size mismatch");

} // namespace sead

#endif // SEAD_CAFE_FSA_FILEDEVICE_H_
