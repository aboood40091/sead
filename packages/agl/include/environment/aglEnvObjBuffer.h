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

    EnvObj* get(s32 type) const
    {
        if (mInfo[type]._2 > 1)
            return *mEnvObj.get(mInfo[type]._0 + 1);

        else
            return nullptr;
    }

    template <typename T>
    T* get() const
    {
        return sead::DynamicCast<T>(get(*T::sTypeInfo));
    }

protected:
    struct Info
    {
        u16 _0;
        u16 _2;
    };
    static_assert(sizeof(Info) == 4);

    u32 _0;
    u32 _4;
    sead::Buffer<Info> mInfo;
    sead::Buffer<EnvObj*> mEnvObj;
    u32 _18[4 / sizeof(u32)];
};
static_assert(sizeof(EnvObjBuffer) == 0x20, "agl::env::EnvObjBuffer size mismatch");

} }
