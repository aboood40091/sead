#ifndef SEAD_PTR_UTIL_H_
#define SEAD_PTR_UTIL_H_

#include <basis/seadTypes.h>

namespace sead {

class PtrUtil
{
public:
    static void* roundUpPow2(const void*, u32);
    static void* roundUpN(const void*, u32);
    static void* roundDownPow2(const void*, u32);
    static void* roundDownN(const void*, u32);
    static void* addOffset(const void* ptr, d_sintptr offset) { return (u8*)ptr + offset; }
    static d_sintptr diff(const void* a, const void* b) { return d_sintptr((u8*)a - (u8*)b); }
    static bool isInclude(const void*, const void*, const void*);
    static bool isAligned(const void*, s32);
    static bool isAlignedPow2(const void*, u32);
    static bool isAlignedN(const void*, s32);
};

} // namespace sead

#endif // SEAD_PTR_UTIL_H_
