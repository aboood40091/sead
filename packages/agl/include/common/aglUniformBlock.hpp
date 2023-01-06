#pragma once

namespace agl {

// There are some asserts which I won't bother with (for now)

inline bool UniformBlock::setUniform(const UniformBlockLocation& location) const
{
    return setUniform(mCurrentBuffer, location, 0, mBlockSize);
}

inline void UniformBlock::setBool(void* p_memory, s32 index, bool data, s32 array_index) const
{
    setInt(p_memory, index, data, array_index);
}

inline void UniformBlock::setBool(s32 index, bool data, s32 array_index) const
{
    setInt(index, data, array_index);
}

inline void UniformBlock::setBool(void* p_memory, s32 index, const bool* p_data, s32 array_num, s32 array_index) const
{
    for (s32 i = 0; i < array_num; i++)
        setInt(p_memory, index, p_data[i], array_index + i);
}

inline void UniformBlock::setBool(s32 index, const bool* p_data, s32 array_num, s32 array_index) const
{
    for (s32 i = 0; i < array_num; i++)
        setInt(index, p_data[i], array_index + i);
}

inline void UniformBlock::setInt(void* p_memory, s32 index, s32 data, s32 array_index) const
{
    setData_(p_memory, index, &data, array_index, 1);
}

inline void UniformBlock::setInt(s32 index, s32 data, s32 array_index) const
{
    setData_(mCurrentBuffer, index, &data, array_index, 1);
}

inline void UniformBlock::setInt(void* p_memory, s32 index, const s32* p_data, s32 array_num, s32 array_index) const
{
    setData_(p_memory, index, p_data, array_index, array_num);
}

inline void UniformBlock::setInt(s32 index, const s32* p_data, s32 array_num, s32 array_index) const
{
    setData_(mCurrentBuffer, index, p_data, array_index, array_num);
}

inline void UniformBlock::setFloat(void* p_memory, s32 index, f32 data, s32 array_index) const
{
    setData_(p_memory, index, &data, array_index, 1);
}

inline void UniformBlock::setFloat(s32 index, f32 data, s32 array_index) const
{
    setData_(mCurrentBuffer, index, &data, array_index, 1);
}

inline void UniformBlock::setFloat(void* p_memory, s32 index, const f32* p_data, s32 array_num, s32 array_index) const
{
    setData_(p_memory, index, p_data, array_index, array_num);
}

inline void UniformBlock::setFloat(s32 index, const f32* p_data, s32 array_num, s32 array_index) const
{
    setData_(mCurrentBuffer, index, p_data, array_index, array_num);
}

inline void UniformBlock::setVector2f(void* p_memory, s32 index, const sead::Vector2f& data, s32 array_index) const
{
    setData_(p_memory, index, &data, array_index, 1);
}

inline void UniformBlock::setVector2f(s32 index, const sead::Vector2f& data, s32 array_index) const
{
    setData_(mCurrentBuffer, index, &data, array_index, 1);
}

inline void UniformBlock::setVector2f(void* p_memory, s32 index, const sead::Vector2f* p_data, s32 array_num, s32 array_index) const
{
    setData_(p_memory, index, p_data, array_index, array_num);
}

inline void UniformBlock::setVector2f(s32 index, const sead::Vector2f* p_data, s32 array_num, s32 array_index) const
{
    setData_(mCurrentBuffer, index, p_data, array_index, array_num);
}

inline void UniformBlock::setVector3f(void* p_memory, s32 index, const sead::Vector3f& data, s32 array_index) const
{
    setData_(p_memory, index, &data, array_index, 1);
}

inline void UniformBlock::setVector3f(s32 index, const sead::Vector3f& data, s32 array_index) const
{
    setData_(mCurrentBuffer, index, &data, array_index, 1);
}

inline void UniformBlock::setVector3f(void* p_memory, s32 index, const sead::Vector3f* p_data, s32 array_num, s32 array_index) const
{
    setData_(p_memory, index, p_data, array_index, array_num);
}

inline void UniformBlock::setVector3f(s32 index, const sead::Vector3f* p_data, s32 array_num, s32 array_index) const
{
    setData_(mCurrentBuffer, index, p_data, array_index, array_num);
}

inline void UniformBlock::setVector4f(void* p_memory, s32 index, const sead::Vector4f& data, s32 array_index) const
{
    setData_(p_memory, index, &data, array_index, 1);
}

inline void UniformBlock::setVector4f(s32 index, const sead::Vector4f& data, s32 array_index) const
{
    setData_(mCurrentBuffer, index, &data, array_index, 1);
}

inline void UniformBlock::setVector4f(void* p_memory, s32 index, const sead::Vector4f* p_data, s32 array_num, s32 array_index) const
{
    setData_(p_memory, index, p_data, array_index, array_num);
}

inline void UniformBlock::setVector4f(s32 index, const sead::Vector4f* p_data, s32 array_num, s32 array_index) const
{
    setData_(mCurrentBuffer, index, p_data, array_index, array_num);
}

}
