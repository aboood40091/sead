#pragma once

// TODO: Move to the proper header
#define SEAD_MACRO_UTIL_ROUNDUP(x, y) ((x) + ((y) - 1) & ~((y) - 1))

#include <cafe/gx2.h>

namespace agl {

namespace {

inline void
setUniform(const UniformLocation& location, u32 size, const void* buffer)
{
    if (location.getVertexLocation() != -1)
        GX2SetVertexUniformReg(location.getVertexLocation(), SEAD_MACRO_UTIL_ROUNDUP(size, sizeof(sead::Vector4u)) / sizeof(u32), buffer);

    if (location.getFragmentLocation() != -1)
        GX2SetPixelUniformReg(location.getFragmentLocation(), SEAD_MACRO_UTIL_ROUNDUP(size, sizeof(sead::Vector4u)) / sizeof(u32), buffer);
}

}

inline void
UniformLocation::setBool(bool value) const
{
    setInt(value);
}

inline void
UniformLocation::setInt(s32 value) const
{
    setUniform(*this, sizeof(s32), &value);
}

inline void
UniformLocation::setUInt(u32 value) const
{
    setUniform(*this, sizeof(u32), &value);
}

inline void
UniformLocation::setFloat(f32 value) const
{
    setUniform(*this, sizeof(f32), &value);
}

inline void
UniformLocation::setIVec2(const sead::Vector2i& value) const
{
    setUniform(*this, sizeof(sead::Vector2i), &value);
}

inline void
UniformLocation::setUVec2(const sead::Vector2u& value) const
{
    setUniform(*this, sizeof(sead::Vector2u), &value);
}

inline void
UniformLocation::setVec2(const sead::Vector2f& value) const
{
    setUniform(*this, sizeof(sead::Vector2f), &value);
}

inline void
UniformLocation::setIVec3(const sead::Vector3i& value) const
{
    setUniform(*this, sizeof(sead::Vector3i), &value);
}

inline void
UniformLocation::setUVec3(const sead::Vector3u& value) const
{
    setUniform(*this, sizeof(sead::Vector3u), &value);
}

inline void
UniformLocation::setVec3(const sead::Vector3f& value) const
{
    setUniform(*this, sizeof(sead::Vector3f), &value);
}

inline void
UniformLocation::setIVec4(const sead::Vector4i& value) const
{
    setUniform(*this, sizeof(sead::Vector4i), &value);
}

inline void
UniformLocation::setUVec4(const sead::Vector4u& value) const
{
    setUniform(*this, sizeof(sead::Vector4u), &value);
}

inline void
UniformLocation::setVec4(const sead::Vector4f& value) const
{
    setUniform(*this, sizeof(sead::Vector4f), &value);
}

inline void
UniformLocation::setVec4(const sead::Color4f& value) const
{
    setUniform(*this, sizeof(sead::Color4f), &value);
}

inline void
UniformLocation::setVec4Array(const sead::Matrix34f& value) const
{
    setUniform(*this, sizeof(sead::Matrix34f), &value);
}

inline void
UniformLocation::setVec4Array(const sead::Matrix44f& value) const
{
    setUniform(*this, sizeof(sead::Matrix44f), &value);
}

inline void
UniformLocation::setMtx43(const f32* value) const
{
    setUniform(*this, sizeof(sead::Matrix34f), value);
}

inline void
UniformLocation::setMtx44(const f32* value) const
{
    setUniform(*this, sizeof(sead::Matrix44f), value);
}

}
