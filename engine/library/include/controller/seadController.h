#ifndef SEAD_CONTROLLER_H_
#define SEAD_CONTROLLER_H_

// TODO: Move this

namespace {

template <typename T> struct remove_pointer                    { typedef T type; };
template <typename T> struct remove_pointer<T*>                { typedef T type; };
template <typename T> struct remove_pointer<T* const>          { typedef T type; };
template <typename T> struct remove_pointer<T* volatile>       { typedef T type; };
template <typename T> struct remove_pointer<T* const volatile> { typedef T type; };

}

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
        cPadIdx_C               = 2,
        cPadIdx_X               = 3,
        cPadIdx_Y               = 4,
        cPadIdx_Z               = 5,
        cPadIdx_2               = 6, // Also Right-Stick Click
        cPadIdx_1               = 7, // Also Left-Stick Click
        cPadIdx_Home            = 8,
        cPadIdx_Minus           = 9,
        cPadIdx_Plus            = 10,
        cPadIdx_Start           = 11,
        cPadIdx_Select          = 12,
        cPadIdx_ZL              = cPadIdx_C,
        cPadIdx_ZR              = cPadIdx_Z,
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

    enum PadFlag
    {
        cPadFlag_A               = 1 << cPadIdx_A,
        cPadFlag_B               = 1 << cPadIdx_B,
        cPadFlag_C               = 1 << cPadIdx_C,
        cPadFlag_X               = 1 << cPadIdx_X,
        cPadFlag_Y               = 1 << cPadIdx_Y,
        cPadFlag_Z               = 1 << cPadIdx_Z,
        cPadFlag_2               = 1 << cPadIdx_2, // Also Right-Stick Click
        cPadFlag_1               = 1 << cPadIdx_1, // Also Left-Stick Click
        cPadFlag_Home            = 1 << cPadIdx_Home,
        cPadFlag_Minus           = 1 << cPadIdx_Minus,
        cPadFlag_Plus            = 1 << cPadIdx_Plus,
        cPadFlag_Start           = 1 << cPadIdx_Start,
        cPadFlag_Select          = 1 << cPadIdx_Select,
        cPadFlag_ZL              = cPadFlag_C,
        cPadFlag_ZR              = cPadFlag_Z,
        cPadFlag_L               = 1 << cPadIdx_L,
        cPadFlag_R               = 1 << cPadIdx_R,
        cPadFlag_Touch           = 1 << cPadIdx_Touch,
        cPadFlag_Up              = 1 << cPadIdx_Up,
        cPadFlag_Down            = 1 << cPadIdx_Down,
        cPadFlag_Left            = 1 << cPadIdx_Left,
        cPadFlag_Right           = 1 << cPadIdx_Right,
        cPadFlag_LeftStickUp     = 1 << cPadIdx_LeftStickUp,
        cPadFlag_LeftStickDown   = 1 << cPadIdx_LeftStickDown,
        cPadFlag_LeftStickLeft   = 1 << cPadIdx_LeftStickLeft,
        cPadFlag_LeftStickRight  = 1 << cPadIdx_LeftStickRight,
        cPadFlag_RightStickUp    = 1 << cPadIdx_RightStickUp,
        cPadFlag_RightStickDown  = 1 << cPadIdx_RightStickDown,
        cPadFlag_RightStickLeft  = 1 << cPadIdx_RightStickLeft,
        cPadFlag_RightStickRight = 1 << cPadIdx_RightStickRight,
        cPadFlag_All             = (1 << cPadIdx_Max) - 1
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
    ControllerDefine::ControllerId getId() const { return mId; }

    // TODO: Add/remove addon
    ControllerAddon* getAddon(ControllerDefine::AddonId id) const;

    template <typename T>
    T getAddonAs() const;

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

template <typename T>
T Controller::getAddonAs() const
{
    for (OffsetList<ControllerAddon>::iterator it = mAddons.begin(); it != mAddons.end(); ++it)
    {
        T addon = DynamicCast<remove_pointer<T>::type>(&(*it));
        if (addon)
            return addon;
    }

    return nullptr;
}

} // namespace sead

#endif // SEAD_CONTROLLER_H_
