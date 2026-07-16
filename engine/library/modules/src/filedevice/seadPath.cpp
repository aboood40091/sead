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

bool Path::getExt(BufferedSafeString* dst, const SafeString& src)
{
    //SEAD_ASSERT_MSG(dst, "destination buffer is null");

    dst->trim(0);

    s32 index = src.rfindIndex(".");
    if (index < 0)
        return false;

    if (src.getPart(index).include('/'))
        return false;

    dst->copy(src.getPart(index + 1));
    return true;
}

bool Path::getFileName(BufferedSafeString* dst, const SafeString& src)
{
    //SEAD_ASSERT_MSG(dst, "destination buffer is null");

    dst->trim(0);

    s32 index = src.rfindIndex("/");

    dst->copy(src.getPart(index + 1));
    return true;
}

bool Path::getDirectoryName(BufferedSafeString* dst, const SafeString& src)
{
    //SEAD_ASSERT_MSG(dst, "destination buffer is null");

    if (dst != &src)
    {
        dst->trim(0);

        s32 index = src.rfindIndex("/");
        if (index <= 0)
            return false;

        dst->copy(src, index);
        return true;
    }
    else
    {
        s32 index = src.rfindIndex("/");
        if (index <= 0)
            return false;

        dst->trim(index);
        return true;
    }
}

bool Path::getLangLocPath(BufferedSafeString* dst, const SafeString& src)
{
    //SEAD_ASSERT_MSG(false, "todo implement");
    (void)dst;
    (void)src;
    return false;
}

} // namespace sead
