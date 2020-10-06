#ifndef SEAD_PATH_H_
#define SEAD_PATH_H_

#include <prim/seadSafeString.h>

namespace sead {

class Path
{
public:
    static bool getDriveName(BufferedSafeString* driveName, const SafeString& path);
    static void getPathExceptDrive(BufferedSafeString* pathNoDrive, const SafeString& path);
};

} // namespace sead

#endif // SEAD_PATH_H_
