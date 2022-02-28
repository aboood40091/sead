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
    enum PadIdx
    {
        cPadIdx_A               = 0,
        cPadIdx_B               = 1,
        cPadIdx_ZL              = 2,
        cPadIdx_X               = 3,
        cPadIdx_Y               = 4,
        cPadIdx_ZR              = 5,
        cPadIdx_2               = 6, // Also Right-Stick Click
        cPadIdx_1               = 7, // Also Left-Stick Click
        cPadIdx_Home            = 8,
        cPadIdx_Minus           = 9,
        cPadIdx_Plus            = 10,
        cPadIdx_Start           = 11,
        cPadIdx_Select          = 12,
        cPadIdx_L               = 13,
        cPadIdx_R               = 14,
        cPadIdx_Touch           = 15,
        cPadIdx_Up              = 16,
        cPadIdx_Down            = 17,
        cPadIdx_Left            = 18,
        cPadIdx_Right           = 19,
        cPadIdx_LeftStickUp     = 20,
        cPadIdx_LeftStickDown   = 21,
        cPadIdx_LeftStickLeft   = 22,
        cPadIdx_LeftStickRight  = 23,
        cPadIdx_RightStickUp    = 24,
        cPadIdx_RightStickDown  = 25,
        cPadIdx_RightStickLeft  = 26,
        cPadIdx_RightStickRight = 27,
        cPadIdx_Max             = 28
    };

public:
    Controller(ControllerMgr* mgr);
    virtual ~Controller() { }

    virtual void calc();
    virtual bool isConnected() const { return true; }

protected:
    virtual void calcImpl_() = 0;
    virtual bool isIdle_();
    virtual void setIdle_();

public:
    // TODO: Add/remove addon
    ControllerAddon* getAddon(ControllerDefine::AddonId id) const;

protected:
    ControllerDefine::ControllerId mId;
    ControllerMgr* mMgr;
    OffsetList<ControllerAddon> mAddons;
    OffsetList<ControllerWrapperBase> mWrappers;

    friend class ControllerWrapperBase;
    friend class ControllerMgr;
};
#ifdef cafe
static_assert(sizeof(Controller) == 0x15C, "sead::Controller size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_H_