#pragma once

#include <container/seadBuffer.h>
#include <container/seadSafeArray.h>
#include <environment/aglEnvObj.h>

namespace agl { namespace env {

class EnvObjBuffer
{
public:
    class AllocateArg
    {
    public:
        AllocateArg();

        virtual ~AllocateArg()
        {
        }

        void setContainMax(s32 type, s32 max);

        template <typename T>
        void setContainMax(s32 max)
        {
            setContainMax(T::getType(), max);
        }

        s32 getContainMax(s32 type) const
        {
            return mTypeMax[type];
        }

        template <typename T>
        s32 getContainMax() const
        {
            return getContainMax(T::getType());
        }

        s32 getContainTotal() const
        {
            return mTotal;
        }

    protected:
        sead::SafeArray<s32, EnvObj::cTypeMax> mTypeMax;
        s32 mTotal;
    };
    static_assert(sizeof(AllocateArg) == 0x108);

public:
    EnvObjBuffer();
    virtual ~EnvObjBuffer();

    virtual void allocBuffer(const AllocateArg& arg, sead::Heap* heap);
    virtual void freeBuffer();

    s32 getContainTotal() const
    {
        return mEnvObjPtrBuffer.size();
    }

    void sort(s32 type);

    s32 searchBufferIndex(s32 type, const sead::SafeString& name) const;

    template <typename T>
    s32 searchBufferIndex(const sead::SafeString& name) const
    {
        return searchBufferIndex(T::getType(), name);
    }

    s32 searchTypeIndex(s32 type, const sead::SafeString& name) const;

    template <typename T>
    s32 searchTypeIndex(const sead::SafeString& name) const
    {
        return searchTypeIndex(T::getType(), name);
    }

    s32 searchType(s32 buffer_index) const;

    EnvObj* getEnvObj(s32 type, s32 index)
    {
        if (0 <= index && index < mTypeInfo[type].mMaxNum)
            return *mEnvObjPtrBuffer.get(mTypeInfo[type].mStartIndex + index);

        else
            return nullptr;
    }

    const EnvObj* getEnvObj(s32 type, s32 index) const
    {
        if (0 <= index && index < mTypeInfo[type].mMaxNum)
            return *mEnvObjPtrBuffer.get(mTypeInfo[type].mStartIndex + index);

        else
            return nullptr;
    }

    template <typename T>
    T* getEnvObj(s32 index)
    {
        return sead::DynamicCast<T>(getEnvObj(T::getType(), index));
    }

    template <typename T>
    const T* getEnvObj(s32 index) const
    {
        return sead::DynamicCast<T>(getEnvObj(T::getType(), index));
    }

    s32 getEnvObjStartIndex(s32 type) const
    {
        return mTypeInfo[type].mStartIndex;
    }

    template <typename T>
    s32 getEnvObjStartIndex(s32 type) const
    {
        return getEnvObjStartIndex(T::getType());
    }

    s32 getEnvObjMaxNum(s32 type) const
    {
        return mTypeInfo[type].mMaxNum;
    }

    template <typename T>
    s32 getEnvObjMaxNum(s32 type) const
    {
        return getEnvObjMaxNum(T::getType());
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
    sead::Buffer<EnvObj*> mEnvObjPtrBuffer;
    u32 _18[4 / sizeof(u32)];
};
static_assert(sizeof(EnvObjBuffer) == 0x20, "agl::env::EnvObjBuffer size mismatch");

} }
