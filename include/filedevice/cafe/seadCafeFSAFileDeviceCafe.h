#ifndef SEAD_CAFE_FSA_FILEDEVICE_H_
#define SEAD_CAFE_FSA_FILEDEVICE_H_

#include <cafe.h>

#include <filedevice/seadFileDevice.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

namespace sead {

class CafeFSAFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(CafeFSAFileDevice, FileDevice)

public:
    CafeFSAFileDevice(const SafeString& default_drive_name, const SafeString& cwd_path);
    virtual ~CafeFSAFileDevice() { }

    void changeDir(const char* path)
    {
        mCWDPath = SafeString(path).cstr();
    }

    const char* getCWD()
    {
        return mCWDPath;
    }

    bool setCurrentDirectory(const SafeString& path);

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
    virtual void formatPathForFSA_(BufferedSafeString* out, const SafeString& path) const;

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
    const char* mCWDPath;
    FSStatus mLastRawError;
    FSRetFlag mFSOpenRetFlag;
    FSRetFlag mFSCloseRetFlag;
    FSRetFlag mFSReadRetFlag;
    FSClient* mFSClient;
};
static_assert(sizeof(CafeFSAFileDevice) == 0x6C, "sead::CafeFSAFileDevice size mismatch");

class CafeContentFileDevice : public CafeFSAFileDevice
{
    SEAD_RTTI_OVERRIDE(CafeContentFileDevice, CafeFSAFileDevice)

public:
    CafeContentFileDevice();
    virtual ~CafeContentFileDevice() { }
};

class CafeSDFileDevice : public CafeFSAFileDevice
{
    SEAD_RTTI_OVERRIDE(CafeSDFileDevice, CafeFSAFileDevice)

public:
    CafeSDFileDevice();
    virtual ~CafeSDFileDevice() { }
};

class CafeHostIOFileDevice : public CafeFSAFileDevice
{
    SEAD_RTTI_OVERRIDE(CafeHostIOFileDevice, CafeFSAFileDevice)

public:
    CafeHostIOFileDevice();
    virtual ~CafeHostIOFileDevice() { }

protected:
    virtual void formatPathForFSA_(BufferedSafeString* out, const SafeString& path) const;

    void convertPathWinToFSA_(char*, u32, const char*) const;
};

class CafeFSNativePathFileDevice : public CafeFSAFileDevice
{
    SEAD_RTTI_OVERRIDE(CafeFSNativePathFileDevice, CafeFSAFileDevice)

public:
    CafeFSNativePathFileDevice();
    virtual ~CafeFSNativePathFileDevice() { }

protected:
    virtual void formatPathForFSA_(BufferedSafeString* out, const SafeString& path) const;
};

} // namespace sead

#endif // SEAD_CAFE_FSA_FILEDEVICE_H_
