#include <filedevice/seadPath.h>
#include <prim/seadSafeString.h>

namespace sead {

bool Path::getDriveName(BufferedSafeString* driveName, const SafeString& path)
{
    //SEAD_ASSERT_MSG(driveName, "destination buffer is null");

    driveName->trim(0);

    s32 index = path.findIndex(":");
    if (index == -1)
        return false;

    driveName->copy(path, index);
    return true;
}

void Path::getPathExceptDrive(BufferedSafeString* pathNoDrive, const SafeString& path)
{
    pathNoDrive->trim(0);

    s32 index = path.findIndex("://");
    if (index != -1)
        pathNoDrive->copyAt(0, path.getPart(index + 3));

    else
        pathNoDrive->copyAt(0, path);
}

} // namespace sead
