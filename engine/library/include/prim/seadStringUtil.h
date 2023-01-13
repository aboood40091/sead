#ifndef SEAD_STRING_UTIL_H_
#define SEAD_STRING_UTIL_H_

#include <basis/seadTypes.h>

#include <cstdarg>

namespace sead { namespace StringUtil {

s32 vsw16printf(char16* dst, size_t dst_len, const char16* format, va_list varg);

} } // namespace sead::StringUtil

#endif // SEAD_STRING_UTIL_H_
