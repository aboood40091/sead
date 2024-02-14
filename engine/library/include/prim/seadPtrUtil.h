#ifndef SEAD_PTR_UTIL_H_
#define SEAD_PTR_UTIL_H_

#include <basis/seadTypes.h>

namespace sead {

class PtrUtil
{
public:
    static void* roundUpPow2(const void* ptr, u32 alignment)
    {
        return (void*)(uintptr(ptr) + (alignment - 1) & ~(alignment - 1));
    }
    static void* roundUpN(const void*, u32);
    static void* roundDownPow2(const void*, u32);
    static void* roundDownN(const void*, u32);

    static void* addOffset(const void* ptr, d_sintptr offset) { return (u8*)ptr + offset; }
    static d_sintptr diff(const void* a, const void* b) { return (d_sintptr)a - (d_sintptr)b; }

    static bool isInclude(const void* ptr, const void* begin, const void* end) { return begin <= ptr && ptr < end; }

    static bool isAligned(const void* ptr, s32 alignment)
    {
      //SEAD_ASSERT(alignment != 0);
        return uintptr(ptr) % alignment == 0;
    }

    static bool isAlignedPow2(const void* ptr, u32 alignment);
    static bool isAlignedN(const void*, s32);
};

} // namespace sead

#endif // SEAD_PTR_UTIL_H_
