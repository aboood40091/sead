#ifndef SEAD_FILEDEVICEMGR_H_
#define SEAD_FILEDEVICEMGR_H_

#ifdef cafe
#include <cafe.h>
#endif // cafe

#include <container/seadTList.h>
#include <filedevice/seadFileDevice.h>
#include <filedevice/seadMainFileDevice.h>
#include <heap/seadDisposer.h>
#include <prim/seadSafeString.h>

namespace sead {

class FileDeviceMgr
{
    SEAD_SINGLETON_DISPOSER(FileDeviceMgr)

public:
    static const s32 cNoDrivePathBufferSize = 256;

private:
    typedef TList<FileDevice*> DeviceList;

public:
    FileDeviceMgr();
    ~FileDeviceMgr();

    FileDevice* open(FileHandle* handle, const SafeString& filename, FileDevice::FileOpenFlag flag, u32 div_num)
    {
        FileDevice* device = tryOpen(handle, filename, flag, div_num);
        //SEAD_ASSERT_MSG(device != nullptr, "open failed. [%s]", filename.cstr());
        return device;
    }

    FileDevice* tryOpen(FileHandle* handle, const SafeString& filename, FileDevice::FileOpenFlag flag, u32 div_num);

    FileDevice* findDeviceFromPath(const SafeString& path, BufferedSafeString* no_drive_path) const;

    FileDevice* openDirectory(DirectoryHandle* handle, const SafeString& dirname)
    {
        FileDevice* device = tryOpenDirectory(handle, dirname);
        //SEAD_ASSERT_MSG(device != nullptr, "open directory failed. [%s]", dirname.cstr());
        return device;
    }

    FileDevice* tryOpenDirectory(DirectoryHandle* handle, const SafeString& dirname);

    u8* load(FileDevice::LoadArg& arg)
    {
        u8* ret = tryLoad(arg);
        //SEAD_ASSERT_MSG(ret != nullptr, "load failed. [%s]", arg.path.cstr());
        return ret;
    }

    u8* tryLoad(FileDevice::LoadArg& arg);

    void unload(u8* data)
    {
        //SEAD_ASSERT(data);
        delete data;
    }

    void mount(FileDevice* device, const SafeString& drive_name = SafeString::cEmptyString);
    void unmount(const SafeString& drive);
    void unmount(FileDevice* device);

    FileDevice* setDefaultFileDevice(FileDevice* device)
    {
        return mDefaultFileDevice = device;
    }

    FileDevice* getDefaultFileDevice() const
    {
        return mDefaultFileDevice;
    }

    MainFileDevice* getMainFileDevice() const
    {
        return mMainFileDevice;
    }

    FileDevice* findDevice(const SafeString& drive) const;

    virtual void traceFilePath(const SafeString& path) const;
    virtual void traceDirectoryPath(const SafeString& path) const;
    virtual void resolveFilePath(BufferedSafeString* out, const SafeString& path) const;
    virtual void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const;

protected:
    DeviceList mDeviceList;
    FileDevice* mDefaultFileDevice;
    MainFileDevice* mMainFileDevice;

#ifdef cafe
public:
    FSClient* getFSClient_() const { return const_cast<FSClient*>(&mFSClient); }

    void setSDCardMountPath(const SafeString&);
    void setHostFileIOMountPath(const SafeString&);

    RawErrorCode mountSDCard();
    RawErrorCode unmountSDCard();

protected:
    bool mountHostFileIO();
    bool unmountHostFileIO();

    static void stateChangeCallback_(FSClient* client, FSVolumeState state, void* context);

protected:
    FSClient mFSClient;
    char mFSHFIOMountPath[FS_MAX_MOUNTPATH_SIZE];
    char mFSSDMountPath[FS_MAX_MOUNTPATH_SIZE];
    u32 mFSSDMountCount;
#else
    #error "Unknown platform"
#endif // cafe
};
#ifdef cafe
static_assert(sizeof(FileDeviceMgr) == 0x182C, "sead::FileDeviceMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_FILEDEVICEMGR_H_
