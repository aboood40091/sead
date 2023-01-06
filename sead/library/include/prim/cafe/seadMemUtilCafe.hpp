#pragma once

#include <cafe.h>

namespace sead {

inline void* MemUtil::fill(void* addr, u8 val, size_t size)
{
    return OSBlockSet(addr, val, size);
}

inline void* MemUtil::fillZero(void* addr, size_t size)
{
    return OSBlockSet(addr, 0, size);
}

inline void* MemUtil::copy(void* dst, const void* src, size_t size)
{
    //SEAD_ASSERT_MSG(false, (src < dst || src >= (u8*)dst + size)
    //                    && (dst < src || dst >= (u8*)src + size), "cross copy area");
    return OSBlockMove(dst, src, size, 0);
}

}  // namespace sead
