#include <detail/aglFileIOMgr.h>
#include <detail/aglPrivateResource.h>
#include <prim/seadMemUtil.h>

namespace agl { namespace detail {

FileIOMgr* FileIOMgr::sInstance = nullptr;

bool FileIOMgr::showDialog(sead::hostio::FileInfo* p_info, const sead::SafeString& mode, const sead::SafeString& id, const sead::SafeString& filter, const sead::SafeString& filename) const
{
    // SEAD_ASSERT(p_info != nullptr);

    sead::SafeString id_ = (!id.isEqual("")) // Why not use id.isEmpty() ??
                                    ? id
                                    : filter;

    sead::FormatFixedSafeString<1024> arg(
        "FileName = %s, Mode = %s, Id = %s, Filter = %sファイル(*.%s)|*.%s|すべてのファイル(*.*)|*.*",
        filename.cstr(),
        mode.cstr(),
        id_.cstr(),
        filter.cstr(),
        filter.cstr(),
        filter.cstr()
    );

    // Stripped in release build:
    // return sead::hostio::ShowFileDialog(p_info, arg, "");

    return false;
}

bool FileIOMgr::save(const void* p_buf, u32 size, const DialogArg& arg)
{
    bool ret = false;

    // TODO: scoped lock
    mCriticalSection.lock();
    do
    {
        sead::hostio::FileInfo file_info;

        bool use_dialog;

        if (arg.mPath.isEmpty())
        {
            if (!showDialog(&file_info, "Save", arg.mDefaultID, arg.mDefaultFilter, arg.mDefaultFilename))
                break;

            use_dialog = true;
        }
        else
        {
            file_info.path = arg.mPath;
            use_dialog = false;
        }

        sead::FileHandle handle;

        sead::FileDevice* device = mFileDevice->tryOpen(&handle, file_info.path, sead::FileDevice::cFileOpenFlag_WriteOnly, 0);
        if (device)
        {
            u8* temp_buf = new (PrivateResource::instance()->getDebugHeap(), sead::FileDevice::cBufferMinAlignment) u8[size];
            sead::MemUtil::copy(temp_buf, p_buf, size);

            handle.write(temp_buf, size);
            device->close(&handle);

            delete temp_buf; // Nintendo did not use delete[] (fixed in later versions)

            if (arg.mOutPath)
                arg.mOutPath->copy(file_info.path);

            ret = true;
        }
        else
        {
            if (use_dialog)
            {
                // Stripped in release build:
                // sead::hostio::ShowMessageBox(
                //     sead::FormatFixedSafeString<1024>(
                //         "%sの保存に失敗しました。\n保存先のアクセス権を確認して下さい。",
                //         file_info.path.cstr()
                //     ),
                //     "Caption=ファイル保存失敗, Icon=Error",
                //     ""
                // );
            }
        }
        break;
    }
    while (false);
    mCriticalSection.unlock();

    return ret;
}

s32 FileIOMgr::load(const DialogArg& arg)
{
    s32 ret = -1;

    // TODO: scoped lock
    mCriticalSection.lock();
    do
    {
        s32 index = -1;
        File* pBinary = nullptr;

        for (sead::Buffer<File>::iterator it = mBuffer.begin(), it_end = mBuffer.end(); it != it_end; ++it)
        {
            if (it->mpData == nullptr)
            {
                index = it.getIndex();
                pBinary = &(*it);
                break;
            }
        }
        // SEAD_ASSERT(0 <= index);

        sead::hostio::FileInfo file_info;

        if (arg.mPath.isEmpty())
        {
            if (!showDialog(&file_info, "Open", arg.mDefaultID, arg.mDefaultFilter, arg.mDefaultFilename))
                break;
        }
        else
        {
            file_info.path = arg.mPath;
        }

        sead::FileDevice::LoadArg load_arg;
        load_arg.path = file_info.path;
        load_arg.heap = PrivateResource::instance()->getDebugHeap();
        load_arg.alignment = arg.mLoadAlignment;

        // SEAD_ASSERT(pBinary->mpData == nullptr);
        pBinary->mpData = mFileDevice->tryLoad(load_arg);
        if (pBinary->mpData)
        {
            pBinary->mSize = load_arg.read_size;

            if (arg.mOutPath)
                arg.mOutPath->copy(file_info.path);

            ret = index;
        }
        break;
    }
    while (false);
    mCriticalSection.unlock();

    return ret;
}

void FileIOMgr::close(s32 index)
{
    // TODO: scoped lock
    mCriticalSection.lock();
    {
        if (mBuffer[index].mpData)
        {
            delete[] mBuffer[index].mpData;
            mBuffer[index].mpData = nullptr;
            mBuffer[index].mSize = 0;
        }
    }
    mCriticalSection.unlock();
}

FileIOMgr::DialogArg::DialogArg()
    : mDefaultFilter()
    , mDefaultFilename()
    , mDefaultID("agl_default")
    , mPath()
    , mOutPath(nullptr)
    , mLoadAlignment(0x20) // Why not sead::FileDevice::cBufferMinAlignment is beyond me
{
}

} }
