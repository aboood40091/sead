#ifndef SEAD_PATH_H_
#define SEAD_PATH_H_

#include <prim/seadSafeString.h>

namespace sead {

class Path
{
public:
    static bool getDriveName(BufferedSafeString* dst, const SafeString& src);
    static bool getExt(BufferedSafeString* dst, const SafeString& src);
    static void getPathExceptDrive(BufferedSafeString* dst, const SafeString& src);
    static bool getFileName(BufferedSafeString* dst, const SafeString& src);
    static bool getBaseFileName(BufferedSafeString* dst, const SafeString& src);
    static bool getDirectoryName(BufferedSafeString* dst, const SafeString& src);
    static bool getLangLocPath(BufferedSafeString* dst, const SafeString& src);
};

} // namespace sead

#endif // SEAD_PATH_H_
