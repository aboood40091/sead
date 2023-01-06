#pragma once

#include <cafe/gx2.h>

namespace agl {

inline void
UniformLocation::setUniform(u32 size, const void* buffer) const
{
    if (mVS != -1)
        GX2SetVertexUniformReg(mVS, size / sizeof(u32), buffer);

    if (mFS != -1)
        GX2SetPixelUniformReg(mFS, size / sizeof(u32), buffer);
}

}
