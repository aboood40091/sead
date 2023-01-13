#include <prim/seadSafeString.h>
#include <prim/seadStringUtil.h>

#include <cstdio>

namespace {

static const char16 cEmptyStringChar16[1] = L"";

} // namespace {no-name}

namespace sead {

template <>
const char SafeStringBase<char>::cNullChar = '\0';

template <>
const char SafeStringBase<char>::cLineBreakChar = '\n';

template <>
const SafeStringBase<char> SafeStringBase<char>::cEmptyString("");

template <>
const char16 SafeStringBase<char16>::cNullChar = 0;

template <>
const char16 SafeStringBase<char16>::cLineBreakChar = static_cast<char16>('\n');

template <>
const SafeStringBase<char16> SafeStringBase<char16>::cEmptyString(cEmptyStringChar16);

template <>
s32 BufferedSafeStringBase<char>::formatImpl_(char* dst, s32 dst_size, const char* format_string, va_list varg)
{
    s32 ret = std::vsnprintf(dst, dst_size, format_string, varg);
    if (ret < 0 || ret >= dst_size)
    {
        dst[dst_size - 1] = '\0';
        ret = dst_size - 1;
    }
    return ret;
}

template <>
s32 BufferedSafeStringBase<char16>::formatImpl_(char16* dst, s32 dst_size, const char16* format_string, va_list varg)
{
    s32 ret = StringUtil::vsw16printf(dst, dst_size, format_string, varg);
    if (ret < 0 || ret >= dst_size)
    {
        dst[dst_size - 1] = L'\0';
        ret = dst_size - 1;
    }
    return ret;
}

template <>
s32 BufferedSafeStringBase<char>::formatV(const char* format_string, va_list varg)
{
    char* mutableString = getMutableStringTop_();
    return formatImpl_(mutableString, mBufferSize, format_string, varg);
}

template <>
s32 BufferedSafeStringBase<char16>::formatV(const char16* format_string, va_list varg)
{
    char16* mutableString = getMutableStringTop_();
    return formatImpl_(mutableString, mBufferSize, format_string, varg);
}

template <>
s32 BufferedSafeStringBase<char>::format(const char* format_string, ...)
{
    va_list va;
    va_start(va, format_string);
    s32 ret = formatV(format_string, va);
    va_end(va);

    return ret;
}

template <>
s32 BufferedSafeStringBase<char16>::format(const char16* format_string, ...)
{
    va_list va;
    va_start(va, format_string);
    s32 ret = formatV(format_string, va);
    va_end(va);

    return ret;
}

} // namespace sead
