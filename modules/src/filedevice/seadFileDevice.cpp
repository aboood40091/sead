#include <basis/seadNew.h>
#include <filedevice/seadFileDevice.h>
#include <filedevice/seadFileDeviceMgr.h>
#include <math/seadMathCalcCommon.h>
#include <prim/seadSafeString.h>

#include <stdlib.h>

namespace sead {

u32 FileHandle::read(u8* buf, u32 size)
{
    if (mDevice != NULL)
        return mDevice->read(this, buf, size);

    //SEAD_ASSERT_MSG(false, "handle not opened");
    return 0;
}

FileDevice::~FileDevice()
{
    if (FileDeviceMgr::instance() != NULL)
        FileDeviceMgr::instance()->unmount(this);
}

bool
FileDevice::isMatchDevice_(
    const HandleBase* handle
) const
{
    return handle->mDevice == this;
}

u8* FileDevice::doLoad_(LoadArg& arg)
{
    if (arg.buffer != NULL && arg.buffer_size == 0)
    {
        //SEAD_WARNING(false, "arg.buffer is specified, but arg.buffer_size is zero\n");
        return NULL;
    }

    FileHandle handle;
    if (tryOpen(&handle, arg.path, FileDevice::cFileOpenFlag_ReadOnly, arg.div_size) == NULL)
        return NULL;

    u32 file_size = 0;
    if (!tryGetFileSize(&file_size, &handle))
        return NULL;

    //SEAD_ASSERT(file_size != 0);

    u32 buffer_size = arg.buffer_size;
    if (buffer_size == 0)
        buffer_size = Mathi::roundUpPow2(file_size, FileDevice::cBufferMinAlignment);

    else if (buffer_size < file_size)
    {
        //SEAD_WARNING(false, "arg.buffer_size[%u] is smaller than file size[%u]\n", buffer_size, file_size);
        return NULL;
    }

    u8* buffer = arg.buffer;
    bool need_unload = false;

    if (buffer == NULL)
    {
        buffer = new (arg.heap, Mathi::sign(arg.alignment) * Mathi::max(Mathi::abs(arg.alignment), FileDevice::cBufferMinAlignment)) u8[buffer_size];
        need_unload = true;
    }

    u32 read_size = 0;
    if (!tryRead(&read_size, &handle, buffer, buffer_size))
    {
        if (need_unload)
            delete[] buffer;

        return NULL;
    }

    if (!tryClose(&handle))
    {
        if (need_unload)
            delete[] buffer;

        return NULL;
    }

    arg.read_size = read_size;
    arg.roundup_size = buffer_size;
    arg.need_unload = need_unload;

    return buffer;
}

void
FileDevice::doTracePath_(
    const SafeString& path
) const
{
    FixedSafeString<512> out;
    doResolvePath_(&out, path);
}

void
FileDevice::doResolvePath_(
    BufferedSafeString* out, const SafeString& path
) const
{
    out->copy(path);
}

bool FileDevice::isAvailable() const
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    return doIsAvailable_();
}

u8* FileDevice::tryLoad(LoadArg& arg)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return NULL;

    return doLoad_(arg);
}

FileDevice*
FileDevice::tryOpen(
    FileHandle* handle, const SafeString& filename,
    FileOpenFlag flag, u32 div_size
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return NULL;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return NULL;
    }

    setFileHandleDivSize_(handle, div_size);
    FileDevice* device = doOpen_(handle, filename, flag);
    setHandleBaseFileDevice_(handle, device);
    if (device != NULL)
        setHandleBaseOriginalFileDevice_(handle, this);

    return device;
}

bool
FileDevice::tryClose(FileHandle* handle)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    bool success = doClose_(handle);
    if (success)
    {
        setHandleBaseFileDevice_(handle, NULL);
        setHandleBaseOriginalFileDevice_(handle, NULL);
    }

    return success;
}

bool
FileDevice::tryRead(
    u32* read_size, FileHandle* handle,
    u8* buf, u32 size
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    if (buf == NULL)
    {
        //SEAD_ASSERT_MSG(false, "buf is null");
        return false;
    }

    if (handle->mDivSize == 0)
    {
        bool success = doRead_(read_size, handle, buf, size);
        //SEAD_ASSERT_MSG(read_size == NULL || *read_size <= size, "buffer overflow");
        return success;
    }

    u32 total_read_size = 0;

    do
    {
        u32 buf_size = Mathi::min(size, handle->mDivSize);
        u32 tmp_read_size = 0;

        if (!doRead_(&tmp_read_size, handle, buf, buf_size))
        {
            if (read_size != NULL)
                *read_size = total_read_size;

            return false;
        }

        total_read_size += tmp_read_size;
        if (tmp_read_size < buf_size)
            break;

        buf += tmp_read_size;
        size -= buf_size;
    }
    while (size != 0);

    if (read_size != NULL)
        *read_size = total_read_size;

    return true;
}

bool
FileDevice::tryWrite(
    u32* write_size, FileHandle* handle,
    const u8* buf, u32 size
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (buf == NULL)
    {
        //SEAD_ASSERT_MSG(false, "buf is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    return doWrite_(write_size, handle, buf, size);
}

bool
FileDevice::trySeek(
    FileHandle* handle, s32 offset,
    FileDevice::SeekOrigin origin
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    return doSeek_(handle, offset, origin);
}

bool
FileDevice::tryGetCurrentSeekPos(
    u32* pos, FileHandle* handle
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    if (pos == NULL)
    {
        //SEAD_ASSERT_MSG(false, "pos is null");
        return false;
    }

    return doGetCurrentSeekPos_(pos, handle);
}

bool
FileDevice::tryGetFileSize(
    u32* size, const SafeString& path
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (size == NULL)
    {
        //SEAD_ASSERT_MSG(false, "size is null");
        return false;
    }

    return doGetFileSize_(size, path);
}

bool
FileDevice::tryGetFileSize(
    u32* size, FileHandle* handle
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (size == NULL)
    {
        //SEAD_ASSERT_MSG(false, "size is null");
        return false;
    }

    return doGetFileSize_(size, handle);
}

bool
FileDevice::tryIsExistFile(
    bool* is_exist, const SafeString& path
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (is_exist == NULL)
    {
        //SEAD_ASSERT_MSG(false, "is_exist is null");
        return false;
    }

    return doIsExistFile_(is_exist, path);
}

bool
FileDevice::tryIsExistDirectory(
    bool* is_exist, const SafeString& path
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (is_exist == NULL)
    {
        //SEAD_ASSERT_MSG(false, "is_exist is null");
        return false;
    }

    return doIsExistDirectory_(is_exist, path);
}

FileDevice*
FileDevice::tryOpenDirectory(
    DirectoryHandle* handle, const SafeString& dirname
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return NULL;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return NULL;
    }

    FileDevice* device = doOpenDirectory_(handle, dirname);
    setHandleBaseFileDevice_(handle, device);
    if (device != NULL)
        setHandleBaseOriginalFileDevice_(handle, this);

    return device;
}

bool
FileDevice::tryCloseDirectory(DirectoryHandle* handle)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    bool success = doCloseDirectory_(handle);
    if (success)
    {
        setHandleBaseFileDevice_(handle, NULL);
        setHandleBaseOriginalFileDevice_(handle, NULL);
    }

    return success;
}

bool
FileDevice::tryReadDirectory(
    u32* read_num, DirectoryHandle* handle,
    DirectoryEntry* entry, u32 num
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    if (handle == NULL)
    {
        //SEAD_ASSERT_MSG(false, "handle is null");
        return false;
    }

    if (!isMatchDevice_(handle))
    {
        //SEAD_ASSERT_MSG(false, "handle device miss match");
        return false;
    }

    u32 tmp_read_num = 0;
    bool success = doReadDirectory_(&tmp_read_num, handle, entry, num);

    if (read_num != NULL)
        *read_num = tmp_read_num;

    if (tmp_read_num > num)
    {
        //SEAD_ASSERT_MSG(false, "buffer overflow");
        return false;
    }

    return success;
}

bool
FileDevice::tryMakeDirectory(
    const SafeString& path, u32 permission
)
{
    //SEAD_ASSERT_MSG(mPermission, "Device permission error.");

    if (!mPermission)
        return false;

    return doMakeDirectory_(path, permission);
}

RawErrorCode FileDevice::getLastRawError() const
{
    return doGetLastRawError_();
}

void
FileDevice::setFileHandleDivSize_(
    FileHandle* handle, u32 div_size
) const
{
    handle->mDivSize = div_size;
}

void
FileDevice::setHandleBaseFileDevice_(
    HandleBase* handle, FileDevice* device
) const
{
    handle->mDevice = device;
}

void
FileDevice::setHandleBaseOriginalFileDevice_(
    HandleBase* handle, FileDevice* device
) const
{
    handle->mOriginalDevice = device;
}

HandleBuffer&
FileDevice::getHandleBaseHandleBuffer_(
    HandleBase* handle
) const
{
    return handle->mHandleBuffer;
}

} // namespace sead
