#include <controller/cafe/seadCafeDebugControllerCafe.h>
#include <controller/cafe/seadCafeDebugPadDeviceCafe.h>
#include <controller/seadControllerMgr.h>

namespace sead {

CafeDebugController::CafeDebugController(ControllerMgr* mgr, s32 channel)
    : Controller(mgr)
    , mChannel(channel)
    , mIsConnected(false)
{
    mId = ControllerDefine::cController_CafeDebug;
}

#if 0 // defined(SEAD_TARGET_DEBUG)

void CafeDebugController::calcImpl_()
{
    CafeDebugPadDevice* device = DynamicCast<CafeDebugPadDevice>(ControllerMgr::instance()->getControlDevice(ControllerDefine::cDevice_CafeDebugPad));
    if (!device)
        return;

    const PADStatus& status = device->getPadStatus(mChannel);
    if (status.err == PAD_ERR_NONE)
    {
        u32 hold = status.button;

        mPadHold.makeAllZero();
        mPadHold.change(cPadFlag_A,      hold & PAD_BUTTON_A);
        mPadHold.change(cPadFlag_B,      hold & PAD_BUTTON_B);
        mPadHold.change(cPadFlag_X,      hold & PAD_BUTTON_X);
        mPadHold.change(cPadFlag_Y,      hold & PAD_BUTTON_Y);
        mPadHold.change(cPadFlag_R,      hold & PAD_TRIGGER_R);
        mPadHold.change(cPadFlag_L,      hold & PAD_TRIGGER_L);
        mPadHold.change(cPadFlag_Z,      hold & PAD_TRIGGER_Z);
        mPadHold.change(cPadFlag_Start,  hold & PAD_BUTTON_START);
        mPadHold.change(cPadFlag_Right,  hold & PAD_BUTTON_RIGHT);
        mPadHold.change(cPadFlag_Left,   hold & PAD_BUTTON_LEFT);
        mPadHold.change(cPadFlag_Up,     hold & PAD_BUTTON_UP);
        mPadHold.change(cPadFlag_Down,   hold & PAD_BUTTON_DOWN);

        clampStickValue_(&mLeftStick, status.stickX, status.stickY);
        clampStickValue_(&mRightStick, status.substickX, status.substickY);

        // mfw Nintendo sets the analog triggers to 0.0f for some reason, even though the PADStatus struct has values for them
        mLeftAnalogTrigger  = /* (f32)status.triggerLeft  / 255.0f */ 0.0f;
        mRightAnalogTrigger = /* (f32)status.triggerRight / 255.0f */ 0.0f;

        mIsConnected = true;
    }
    else
    {
        mIsConnected = false;
    }
}

#endif

void CafeDebugController::clampStickValue_(Vector2f* out, s8 x, s8 y)
{
    static const f32 max_val = 56.0f;

    out->set((f32)x / max_val, (f32)y / max_val);
    {
        f32 length = out->length();
        if (length > 1.0f)
            *out /= length;
    }
}

} // namespace sead
