#pragma once

#include <cafe.h>

namespace sead {

inline void* MemUtil::fill(void* ptr, u8 c, size_t size)
{
    return OSBlockSet(ptr, c, size);
}

inline void* MemUtil::fillZero(void* ptr, size_t size)
{
    return OSBlockSet(ptr, 0, size);
}

inline void* MemUtil::copy(void* dest, const void* src, size_t size)
{
    return OSBlockMove(dest, src, size, 0);
}

}  // namespace sead
