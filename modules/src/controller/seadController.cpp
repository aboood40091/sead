#include <controller/seadController.h>

namespace sead {

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

} // namespace sead
