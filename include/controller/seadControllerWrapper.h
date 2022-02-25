#ifndef SEAD_CONTROLLER_WRAPPER_H_
#define SEAD_CONTROLLER_WRAPPER_H_

#include <controller/seadControllerWrapperBase.h>

namespace sead {

class ControllerWrapper : public ControllerWrapperBase
{
    SEAD_RTTI_OVERRIDE(ControllerWrapper, ControllerWrapperBase)

public:
    static const u8 cPadConfigDefault[28];

public:
    ControllerWrapper();
    virtual ~ControllerWrapper() { }

    virtual void calc(u32, bool);

    u32 createPadMaskFromControllerPadMask_(u32) const;
    void setPadConfig(s32, const u8*, bool);

    u8 mPadConfig[32];
};
#ifdef cafe
static_assert(sizeof(ControllerWrapper) == 0x194, "sead::ControllerWrapper size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_WRAPPER_H_
