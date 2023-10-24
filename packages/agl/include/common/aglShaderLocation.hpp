#pragma once

// TODO: Move to the proper header
#define SEAD_MACRO_UTIL_ROUNDUP(x, y) ((x) + ((y) - 1) & ~((y) - 1))

#include <cafe/gx2.h>

namespace agl {

inline void
UniformLocation::setUniform(u32 size, const void* buffer) const
{
    const u32 cVec4Size = 4 * sizeof(u32);

    if (mVS != -1)
        GX2SetVertexUniformReg(mVS, SEAD_MACRO_UTIL_ROUNDUP(size, cVec4Size) / sizeof(u32), buffer);

    if (mFS != -1)
        GX2SetPixelUniformReg(mFS, SEAD_MACRO_UTIL_ROUNDUP(size, cVec4Size) / sizeof(u32), buffer);
}

}
