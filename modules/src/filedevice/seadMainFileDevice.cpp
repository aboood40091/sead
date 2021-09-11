#ifdef cafe
#include <filedevice/cafe/seadCafeFSAFileDeviceCafe.h>
#endif // cafe

#include <filedevice/seadMainFileDevice.h>
#include <prim/seadSafeString.h>

namespace sead {

MainFileDevice::MainFileDevice(Heap* heap)
    : FileDevice("main")
    , mFileDevice(NULL)
{
#ifdef cafe
    mFileDevice = new(heap, 4) CafeContentFileDevice();
#else
    #error "Unknown platform"
#endif // cafe

    //SEAD_ASSERT(mFileDevice);
}

MainFileDevice::~MainFileDevice()
{
    if (mFileDevice != NULL)
    {
        delete mFileDevice;
        mFileDevice = NULL;
    }
}

void
MainFileDevice::traceFilePath(
    const SafeString& path
) const
{
    mFileDevice->traceFilePath(path);
}

void
MainFileDevice::traceDirectoryPath(
    const SafeString& path
) const
{
    mFileDevice->traceDirectoryPath(path);
}

void
MainFileDevice::resolveFilePath(
    BufferedSafeString* out, const SafeString& path
) const
{
    mFileDevice->resolveFilePath(out, path);
}

void
MainFileDevice::resolveDirectoryPath(
    BufferedSafeString* out, const SafeString& path
) const
{
    mFileDevice->resolveDirectoryPath(out, path);
}

} // namespace sead
