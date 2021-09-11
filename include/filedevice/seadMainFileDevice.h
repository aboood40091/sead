#ifndef SEAD_MAIN_FILEDEVICE_H_
#define SEAD_MAIN_FILEDEVICE_H_

#include <filedevice/seadFileDevice.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

namespace sead {

class MainFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(MainFileDevice, FileDevice)

public:
    MainFileDevice(Heap* heap);
    virtual ~MainFileDevice();

    virtual void traceFilePath(const SafeString& path) const;
    virtual void traceDirectoryPath(const SafeString& path) const;
    virtual void resolveFilePath(BufferedSafeString* out, const SafeString& path) const;
    virtual void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const;

protected:
    virtual bool doIsAvailable_() const
    {
        return mFileDevice->isAvailable();
    }

    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag)
    {
        return mFileDevice->tryOpen(handle, filename, flag, handle->getDivSize());
    }

    virtual bool doClose_(FileHandle* handle)
    {
        return mFileDevice->tryClose(handle);
    }

    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size)
    {
        return mFileDevice->tryRead(read_size, handle, buf, size);
    }

    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size)
    {
        return mFileDevice->tryWrite(write_size, handle, buf, size);
    }

    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
    {
        return mFileDevice->trySeek(handle, offset, origin);
    }

    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
    {
        return mFileDevice->tryGetCurrentSeekPos(pos, handle);
    }

    virtual bool doGetFileSize_(u32* size, const SafeString& path)
    {
        return mFileDevice->tryGetFileSize(size, path);
    }

    virtual bool doGetFileSize_(u32* size, FileHandle* handle)
    {
        return mFileDevice->tryGetFileSize(size, handle);
    }

    virtual bool doIsExistFile_(bool* is_exist, const SafeString& path)
    {
        return mFileDevice->tryIsExistFile(is_exist, path);
    }

    virtual bool doIsExistDirectory_(bool* is_exist, const SafeString& path)
    {
        return mFileDevice->tryIsExistDirectory(is_exist, path);
    }

    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname)
    {
        return mFileDevice->tryOpenDirectory(handle, dirname);
    }

    virtual bool doCloseDirectory_(DirectoryHandle* handle)
    {
        return mFileDevice->tryCloseDirectory(handle);
    }

    virtual bool doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num)
    {
        return mFileDevice->tryReadDirectory(read_num, handle, entry, num);
    }

    virtual bool doMakeDirectory_(const SafeString& path, u32 permission)
    {
        return mFileDevice->tryMakeDirectory(path, permission);
    }

    virtual bool isMatchDevice_(const HandleBase* handle) const
    {
        return mFileDevice->isMatchDevice_(handle);
    }

    virtual RawErrorCode doGetLastRawError_() const
    {
        return mFileDevice->getLastRawError();
    }

protected:
    FileDevice* mFileDevice;
};

} // namespace sead

#endif // SEAD_MAIN_FILEDEVICE_H_
