#pragma once

#include <common/aglShaderLocation.h>
#include <heap/seadHeap.h>
#include <math/seadVector.h>

namespace agl {

class UniformBlock
{
    struct Member
    {
        u8 mType;
        u16 mNum;
        u16 mOffset;
    };
    static_assert(sizeof(Member) == 6, "agl::UniformBlock::Member size mismatch");

    struct Header
    {
        Member* mpMember;
        u16 mMemberNum;
        u16 mMemberCount;
    };
    static_assert(sizeof(Header) == 8, "agl::UniformBlock::Header size mismatch");

public:
    static const u32 cUniformBlockAlignment = 0x100;
    static const u32 cCPUCacheLineSize = 0x20;

public:
    enum Type
    {
        cType_bool  = 0,
        cType_int   = 1,
        cType_float = 2,
        cType_vec2  = 3,
        cType_vec3  = 4,
        cType_vec4  = 5,

        // "not implemented yet."
      //cType_      = 6,
      //cType_      = 7,
      //cType_      = 8,
      //cType_      = 9,
      //cType_      = 10,
      //cType_      = 11,

      //cType_      = 12, // mat4?
      //cType_      = 13, // ^^
      //cType_      = 14, // ^^

        cType_Num   = 15
    };

public:
    UniformBlock();
    virtual ~UniformBlock();

    void startDeclare(s32 num, sead::Heap* heap);
    void declare(Type type, s32 num);
    void declare(const UniformBlock& block);
    void create(sead::Heap* heap);
    void destroy();
    void dcbz() const;
    void flush(void* p_memory, bool invalidate_gpu) const;
    void flush(bool invalidate_gpu) const { flush(mCurrentBuffer, invalidate_gpu); }
    void flushNoSync(void* p_memory, bool invalidate_gpu) const;
    void flushNoSync(bool invalidate_gpu) const { flushNoSync(mCurrentBuffer, invalidate_gpu); }

    bool setUniform(const void* p_data, const UniformBlockLocation& location, u32 offset, size_t size) const;
    bool setUniform(const UniformBlockLocation& location) const;

    void setBool(void* p_memory, s32 index, bool data, s32 array_index = 0) const;
    void setBool(s32 index, bool data, s32 array_index = 0) const;
    void setBool(void* p_memory, s32 index, const bool* p_data, s32 array_num, s32 array_index = 0) const;
    void setBool(s32 index, const bool* p_data, s32 array_num, s32 array_index = 0) const;

    void setInt(void* p_memory, s32 index, s32 data, s32 array_index = 0) const;
    void setInt(s32 index, s32 data, s32 array_index = 0) const;
    void setInt(void* p_memory, s32 index, const s32* p_data, s32 array_num, s32 array_index = 0) const;
    void setInt(s32 index, const s32* p_data, s32 array_num, s32 array_index = 0) const;

    void setFloat(void* p_memory, s32 index, f32 data, s32 array_index = 0) const;
    void setFloat(s32 index, f32 data, s32 array_index = 0) const;
    void setFloat(void* p_memory, s32 index, const f32* p_data, s32 array_num, s32 array_index = 0) const;
    void setFloat(s32 index, const f32* p_data, s32 array_num, s32 array_index = 0) const;

    void setVector2f(void* p_memory, s32 index, const sead::Vector2f& data, s32 array_index = 0) const;
    void setVector2f(s32 index, const sead::Vector2f& data, s32 array_index = 0) const;
    void setVector2f(void* p_memory, s32 index, const sead::Vector2f* p_data, s32 array_num, s32 array_index = 0) const;
    void setVector2f(s32 index, const sead::Vector2f* p_data, s32 array_num, s32 array_index = 0) const;

    void setVector3f(void* p_memory, s32 index, const sead::Vector3f& data, s32 array_index = 0) const;
    void setVector3f(s32 index, const sead::Vector3f& data, s32 array_index = 0) const;
    void setVector3f(void* p_memory, s32 index, const sead::Vector3f* p_data, s32 array_num, s32 array_index = 0) const;
    void setVector3f(s32 index, const sead::Vector3f* p_data, s32 array_num, s32 array_index = 0) const;

    void setVector4f(void* p_memory, s32 index, const sead::Vector4f& data, s32 array_index = 0) const;
    void setVector4f(s32 index, const sead::Vector4f& data, s32 array_index = 0) const;
    void setVector4f(void* p_memory, s32 index, const sead::Vector4f* p_data, s32 array_num, s32 array_index = 0) const;
    void setVector4f(s32 index, const sead::Vector4f* p_data, s32 array_num, s32 array_index = 0) const;

private:
    void setData_(void* p_memory, s32 index, const void* p_data, s32 array_index, s32 array_num) const;

private:
    enum Flags
    {
        cFlag_OwnHeader = 1 << 0,
        cFlag_OwnBuffer = 1 << 1
    };

    Header* mpHeader;
    u8* mCurrentBuffer;
    u32 mBlockSize;
    sead::BitFlag8 mFlag;
};
static_assert(sizeof(UniformBlock) == 0x14, "agl::UniformBlock size mismatch");

}

#ifdef __cplusplus

#include <common/aglUniformBlock.hpp>

#endif // __cplusplus
