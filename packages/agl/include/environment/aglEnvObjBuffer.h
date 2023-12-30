#pragma once

#include <container/seadBuffer.h>

namespace agl { namespace env {

class EnvObj;

class EnvObjBuffer
{
public:
    class AllocateArg;

public:
    EnvObjBuffer();
    virtual ~EnvObjBuffer();

    virtual void allocBuffer(const AllocateArg& arg, sead::Heap* heap);
    virtual void freeBuffer();

    EnvObj* getEnvObj(s32 type, s32 index)
    {
        if (index < mTypeInfo[type].mMaxNum)
            return *mEnvObj.get(mTypeInfo[type].mStartIndex + index);

        else
            return nullptr;
    }

    const EnvObj* getEnvObj(s32 type, s32 index) const
    {
        if (index < mTypeInfo[type].mMaxNum)
            return *mEnvObj.get(mTypeInfo[type].mStartIndex + index);

        else
            return nullptr;
    }

    template <typename T>
    T* getEnvObj(s32 index)
    {
        return sead::DynamicCast<T>(getEnvObj(*T::sTypeInfo, index));
    }

    template <typename T>
    const T* getEnvObj(s32 index) const
    {
        return sead::DynamicCast<T>(getEnvObj(*T::sTypeInfo, index));
    }

protected:
    struct TypeInfo
    {
        u16 mStartIndex;
        u16 mMaxNum;
    };
    static_assert(sizeof(TypeInfo) == 4);

    u32 _0;
    u32 _4;
    sead::Buffer<TypeInfo> mTypeInfo;
    sead::Buffer<EnvObj*> mEnvObj;
    u32 _18[4 / sizeof(u32)];
};
static_assert(sizeof(EnvObjBuffer) == 0x20, "agl::env::EnvObjBuffer size mismatch");

} }
