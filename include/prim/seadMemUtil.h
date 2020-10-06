#ifndef SEAD_MEM_UTIL_H_
#define SEAD_MEM_UTIL_H_

#include <basis/seadTypes.h>

namespace sead
{

class MemUtil
{
public:
    static void* fill(void* ptr, u8 c, size_t size);
    static void* fillZero(void* ptr, size_t size);
    static void* copy(void* dest, const void* src, size_t size);
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
