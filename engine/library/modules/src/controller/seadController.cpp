#include <controller/seadController.h>
#include <controller/seadControllerAddon.h>
#include <controller/seadControllerMgr.h>
#include <controller/seadControllerWrapperBase.h>

namespace sead {

Controller::Controller(ControllerMgr* mgr)
    : ControllerBase(cPadIdx_Max, cPadIdx_LeftStickUp, cPadIdx_RightStickUp, cPadIdx_Touch)
    , mId(ControllerDefine::cController_Null)
    , mMgr(mgr)
    , mAddons()
    , mWrappers()
{
    mAddons.initOffset(offsetof(ControllerAddon, mListNode));
    mWrappers.initOffset(offsetof(ControllerWrapperBase, mListNode));
}

void Controller::calc()
{
    u32 prev_hold = getHoldMask();
    bool prev_pointer_on = isPointerOn();

    calcImpl_();

    updateDerivativeParams_(prev_hold, prev_pointer_on);

    bool is_idle = true;

    for (OffsetList<ControllerAddon>::iterator it = mAddons.begin(); it != mAddons.end(); ++it)
        if ((*it).calc())
            is_idle = false;

    if (is_idle && isIdle_())
        mIdleFrame++;
    else
        mIdleFrame = 0;

    for (OffsetList<ControllerWrapperBase>::iterator it = mWrappers.begin(); it != mWrappers.end(); ++it)
        (*it).calc(prev_hold, prev_pointer_on);
}

ControllerAddon* Controller::getAddon(ControllerDefine::AddonId id) const
{
    for (OffsetList<ControllerAddon>::iterator it = mAddons.begin(); it != mAddons.end(); ++it)
    {
        ControllerAddon& addon = *it;
        if (addon.mId == id)
            return &addon;
    }

    return nullptr;
}

bool Controller::isIdle_()
{
    return isIdleBase_();
}

void Controller::setIdle_()
{
    setIdleBase_();

    for (OffsetList<ControllerWrapperBase>::iterator it = mWrappers.begin(); it != mWrappers.end(); ++it)
        (*it).setIdle();
}

} // namespace sead
