#pragma once

#include <utility/aglNamedObj.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace env {

class EnvObj : public utl::IParameterObj, public utl::INamedObj
{
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
