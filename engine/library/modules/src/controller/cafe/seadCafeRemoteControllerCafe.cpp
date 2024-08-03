#include <controller/cafe/seadCafeRemoteControllerCafe.h>
#include <controller/cafe/seadCafeWPadDeviceCafe.h>
#include <controller/seadControllerMgr.h>

namespace sead {

CafeRemoteController::CafeRemoteController(ControllerMgr* mgr, s32 channel)
    : Controller(mgr)
    , mChannel(channel)
    , mDeviceTypeCurrent(WPAD_DEV_NOT_FOUND)
    , mDeviceTypeLastConnected(WPAD_DEV_NOT_FOUND)
    , mIsConnected(false)
    , mIsEnableCL(false)
{
    mId = ControllerDefine::cController_CafeRemote;
}

void CafeRemoteController::calcImpl_()
{
    CafeWPadDevice* device = DynamicCast<CafeWPadDevice>(ControllerMgr::instance()->getControlDevice(ControllerDefine::cDevice_CafeWPad));
    if (!device)
        return;

    const CafeWPadDevice::KPadInfo& k_pad_info = device->getKPadInfo(mChannel);

    if (k_pad_info.last_read_length > 0 || k_pad_info.last_read_error == KPAD_READ_ERR_NO_DATA)
    {
        const KPADStatus& status = k_pad_info.status[0];
        if (status.wpad_err == WPAD_ERR_INVALID)
            return;

        if (status.dev_type != WPAD_DEV_NOT_FOUND)
        {
            mPadHold.makeAllZero();

            if (status.dev_type == WPAD_DEV_URCC)
            {
                u32 hold = status.ex_status.uc.hold;

                mPadHold.change(1 << Controller::cPadIdx_A,      hold & KPAD_UC_BUTTON_A);
                mPadHold.change(1 << Controller::cPadIdx_B,      hold & KPAD_UC_BUTTON_B);
                mPadHold.change(1 << Controller::cPadIdx_X,      hold & KPAD_UC_BUTTON_X);
                mPadHold.change(1 << Controller::cPadIdx_Y,      hold & KPAD_UC_BUTTON_Y);
                mPadHold.change(1 << Controller::cPadIdx_L,      hold & KPAD_UC_TRIGGER_L);
                mPadHold.change(1 << Controller::cPadIdx_R,      hold & KPAD_UC_TRIGGER_R);
                mPadHold.change(1 << Controller::cPadIdx_C,      hold & KPAD_UC_TRIGGER_ZL);
                mPadHold.change(1 << Controller::cPadIdx_Z,      hold & KPAD_UC_TRIGGER_ZR);
                mPadHold.change(1 << Controller::cPadIdx_Plus |
                                1 << Controller::cPadIdx_Start,  hold & KPAD_UC_BUTTON_PLUS);
                mPadHold.change(1 << Controller::cPadIdx_Minus |
                                1 << Controller::cPadIdx_Select, hold & KPAD_UC_BUTTON_MINUS);
                mPadHold.change(1 << Controller::cPadIdx_Home,   hold & KPAD_UC_BUTTON_HOME);
                mPadHold.change(1 << Controller::cPadIdx_Right,  hold & KPAD_UC_BUTTON_RIGHT);
                mPadHold.change(1 << Controller::cPadIdx_Left,   hold & KPAD_UC_BUTTON_LEFT);
                mPadHold.change(1 << Controller::cPadIdx_Up,     hold & KPAD_UC_BUTTON_UP);
                mPadHold.change(1 << Controller::cPadIdx_Down,   hold & KPAD_UC_BUTTON_DOWN);
                mPadHold.change(1 << Controller::cPadIdx_1,      hold & KPAD_UC_BUTTON_STICK_L);
                mPadHold.change(1 << Controller::cPadIdx_2,      hold & KPAD_UC_BUTTON_STICK_R);

                mLeftStick.set(status.ex_status.uc.lstick.x, status.ex_status.uc.lstick.y);
                mRightStick.set(status.ex_status.uc.rstick.x, status.ex_status.uc.rstick.y);

                setPointerWithBound_(false, false, Vector2f::zero);
            }
            else
            {
                u32 hold = status.hold;

                mPadHold.change(1 << Controller::cPadIdx_A,      hold & KPAD_BUTTON_A);
                mPadHold.change(1 << Controller::cPadIdx_B,      hold & KPAD_BUTTON_B);
                mPadHold.change(1 << Controller::cPadIdx_1,      hold & KPAD_BUTTON_1);
                mPadHold.change(1 << Controller::cPadIdx_2,      hold & KPAD_BUTTON_2);
                mPadHold.change(1 << Controller::cPadIdx_Z,      hold & KPAD_BUTTON_Z);
                mPadHold.change(1 << Controller::cPadIdx_C,      hold & KPAD_BUTTON_C);
                mPadHold.change(1 << Controller::cPadIdx_Plus |
                                1 << Controller::cPadIdx_Start,  hold & KPAD_BUTTON_PLUS);
                mPadHold.change(1 << Controller::cPadIdx_Minus |
                                1 << Controller::cPadIdx_Select, hold & KPAD_BUTTON_MINUS);
                mPadHold.change(1 << Controller::cPadIdx_Home,   hold & KPAD_BUTTON_HOME);
                mPadHold.change(1 << Controller::cPadIdx_Right,  hold & KPAD_BUTTON_RIGHT);
                mPadHold.change(1 << Controller::cPadIdx_Left,   hold & KPAD_BUTTON_LEFT);
                mPadHold.change(1 << Controller::cPadIdx_Up,     hold & KPAD_BUTTON_UP);
                mPadHold.change(1 << Controller::cPadIdx_Down,   hold & KPAD_BUTTON_DOWN);

                if (status.dev_type == WPAD_DEV_FREESTYLE || status.dev_type == WPAD_DEV_MPLS_FREESTYLE)
                    mLeftStick.set(status.ex_status.fs.stick.x, status.ex_status.fs.stick.y);
                else
                    mLeftStick.set(0.0f, 0.0f);

                mRightStick.set(0.0f, 0.0f);

                setPointerWithBound_(status.dpd_valid_fg > 0, hold & KPAD_BUTTON_A, Vector2f(status.pos.x, status.pos.y));

                if (mIsEnableCL && (status.dev_type == WPAD_DEV_CLASSIC || status.dev_type == WPAD_DEV_MPLS_CLASSIC))
                {
                    u32 cl_hold = status.ex_status.cl.hold;

                    if (cl_hold & WPAD_CL_BUTTON_A)     mPadHold.set(1 << Controller::cPadIdx_A);
                    if (cl_hold & WPAD_CL_BUTTON_B)     mPadHold.set(1 << Controller::cPadIdx_B);
                    if (cl_hold & WPAD_CL_BUTTON_X)     mPadHold.set(1 << Controller::cPadIdx_X);
                    if (cl_hold & WPAD_CL_BUTTON_Y)     mPadHold.set(1 << Controller::cPadIdx_Y);
                    if (cl_hold & WPAD_CL_BUTTON_PLUS)  mPadHold.set(1 << Controller::cPadIdx_Plus |
                                                                     1 << Controller::cPadIdx_Start);
                    if (cl_hold & WPAD_CL_BUTTON_MINUS) mPadHold.set(1 << Controller::cPadIdx_Minus |
                                                                     1 << Controller::cPadIdx_Select);
                    if (cl_hold & WPAD_CL_TRIGGER_L)    mPadHold.set(1 << Controller::cPadIdx_L);
                    if (cl_hold & WPAD_CL_TRIGGER_R)    mPadHold.set(1 << Controller::cPadIdx_R);
                    if (cl_hold & WPAD_CL_TRIGGER_ZL)   mPadHold.set(1 << Controller::cPadIdx_C);
                    if (cl_hold & WPAD_CL_TRIGGER_ZR)   mPadHold.set(1 << Controller::cPadIdx_Z);
                    if (cl_hold & WPAD_CL_BUTTON_HOME)  mPadHold.set(1 << Controller::cPadIdx_Home);
                    if (cl_hold & WPAD_CL_BUTTON_UP)    mPadHold.set(1 << Controller::cPadIdx_Up);
                    if (cl_hold & WPAD_CL_BUTTON_DOWN)  mPadHold.set(1 << Controller::cPadIdx_Down);
                    if (cl_hold & WPAD_CL_BUTTON_LEFT)  mPadHold.set(1 << Controller::cPadIdx_Left);
                    if (cl_hold & WPAD_CL_BUTTON_RIGHT) mPadHold.set(1 << Controller::cPadIdx_Right);

                    mLeftStick.set(status.ex_status.cl.lstick.x, status.ex_status.cl.lstick.y);
                    mRightStick.set(status.ex_status.cl.rstick.x, status.ex_status.cl.rstick.y);
                }
            }

            mLeftAnalogTrigger = 0.0f;
            mRightAnalogTrigger = 0.0f;
            mIsConnected = true;
            mDeviceTypeCurrent = status.dev_type;
        }
        else
        {
            mIsConnected = false;
            mDeviceTypeCurrent = WPAD_DEV_NOT_FOUND;
        }
    }
    else
    {
        mIsConnected = false;
        mDeviceTypeCurrent = WPAD_DEV_NOT_FOUND;
    }

    if (mDeviceTypeCurrent < WPAD_DEV_NOT_SUPPORTED)
        mDeviceTypeLastConnected = mDeviceTypeCurrent;
}

} // namespace sead
