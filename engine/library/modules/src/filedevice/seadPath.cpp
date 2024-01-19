#include <filedevice/seadPath.h>
#include <prim/seadSafeString.h>

namespace sead {

bool Path::getDriveName(BufferedSafeString* dst, const SafeString& src)
{
    //SEAD_ASSERT_MSG(dst, "destination buffer is null");

    dst->trim(0);

    s32 index = src.findIndex(":");
    if (index == -1)
        return false;

    dst->copy(src, index);
    return true;
}

void Path::getPathExceptDrive(BufferedSafeString* dst, const SafeString& src)
{
    //SEAD_ASSERT_MSG(dst, "destination buffer is null");

    dst->trim(0);

    s32 index = src.findIndex("://");
    if (index == -1)
        dst->copyAt(0, src);
    else
        dst->copyAt(0, src.getPart(index + 3));
}

} // namespace sead
