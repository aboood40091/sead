#include <prim/seadStringUtil.h>

#include <stdarg.h>
#include <cwchar>

namespace sead { namespace StringUtil {

s32 vsw16printf(char16* dst, size_t dst_len, const char16* format, va_list varg)
{
    if (dst_len == 0)
        return -1;

    // Nintendo actually made their own implementation of vswprintf
    // Will use std::vswprintf for now

    // TODO: actually implement this

    return std::vswprintf(dst, dst_len, format, varg);
}

} } // namespace sead::StringUtil
