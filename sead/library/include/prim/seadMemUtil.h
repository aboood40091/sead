#ifndef SEAD_MEM_UTIL_H_
#define SEAD_MEM_UTIL_H_

#include <basis/seadTypes.h>

namespace sead
{

class MemUtil
{
public:
    static void* fill(void*, u8, size_t size);
    static void* fillZero(void*, size_t size);
    static void* copyOverlap(void*, const void*, size_t);
    static void* copy(void*, const void*, size_t);
    static void* copyAlign32(void*, const void*, size_t);
    static s32 compare(const void*, const void*, size_t);
    static bool isStack(const void*);
    static bool isHeap(const void*);
    static void dumpMemoryBinary(const void*, const u32, const u32, const bool);
};

}  // namespace sead

#ifdef __cplusplus

#ifdef cafe
#include <prim/cafe/seadMemUtilCafe.hpp>
#else
#error "Unknown platform"
#endif // cafe

#endif // __cplusplus

#endif // SEAD_MEM_UTIL_H_
