#include <filedevice/seadAnyFileDevice.h>

namespace {

sead::FixedSafeString<512> sOpenedDirectoryName;

}

namespace sead {

AnyFileDevice::AnyFileDevice(s32 limit_num, Heap* heap, s32 alignment)
    : FileDevice("any")
    , mLastFileDevice(nullptr)
{
    mFileDeviceList.allocBuffer(limit_num, heap, alignment);
}

AnyFileDevice::~AnyFileDevice()
{
    mFileDeviceList.freeBuffer();
}

bool AnyFileDevice::pushBack(FileDevice* device)
{
    //SEAD_ASSERT(device);

    if (mFileDeviceList.isFull())
        return false;

    mFileDeviceList.pushBack(device);
    return true;
}

bool AnyFileDevice::pushFront(FileDevice* device)
{
    //SEAD_ASSERT(device);

    if (mFileDeviceList.isFull())
        return false;

    mFileDeviceList.pushFront(device);
    return true;
}

bool AnyFileDevice::erase(FileDevice* device)
{
    //SEAD_ASSERT(device);

    s32 index = mFileDeviceList.indexOf(device);
    if (index == -1)
        return false;

    mFileDeviceList.erase(index);
    return true;
}

bool AnyFileDevice::doIsAvailable_() const
{
    for (FileDeviceList::constIterator it = mFileDeviceList.constBegin(); it != mFileDeviceList.constEnd(); ++it)
        if (it->isAvailable())
            return true;

    return false;
}

u8* AnyFileDevice::doLoad_(LoadArg& arg)
{
    FileDevice* device = findFileDeviceByFile_(arg.path);
    if (!device)
        return nullptr;

    mLastFileDevice = device;
    return device->tryLoad(arg);
}

FileDevice* AnyFileDevice::findFileDeviceByFile_(const SafeString& path) const
{
    for (FileDeviceList::iterator it = mFileDeviceList.begin(); it != mFileDeviceList.end(); ++it)
    {
        FileDevice* device = &(*it);
        if (device->isAvailable() && device->isExistFile(path))
            return device;
    }

    return nullptr;
}

FileDevice* AnyFileDevice::findFileDeviceByDirectory_(const SafeString& path) const
{
    for (FileDeviceList::iterator it = mFileDeviceList.begin(); it != mFileDeviceList.end(); ++it)
    {
        FileDevice* device = &(*it);
        if (device->isAvailable() && device->isExistDirectory(path))
            return device;
    }

    return nullptr;
}

FileDevice* AnyFileDevice::doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag)
{
    FileDevice* device = findFileDeviceByFile_(filename);
    if (!device)
        return nullptr;

    mLastFileDevice = device;
    return device->tryOpen(handle, filename, flag, handle->getDivSize());
}

bool AnyFileDevice::doClose_(FileHandle* handle)
{
    mLastFileDevice = handle->getDevice();
    return mLastFileDevice->tryClose(handle);
}

bool AnyFileDevice::doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size)
{
    mLastFileDevice = handle->getDevice();
    return mLastFileDevice->tryRead(read_size, handle, buf, size);
}

bool AnyFileDevice::doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size)
{
    mLastFileDevice = handle->getDevice();
    return mLastFileDevice->tryWrite(write_size, handle, buf, size);
}

bool AnyFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    mLastFileDevice = handle->getDevice();
    return mLastFileDevice->trySeek(handle, offset, origin);
}

bool AnyFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    mLastFileDevice = handle->getDevice();
    return mLastFileDevice->tryGetCurrentSeekPos(pos, handle);
}

bool AnyFileDevice::doGetFileSize_(u32* size, const SafeString& path)
{
    FileDevice* device = findFileDeviceByFile_(path);
    if (!device)
        return false;

    mLastFileDevice = device;
    return device->tryGetFileSize(size, path);
}

bool AnyFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
{
    mLastFileDevice = handle->getDevice();
    return mLastFileDevice->tryGetFileSize(size, handle);
}

bool AnyFileDevice::doIsExistFile_(bool* is_exist, const SafeString& path)
{
    FileDevice* device = findFileDeviceByFile_(path);
    mLastFileDevice = device;
    *is_exist = device != nullptr;
    return true;
}

bool AnyFileDevice::doIsExistDirectory_(bool* is_exist, const SafeString& path)
{
    FileDevice* device = findFileDeviceByDirectory_(path);
    mLastFileDevice = device;
    *is_exist = device != nullptr;
    return true;
}

FileDevice* AnyFileDevice::doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname)
{
    if (!sOpenedDirectoryName.isEmpty())
    {
        //SEAD_ASSERT_MSG(false, "Already opened directory");
        return nullptr;
    }

    FileDevice* device = nullptr;
    for (FileDeviceList::iterator itr_end = mFileDeviceList.end(), itr = mFileDeviceList.begin(); itr != itr_end; ++itr)
        if ((device = itr->tryOpenDirectory(handle, dirname)))
            break;
    if (device)
    {
        mLastFileDevice = device;
        sOpenedDirectoryName = dirname;
    }
    return device;
}

bool AnyFileDevice::doCloseDirectory_(DirectoryHandle* handle)
{
    mLastFileDevice = handle->getDevice();
    sOpenedDirectoryName.clear();
    return mLastFileDevice->closeDirectory(handle);
}

bool AnyFileDevice::doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num)
{
    u32 div_read_num = 0;
    if (!handle->getDevice()->tryReadDirectory(&div_read_num, handle, entry, num))
    {
        if (read_num != nullptr)
            *read_num = div_read_num;
        return false;
    }
    if (div_read_num >= num)
    {
        if (read_num != nullptr)
            *read_num = div_read_num;
        return true;
    }
    u32 remaining_num = num - div_read_num;
    DirectoryEntry* remaining_entry = entry + div_read_num;
    u32 total_read_num = div_read_num;
    FileDeviceList::iterator it = mFileDeviceList.begin();
    for (; it != mFileDeviceList.end(); ++it)
    {
        FileDevice* device = &(*it);
        if (handle->getDevice() == device)
        {
            ++it;
            break;
        }
    }
    while (true)
    {
        if (it == mFileDeviceList.end())
        {
            if (read_num != nullptr)
                *read_num = total_read_num;
            return true;
        }

        FileDevice* device = it->tryOpenDirectory(handle, sOpenedDirectoryName);
        if (!device)
        {
            if (read_num != nullptr)
                *read_num = total_read_num;
            return false;
        }
        mLastFileDevice = device;
        if (!handle->getDevice()->tryReadDirectory(&div_read_num, handle, remaining_entry, remaining_num))
        {
            if (read_num != nullptr)
                *read_num = total_read_num;
            return false;
        }
        if (div_read_num >= remaining_num)
        {
            if (read_num != nullptr)
                *read_num = total_read_num + div_read_num;
            return true;
        }
        remaining_num -= div_read_num;
        remaining_entry += div_read_num;
        total_read_num += div_read_num;
        ++it;
    }
}

bool AnyFileDevice::doMakeDirectory_(const SafeString& path, u32 permission)
{
    return false;
}

bool AnyFileDevice::isMatchDevice_(const HandleBase* handle) const
{
    for (FileDeviceList::constIterator it = mFileDeviceList.constBegin(); it != mFileDeviceList.constEnd(); ++it)
        if (it->isMatchDevice_(handle))
            return true;

    return false;
}

RawErrorCode AnyFileDevice::doGetLastRawError_() const
{
    if (!mLastFileDevice)
        return 0;

    return mLastFileDevice->getLastRawError();
}

void AnyFileDevice::traceFilePath(const SafeString& path) const
{
    //SEAD_PRINT("[%s] %s\n", getDriveName().cstr(), path.cstr());

    FileDevice* device = findFileDeviceByFile_(path);
    if (!device)
    {
        //SEAD_WARNING(false, "FileDevice not found: %s\n", path.cstr());
        return;
    }

    device->traceFilePath(path);
}

void AnyFileDevice::traceDirectoryPath(const SafeString& path) const
{
    //SEAD_PRINT("[%s] %s\n", getDriveName().cstr(), path.cstr());

    FileDevice* device = findFileDeviceByDirectory_(path);
    if (!device)
    {
        //SEAD_WARNING(false, "FileDevice not found: %s\n", path.cstr());
        return;
    }

    device->traceDirectoryPath(path);
}

void AnyFileDevice::resolveFilePath(BufferedSafeString* out, const SafeString& path) const
{
    FileDevice* device = findFileDeviceByFile_(path);
    if (!device)
    {
        //SEAD_WARNING(false, "FileDevice not found: %s\n", path.cstr());
        return;
    }

    device->resolveFilePath(out, path);
}

void AnyFileDevice::resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const
{
    FileDevice* device = findFileDeviceByDirectory_(path);
    if (!device)
    {
        //SEAD_WARNING(false, "FileDevice not found: %s\n", path.cstr());
        return;
    }

    device->resolveDirectoryPath(out, path);
}

} // namespace sead
