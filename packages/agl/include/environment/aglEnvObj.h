#pragma once

#include <math/seadMatrix.h>
#include <prim/seadBitFlag.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <utility/aglNamedObj.h>
#include <utility/aglNamedObjIndex.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace env {

class EnvObj : public utl::IParameterObj, public utl::INamedObj
{
public:
    class Index : public utl::INamedObjIndex
    {
    public:
        Index()
            : utl::INamedObjIndex()
            , mEnvType(0)
        {
        }

        virtual const sead::SafeString& getNamedObjName(s32) const;
        virtual s32 getNamedObjNum() const;

    protected:
        s32 mEnvType;
    };
    static_assert(sizeof(Index) == 0x54);

public:
    EnvObj();

    virtual void postRead_() { callbackLoadData(); }

    SEAD_RTTI_BASE(EnvObj)

public:
    virtual ~EnvObj();

    virtual void initialize(s32, sead::Heap* heap) { }
    virtual void update() { }
    virtual void updateView(const sead::Matrix34f&, const sead::Matrix44f&, s32) { }
    virtual void drawDebug(const sead::Matrix34f&, const sead::Matrix44f&, s32) { }
    virtual void callbackLoadData() { }
    virtual const sead::SafeString& getEnvObjName() const { return *mName; }
    virtual const sead::SafeString& getGroupName() const { return *mGroup; }
    virtual const sead::SafeString& getObjName() const { return getEnvObjName(); }
    virtual bool isHostIOEnabled() const { return mFlag.isOnBit(0); }
    virtual s32 getObjType() const { return getTypeID(); }
    virtual s32 getTypeID() const = 0;

    void setName(const sead::SafeString& name)
    {
        *mName = name;
    }

protected:
    u32 _74;
    u16 _78;
    sead::BitFlag16 mFlag;
    utl::Parameter< sead::FixedSafeString<32> > mName;
    utl::Parameter< sead::FixedSafeString<32> > mGroup;
    utl::Parameter<bool> mEnable;
};
static_assert(sizeof(EnvObj) == 0x108);

} }

#define AGL_ENV_OBJ_TYPE_INFO()                                 \
    private:                                                    \
        static s32* sTypeInfo;                                  \
                                                                \
    public:                                                     \
        static s32 getType() { return *sTypeInfo; }             \
        virtual s32 getTypeID() const { return *sTypeInfo; }
