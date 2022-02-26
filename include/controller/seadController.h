#ifndef SEAD_CONTROLLER_H_
#define SEAD_CONTROLLER_H_

#include <container/seadOffsetList.h>
#include <controller/seadControllerBase.h>
#include <controller/seadControllerDefine.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class ControllerAddon;
class ControllerMgr;
class ControllerWrapperBase;

class Controller : public ControllerBase
{
    SEAD_RTTI_BASE(Controller)

public:
    enum
    {
        cPadIdxTouch           = 15,
        cPadIdxUp              = 16,
        cPadIdxDown            = 17,
        cPadIdxLeft            = 18,
        cPadIdxRight           = 19,
        cPadIdxLeftStickUp     = 20,
        cPadIdxLeftStickDown   = 21,
        cPadIdxLeftStickLeft   = 22,
        cPadIdxLeftStickRight  = 23,
        cPadIdxRightStickUp    = 24,
        cPadIdxRightStickDown  = 25,
        cPadIdxRightStickLeft  = 26,
        cPadIdxRightStickRight = 27,
        cPadIdxMax             = 28
    };

public:
    Controller(ControllerMgr* mgr);
    virtual ~Controller() { }

    virtual void calc();
    virtual bool isConnected() const { return true; }
    virtual void calcImpl_() = 0;
    virtual bool isIdle_();
    virtual void setIdle_();

protected:
    ControllerDefine::ControllerId mId;
    ControllerMgr* mMgr;
    OffsetList<ControllerAddon> mAddons;
    OffsetList<ControllerWrapperBase> mWrappers;
};
#ifdef cafe
static_assert(sizeof(Controller) == 0x15C, "sead::Controller size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_H_
