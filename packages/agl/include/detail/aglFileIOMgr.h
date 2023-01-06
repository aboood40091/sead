#pragma once

#include <container/seadBuffer.h>
#include <filedevice/seadFileDevice.h>
#include <heap/seadDisposer.h>
#include <hostio/seadHostIODefine.h>
#include <prim/seadSafeString.h>
#include <thread/seadCriticalSection.h>

namespace agl { namespace detail {

class FileIOMgr
{
    SEAD_SINGLETON_DISPOSER(FileIOMgr)

public:
    struct DialogArg
    {
        DialogArg();

        sead::SafeString mDefaultFilter;
        sead::SafeString mDefaultFilename;
        sead::SafeString mDefaultID;
        sead::SafeString mPath;
        sead::BufferedSafeString* mOutPath;
        s32 mLoadAlignment;
    };
    static_assert(sizeof(DialogArg) == 0x28, "agl::detail::FileIOMgr::DialogArg size mismatch");

    struct File // No idea what the actual name is
    {
        u8* mpData;
        u32 mSize;
    };
    static_assert(sizeof(File) == 8, "agl::detail::FileIOMgr::File size mismatch");

public:
    FileIOMgr();
    virtual ~FileIOMgr();

    bool showDialog(sead::hostio::FileInfo* p_info, const sead::SafeString& mode, const sead::SafeString& id, const sead::SafeString& filter, const sead::SafeString& filename) const;

    bool save(const void* p_buf, u32 size, const DialogArg& arg);

    s32 load(const DialogArg& arg);
    void close(s32 index);

    s32 getFileNum() const
    {
        return mBuffer.size();
    }

    const File& getFile(s32 index) const
    {
        return mBuffer[index];
    }

private:
    sead::FileDevice* mFileDevice; // CafeHostIOFileDevice*
    sead::Buffer<File> mBuffer;
    sead::CriticalSection mCriticalSection;
};
static_assert(sizeof(FileIOMgr) == 0x5C, "agl::detail::FileIOMgr size mismatch");

} }
