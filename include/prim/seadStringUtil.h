#ifndef SEAD_STRING_UTIL_H_
#define SEAD_STRING_UTIL_H_

#include <basis/seadTypes.h>

#include <stdarg.h>
#include <stdio.h>

namespace sead { namespace StringUtil {

inline s32 vsnprintf(char* dst, size_t dst_len, const char* format, va_list varg)
{
    s32 ret = ::vsnprintf(dst, dst_len, format, varg);
    if (ret < 0 || ret >= dst_len)
    {
        dst[dst_len - 1] = '\0';
        ret = dst_len - 1;
    }

    return ret;
}

s32 vsw16printf(char16* dst, size_t dst_len, const char16* format, va_list varg);

inline s32 vsnw16printf(char16* dst, size_t dst_len, const char16* format, va_list varg)
{
    s32 ret = vsw16printf(dst, dst_len, format, varg);
    if (ret < 0 || ret >= dst_len)
    {
        dst[dst_len - 1] = 0;
        ret = dst_len - 1;
    }

    return ret;
}

} } // namespace sead::StringUtil

#endif // SEAD_STRING_UTIL_H_
