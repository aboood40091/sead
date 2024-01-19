#pragma once

#include <prim/seadPtrUtil.h>

#include <cafe.h>

#include <cstring>

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
    //SEAD_ASSERT_MSG(!PtrUtil::isInclude(src, dst, PtrUtil::addOffset(dst, size))
    //             && !PtrUtil::isInclude(dst, src, PtrUtil::addOffset(src, size)), "cross copy area");
    return OSBlockMove(dst, src, size, 0);
}

inline s32 MemUtil::compare(const void* buf1, const void* buf2, size_t size)
{
    return std::memcmp(buf1, buf2, size);
}

}  // namespace sead
