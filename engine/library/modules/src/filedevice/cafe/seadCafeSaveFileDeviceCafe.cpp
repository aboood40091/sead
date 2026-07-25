#include <filedevice/cafe/seadCafeSaveFileDeviceCafe.h>
#include <filedevice/seadFileDeviceMgr.h>

namespace sead {

CafeSaveFileDevice::CafeSaveFileDevice(u8 account_slot_no)
    : FileDevice("save")
    , mLastRawError(SAVE_STATUS_OK)
    , mFSOpenRetFlag(FS_RET_ALREADY_OPEN |
                     FS_RET_NOT_FOUND |
                     FS_RET_NOT_FILE |
                     FS_RET_ACCESS_ERROR |
                     FS_RET_PERMISSION_ERROR)
    , mFSCloseRetFlag(FS_RET_NO_ERROR)
    , mFSReadRetFlag(FS_RET_NO_ERROR)
    , mFSClient(nullptr)
    , mAccountSlotNo(account_slot_no)
{
}

bool CafeSaveFileDevice::tryInitSaveDir()
{
    SAVEStatus status = SAVEInitSaveDir(mAccountSlotNo);
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

bool CafeSaveFileDevice::tryGetFreeSpaceSize(FSFreeSpaceSize* free_space_size)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    SAVEStatus status = SAVEGetFreeSpaceSize(client, &block, mAccountSlotNo, free_space_size, FS_RET_NOT_FOUND);
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

bool CafeSaveFileDevice::tryDelete(const SafeString& path)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    SAVEStatus status = SAVERemove(client, &block, mAccountSlotNo, path.cstr(), (
        FS_RET_ALREADY_OPEN |
        FS_RET_NOT_FOUND |
        FS_RET_PERMISSION_ERROR |
        FS_RET_STORAGE_FULL |
        FS_RET_JOURNAL_FULL
    ));
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

bool CafeSaveFileDevice::tryRename(const SafeString& old_path, const SafeString& new_path)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    SAVEStatus status = SAVERename(client, &block, mAccountSlotNo, old_path.cstr(), new_path.cstr(), (
        FS_RET_ALREADY_OPEN |
        FS_RET_NOT_FOUND |
        FS_RET_PERMISSION_ERROR |
        FS_RET_STORAGE_FULL |
        FS_RET_JOURNAL_FULL
    ));
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

bool CafeSaveFileDevice::tryFlush()
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    SAVEStatus status = SAVEFlushQuota(client, &block, mAccountSlotNo, FS_RET_NO_ERROR);
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

bool CafeSaveFileDevice::tryRollback()
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    SAVEStatus status = SAVERollbackQuota(client, &block, mAccountSlotNo, (
        FS_RET_ALL_ERROR & ~(FS_RET_MAX | FS_RET_ALREADY_OPEN)
    ));
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

FileDevice* CafeSaveFileDevice::doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag)
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
    SAVEStatus status = SAVEOpenFile(client, &block, mAccountSlotNo, filename.cstr(), mode, &handle_inner->mHandle, mFSOpenRetFlag);
    handle_inner->mPosition = 0;
    if (mLastRawError = status, status != SAVE_STATUS_OK)
    {
        handle_inner->mHandle = 0;
        return nullptr;
    }
    return this;
}

bool CafeSaveFileDevice::doClose_(FileHandle* handle)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    FSStatus status = FSCloseFile(client, &block, handle_inner->mHandle, mFSCloseRetFlag);
    return status == FS_STATUS_OK;
}

bool CafeSaveFileDevice::doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size)
{
    //SEAD_ASSERT_MSG((uintptr)buf % LL_CACHE_FETCH_SIZE == 0, "buf[0x%p] is not aligned with LL_CACHE_FETCH_SIZE[%d]", buf, LL_CACHE_FETCH_SIZE);
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    FSStatus status = FSReadFile(client, &block, buf, sizeof(u8), size, handle_inner->mHandle, 0, mFSReadRetFlag);
    if (status >= 0)
    {
        mLastRawError = SAVE_STATUS_OK;
        handle_inner->mPosition += status;
        if (read_size != nullptr)
            *read_size = status;
        return true;
    }
    else
    {
        mLastRawError = status;
        return false;
    }
}

bool CafeSaveFileDevice::doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size)
{
    //SEAD_ASSERT_MSG((uintptr)buf % LL_CACHE_FETCH_SIZE == 0, "buf[0x%p] is not aligned with LL_CACHE_FETCH_SIZE[%d]", buf, LL_CACHE_FETCH_SIZE);
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    FSStatus status = FSWriteFile(client, &block, buf, sizeof(const u8), size, handle_inner->mHandle, 0, FS_RET_NO_ERROR);
    if (status >= 0)
    {
        mLastRawError = SAVE_STATUS_OK;
        handle_inner->mPosition += status;
        if (write_size != nullptr)
            *write_size = status;
        return true;
    }
    else
    {
        mLastRawError = status;
        return false;
    }
}

bool CafeSaveFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
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

bool CafeSaveFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    FileHandleInner* handle_inner = getFileHandleInner_(handle);
    *pos = handle_inner->mPosition;
    return true;
}

bool CafeSaveFileDevice::doGetFileSize_(u32* size, const SafeString& path)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    FSStat stat;
    SAVEStatus status = SAVEGetStat(client, &block, mAccountSlotNo, path.cstr(), &stat, FS_RET_NO_ERROR);
    if (mLastRawError = status, status != SAVE_STATUS_OK)
        return false;
    *size = stat.size;
    return true;
}

bool CafeSaveFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
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

bool CafeSaveFileDevice::doIsExistFile_(bool* is_exist, const SafeString& path)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    FSStat stat;
    SAVEStatus status = SAVEGetStat(client, &block, mAccountSlotNo, path.cstr(), &stat, (
        FS_RET_NOT_FOUND |
        FS_RET_PERMISSION_ERROR
    ));
    mLastRawError = status;
    switch (status)
    {
    default:
        return false;
    case SAVE_STATUS_NOT_FOUND:
        *is_exist = false;
        return true;
    case SAVE_STATUS_OK:
        *is_exist = (stat.flag & (FS_STAT_FLAG_IS_QUOTA | FS_STAT_FLAG_IS_DIRECTORY)) == 0;
        return true;
    }
}

bool CafeSaveFileDevice::doIsExistDirectory_(bool* is_exist, const SafeString& path)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    FSStat stat;
    SAVEStatus status = SAVEGetStat(client, &block, mAccountSlotNo, path.cstr(), &stat, (
        FS_RET_NOT_FOUND |
        FS_RET_PERMISSION_ERROR
    ));
    mLastRawError = status;
    switch (status)
    {
    default:
        return false;
    case SAVE_STATUS_NOT_FOUND:
        *is_exist = false;
        return true;
    case SAVE_STATUS_OK:
        *is_exist = (stat.flag & (FS_STAT_FLAG_IS_DIRECTORY)) != 0;
        return true;
    }
}

FileDevice* CafeSaveFileDevice::doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    DirHandleInner* handle_inner = getDirHandleInner_(handle);
    SAVEStatus status = SAVEOpenDir(client, &block, mAccountSlotNo, dirname.cstr(), &handle_inner->mHandle, (
        FS_RET_ALREADY_OPEN |
        FS_RET_NOT_FOUND |
        FS_RET_NOT_DIR |
        FS_RET_ACCESS_ERROR |
        FS_RET_PERMISSION_ERROR
    ));
    if (mLastRawError = status, status != SAVE_STATUS_OK)
        return nullptr;
    return this;
}

bool CafeSaveFileDevice::doCloseDirectory_(DirectoryHandle* handle)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    DirHandleInner* handle_inner = getDirHandleInner_(handle);
    FSStatus status = FSCloseDir(client, &block, handle_inner->mHandle, FS_RET_NO_ERROR);
    mLastRawError = status;
    return status == FS_STATUS_OK;
}

bool CafeSaveFileDevice::doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    DirHandleInner* handle_inner = getDirHandleInner_(handle);
    for (u32 i = 0; i < num; i++)
    {
        FSDirEntry dir_entry;
        FSStatus status = FSReadDir(client, &block, handle_inner->mHandle, &dir_entry, FS_RET_NO_ERROR);
        if (mLastRawError = status, status != FS_STATUS_OK)
        {
            if (read_num != nullptr)
                *read_num = i;
            if (status == FS_STATUS_END)
                return true;
            return false;
        }
        entry[i].name = dir_entry.name;
        entry[i].is_directory = (dir_entry.stat.flag & FS_STAT_FLAG_IS_DIRECTORY) != 0;
    }
    if (read_num != nullptr)
        *read_num = num;
    return true;
}

bool CafeSaveFileDevice::doMakeDirectory_(const SafeString& path, u32 permission)
{
    FSCmdBlock block;
    FSInitCmdBlock(&block);
    FSClient* client = getUsableFSClient_();
    SAVEStatus status = SAVEMakeDir(client, &block, mAccountSlotNo, path.cstr(), (
        FS_RET_NOT_FOUND |
        FS_RET_PERMISSION_ERROR |
        FS_RET_STORAGE_FULL |
        FS_RET_JOURNAL_FULL
    ));
    mLastRawError = status;
    return status == SAVE_STATUS_OK;
}

RawErrorCode CafeSaveFileDevice::doGetLastRawError_() const
{
    return mLastRawError;
}

CafeSaveFileDevice::FileHandleInner* CafeSaveFileDevice::getFileHandleInner_(FileHandle* handle)
{
    return reinterpret_cast<FileHandleInner*>(getHandleBaseHandleBuffer_(handle).getBufferPtr());
}

CafeSaveFileDevice::DirHandleInner* CafeSaveFileDevice::getDirHandleInner_(DirectoryHandle* handle)
{
    return reinterpret_cast<DirHandleInner*>(getHandleBaseHandleBuffer_(handle).getBufferPtr());
}

void CafeSaveFileDevice::doResolvePath_(BufferedSafeString* out, const SafeString& path) const
{
    out->copy(path);
}

FSClient* CafeSaveFileDevice::getUsableFSClient_() const
{
    if (mFSClient != nullptr)
        return mFSClient;

    return FileDeviceMgr::instance()->getFSClient_();
}

} // namespace sead
