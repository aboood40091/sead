#include <filedevice/seadAliasFileDevice.h>

namespace sead {

AliasFileDevice::AliasFileDevice(const SafeString& drive_name, FileDevice* device, const SafeString& path)
    : FileDevice(drive_name)
    , mFileDevice(device)
{
    if (!device)
    {
        //SEAD_ASSERT_MSG(false, "device is null");
        return;
    }

    mPath = path;
}

AliasFileDevice::~AliasFileDevice()
{
}

bool AliasFileDevice::doIsAvailable_() const
{
    return mFileDevice->isAvailable();
}

FileDevice* AliasFileDevice::doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag)
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(filename);
    return mFileDevice->tryOpen(handle, full_path, flag, handle->getDivSize());
}

bool AliasFileDevice::doClose_(FileHandle* handle)
{
    return mFileDevice->tryClose(handle);
}

bool AliasFileDevice::doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size)
{
    return mFileDevice->tryRead(read_size, handle, buf, size);
}

bool AliasFileDevice::doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size)
{
    return mFileDevice->tryWrite(write_size, handle, buf, size);
}

bool AliasFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    return mFileDevice->trySeek(handle, offset, origin);
}

bool AliasFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    return mFileDevice->tryGetCurrentSeekPos(pos, handle);
}

bool AliasFileDevice::doGetFileSize_(u32* size, const SafeString& path)
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    return mFileDevice->tryGetFileSize(size, full_path);
}

bool AliasFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
{
    return mFileDevice->tryGetFileSize(size, handle);
}

bool AliasFileDevice::doIsExistFile_(bool* is_exist, const SafeString& path)
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    return mFileDevice->tryIsExistFile(is_exist, full_path);
}

bool AliasFileDevice::doIsExistDirectory_(bool* is_exist, const SafeString& path)
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    return mFileDevice->tryIsExistDirectory(is_exist, full_path);
}

FileDevice* AliasFileDevice::doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname)
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(dirname);
    return mFileDevice->tryOpenDirectory(handle, full_path);
}

bool AliasFileDevice::doCloseDirectory_(DirectoryHandle* handle)
{
    return mFileDevice->closeDirectory(handle);
}

bool AliasFileDevice::doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num)
{
    return mFileDevice->tryReadDirectory(read_num, handle, entry, num);
}

bool AliasFileDevice::doMakeDirectory_(const SafeString& path, u32 permission)
{
    return mFileDevice->tryMakeDirectory(path, permission);
}

bool AliasFileDevice::isMatchDevice_(const HandleBase* handle) const
{
    return mFileDevice->isMatchDevice_(handle);
}

RawErrorCode AliasFileDevice::doGetLastRawError_() const
{
    return mFileDevice->getLastRawError();
}

void AliasFileDevice::traceFilePath(const SafeString& path) const
{
    //SEAD_PRINT("[%s] %s\n", getDriveName().cstr(), path.cstr());

    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    mFileDevice->traceFilePath(full_path);
}

void AliasFileDevice::traceDirectoryPath(const SafeString& path) const
{
    //SEAD_PRINT("[%s] %s\n", getDriveName().cstr(), path.cstr());

    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    mFileDevice->traceDirectoryPath(full_path);
}

void AliasFileDevice::resolveFilePath(BufferedSafeString* out, const SafeString& path) const
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    mFileDevice->resolveFilePath(out, full_path);
}

void AliasFileDevice::resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const
{
    FixedSafeString<512> full_path;
    full_path = mPath;
    full_path.append(path);
    mFileDevice->resolveDirectoryPath(out, full_path);
}

} // namespace sead
