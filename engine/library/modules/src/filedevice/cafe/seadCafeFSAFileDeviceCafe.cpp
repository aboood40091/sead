#include <cafe.h>

#include <filedevice/cafe/seadCafeFSAFileDeviceCafe.h>
#include <filedevice/seadFileDevice.h>
#include <filedevice/seadFileDeviceMgr.h>
#include <prim/seadSafeString.h>

namespace sead {

CafeFSAFileDevice::CafeFSAFileDevice(
    const SafeString& default_drive_name, const SafeString& cwd_path
)
    : FileDevice(default_drive_name)
    , mCWDPath(cwd_path.cstr())
    , mLastRawError(FS_STATUS_OK)
    , mFSOpenRetFlag(FS_RET_PERMISSION_ERROR | FS_RET_ACCESS_ERROR |
                     FS_RET_NOT_FILE | FS_RET_NOT_FOUND |
                     FS_RET_ALREADY_OPEN)
    , mFSCloseRetFlag(FS_RET_NO_ERROR)
    , mFSReadRetFlag(FS_RET_NO_ERROR)
    , mFSClient(nullptr)
{
}

bool
CafeFSAFileDevice::setCurrentDirectory(
    const SafeString& path
)
{

    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = FileDeviceMgr::instance()->getFSClient_();

    FSChangeDir(client, &block, path.cstr(), FS_RET_NO_ERROR);
    return true;
}

FileDevice*
CafeFSAFileDevice::doOpen_(
    FileHandle* handle, const SafeString& filename,
    FileDevice::FileOpenFlag flag
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    char* mode;
    switch (flag)
    {
    case FileDevice::cFileOpenFlag_ReadOnly:
        mode = "r";
        break;
    case FileDevice::cFileOpenFlag_WriteOnly:
        mode = "w";
        break;
    case FileDevice::cFileOpenFlag_ReadWrite:
        mode = "r+";
        break;
    case FileDevice::cFileOpenFlag_Create:
        mode = "w+";
        break;
    default:
        //SEAD_ASSERT_MSG(false, "illegal open flag[%d]", (s32)flag);
        mode = "r";
    }

    FixedSafeString<FileDeviceMgr::cNoDrivePathBufferSize> file_path;
    formatPathForFSA_(&file_path, filename);

    FSStatus status = FSOpenFile(client, &block, file_path.cstr(), mode, &handle_inner->mHandle, mFSOpenRetFlag);
    handle_inner->mPosition = 0;

    if (mLastRawError = status, status != FS_STATUS_OK)
    {
        handle_inner->mHandle = 0;
        return nullptr;
    }

    return this;
}

bool
CafeFSAFileDevice::doClose_(FileHandle* handle)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    FSStatus status = FSCloseFile(client, &block, handle_inner->mHandle, mFSCloseRetFlag);
    if (/*mLastRawError = status, */status != FS_STATUS_OK)
        return false;

    return true;
}

bool
CafeFSAFileDevice::doRead_(
    u32* read_size, FileHandle* handle,
    u8* buf, u32 size
)
{
    //SEAD_ASSERT_MSG(((uintptr_t)buf & LL_CACHE_FETCH_SIZE - 1u) == 0, "buf[0x%p] is not aligned with LL_CACHE_FETCH_SIZE[%d]", buf, LL_CACHE_FETCH_SIZE);

    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    s32 result = FSReadFile(client, &block, buf, sizeof(u8), size, handle_inner->mHandle, 0, mFSReadRetFlag);
    if (result >= FS_STATUS_OK)
    {
        mLastRawError = FS_STATUS_OK;
        handle_inner->mPosition += result;

        if (read_size != nullptr)
            *read_size = result;

        return true;
    }

    mLastRawError = result;
    return false;
}

bool
CafeFSAFileDevice::doWrite_(
    u32* write_size, FileHandle* handle,
    const u8* buf, u32 size
)
{
    //SEAD_ASSERT_MSG(((uintptr_t)buf & LL_CACHE_FETCH_SIZE - 1u) == 0, "buf[0x%p] is not aligned with LL_CACHE_FETCH_SIZE[%d]", buf, LL_CACHE_FETCH_SIZE);

    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    s32 result = FSWriteFile(client, &block, buf, sizeof(const u8), size, handle_inner->mHandle, 0, FS_RET_STORAGE_FULL | FS_RET_FILE_TOO_BIG);
    if (result >= FS_STATUS_OK)
    {
        mLastRawError = FS_STATUS_OK;
        handle_inner->mPosition += result;

        if (write_size != nullptr)
            *write_size = result;

        return true;
    }

    mLastRawError = result;
    return false;
}

bool
CafeFSAFileDevice::doSeek_(
    FileHandle* handle, s32 offset, FileDevice::SeekOrigin origin
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    switch (origin)
    {
    case FileDevice::cSeekOrigin_Begin:
        break;
    case FileDevice::cSeekOrigin_Current:
        offset += handle_inner->mPosition;
        break;
    case FileDevice::cSeekOrigin_End:
        //SEAD_ASSERT(offset <= 0);
        {
            u32 size = 0;
            if (doGetFileSize_(&size, handle))
                offset += size;

            else
                return false;
        }
        break;
    default:
        return false;
    }

    FSStatus status = FSSetPosFile(client, &block, handle_inner->mHandle, offset, FS_RET_NO_ERROR);
    if (mLastRawError = status, status == FS_STATUS_OK)
    {
        handle_inner->mPosition = offset;
        return true;
    }

    return false;
}

bool
CafeFSAFileDevice::doGetCurrentSeekPos_(
    u32* pos, FileHandle* handle
)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    *pos = handle_inner->mPosition;
    return true;
}

bool
CafeFSAFileDevice::doGetFileSize_(
    u32* size, const SafeString& path
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();

    FixedSafeString<FileDeviceMgr::cNoDrivePathBufferSize> file_path;
    formatPathForFSA_(&file_path, path);

    FSStat stat;
    FSStatus status = FSGetStat(client, &block, file_path.cstr(), &stat, FS_RET_NO_ERROR);

    if (mLastRawError = status, status != FS_STATUS_OK)
        return false;

    *size = stat.size;
    return true;
}

bool
CafeFSAFileDevice::doGetFileSize_(
    u32* size, FileHandle* handle
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);

    FSStat stat;
    FSStatus status = FSGetStatFile(client, &block, handle_inner->mHandle, &stat, FS_RET_NO_ERROR);

    if (mLastRawError = status, status != FS_STATUS_OK)
        return false;

    *size = stat.size;
    return true;
}

bool
CafeFSAFileDevice::doIsExistFile_(
    bool* is_exist, const SafeString& path
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();

    FixedSafeString<FileDeviceMgr::cNoDrivePathBufferSize> file_path;
    formatPathForFSA_(&file_path, path);

    FSStat stat;
    FSStatus status = FSGetStat(client, &block, file_path.cstr(), &stat, FS_RET_PERMISSION_ERROR | FS_RET_NOT_FOUND);

    if (mLastRawError = status, status != FS_STATUS_OK)
    {
        if (status != FS_STATUS_NOT_FOUND)
            return false;

        *is_exist = false;
    }
    else
    {
        *is_exist = (stat.flag & (FS_STAT_FLAG_IS_DIRECTORY | FS_STAT_FLAG_IS_QUOTA)) == 0;
    }

    return true;
}

bool
CafeFSAFileDevice::doIsExistDirectory_(
    bool* is_exist, const SafeString& path
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();

    FixedSafeString<FileDeviceMgr::cNoDrivePathBufferSize> dir_path;
    formatPathForFSA_(&dir_path, path);

    FSStat stat;
    FSStatus status = FSGetStat(client, &block, dir_path.cstr(), &stat, FS_RET_PERMISSION_ERROR | FS_RET_NOT_FOUND);

    if (mLastRawError = status, status != FS_STATUS_OK)
    {
        if (status != FS_STATUS_NOT_FOUND)
            return false;

        *is_exist = false;
    }
    else
    {
        *is_exist = stat.flag & FS_STAT_FLAG_IS_DIRECTORY;
    }

    return true;
}

FileDevice*
CafeFSAFileDevice::doOpenDirectory_(
    DirectoryHandle* handle, const SafeString& dirname
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    DirHandleInner* handle_inner = getDirHandleInner_(handle);

    FixedSafeString<FileDeviceMgr::cNoDrivePathBufferSize> dir_path;
    formatPathForFSA_(&dir_path, dirname);

    FSStatus status = FSOpenDir(client, &block, dir_path.cstr(), &handle_inner->mHandle, FS_RET_PERMISSION_ERROR | FS_RET_ACCESS_ERROR |
                                                                                         FS_RET_NOT_DIR | FS_RET_NOT_FOUND | FS_RET_ALREADY_OPEN);

    if (mLastRawError = status, status != FS_STATUS_OK)
        return nullptr;

    return this;
}

bool
CafeFSAFileDevice::doCloseDirectory_(
    DirectoryHandle* handle
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    DirHandleInner* handle_inner = getDirHandleInner_(handle);

    FSStatus status = FSCloseDir(client, &block, handle_inner->mHandle, FS_RET_NO_ERROR);
    if (mLastRawError = status, status != FS_STATUS_OK)
        return false;

    return true;
}

bool
CafeFSAFileDevice::doReadDirectory_(
    u32* read_num, DirectoryHandle* handle,
    DirectoryEntry* entry, u32 num
)
{
    FSDirEntry dir_entry;

    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();
    DirHandleInner* handle_inner = getDirHandleInner_(handle);

    for (u32 i = 0; i < num; i++)
    {
        FSStatus status = FSReadDir(client, &block, handle_inner->mHandle, &dir_entry, FS_RET_NO_ERROR);
        if (mLastRawError = status, status != FS_STATUS_OK)
        {
            if (read_num != nullptr)
                *read_num = i;

            if (status == FS_STATUS_END)
                return true;

            return false;
        }

        entry[i].name.copy(SafeString(dir_entry.name));
        entry[i].is_directory = dir_entry.stat.flag & FS_STAT_FLAG_IS_DIRECTORY;
    }

    if (read_num != nullptr)
        *read_num = num;

    return true;
}

bool
CafeFSAFileDevice::doMakeDirectory_(
    const SafeString& path, u32 permission
)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);

    FSClient* client = getUsableFSClient_();

    FixedSafeString<FileDeviceMgr::cNoDrivePathBufferSize> dir_path;
    formatPathForFSA_(&dir_path, path);

    FSStatus status = FSMakeDir(client, &block, dir_path.cstr(), FS_RET_JOURNAL_FULL | FS_RET_STORAGE_FULL |
                                                                 FS_RET_PERMISSION_ERROR | FS_RET_NOT_FOUND);
    if (mLastRawError = status, status != FS_STATUS_OK)
        return false;

    return true;
}

RawErrorCode CafeFSAFileDevice::doGetLastRawError_() const
{
    return mLastRawError;
}

void
CafeFSAFileDevice::doResolvePath_(
    BufferedSafeString* out, const SafeString& path
) const
{
    formatPathForFSA_(out, path);
}

void
CafeFSAFileDevice::formatPathForFSA_(
    BufferedSafeString* out, const SafeString& path
) const
{
    out->format("%s/%s", mCWDPath, path.cstr());
}

FSClient*
CafeFSAFileDevice::getUsableFSClient_() const
{
    if (mFSClient == nullptr)
        return FileDeviceMgr::instance()->getFSClient_();

    return mFSClient;
}
CafeFSAFileDevice::FileHandleInner*
CafeFSAFileDevice::getFileHandleInner_(
    FileHandle* handle
)
{
    return reinterpret_cast<FileHandleInner*>(getHandleBaseHandleBuffer_(handle).getBufferPtr());
}
CafeFSAFileDevice::DirHandleInner*
CafeFSAFileDevice::getDirHandleInner_(
    DirectoryHandle* handle
)
{
    return reinterpret_cast<DirHandleInner*>(getHandleBaseHandleBuffer_(handle).getBufferPtr());
}

CafeContentFileDevice::CafeContentFileDevice()
    : CafeFSAFileDevice("content", FS_CONTENT_DIR)
{
}

CafeFSNativePathFileDevice::CafeFSNativePathFileDevice()
    : CafeFSAFileDevice("native", "")
{
}

void CafeFSNativePathFileDevice::formatPathForFSA_(BufferedSafeString* out, const SafeString& path) const
{
    out->copy(path);
}

} // namespace sead
