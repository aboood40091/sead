#include <controller/cafe/seadCafeDRCControllerCafe.h>
#include <controller/cafe/seadCafeVPadDeviceCafe.h>
#include <controller/seadControllerMgr.h>

namespace sead {

CafeDRCController::CafeDRCController(ControllerMgr* mgr)
    : Controller(mgr)
    , mIsConnected(false)
    , mTouchPanelSamplingUseNum(3)
    , mTimeout(0)
    , mCurrentTouchPanel(0)
{
    mId = ControllerDefine::cController_CafeDRC;
}

void CafeDRCController::setTouchPanelSamplingUseNum(u32 num)
{
    if (num)
    {
        mTouchPanelSamplingUseNum = num;
    }
    else
    {
        //SEAD_ASSERT_MSG(false, "illegal sampling use num");
    }
}

void CafeDRCController::calcImpl_()
{
    CafeVPadDevice* device = DynamicCast<CafeVPadDevice>(ControllerMgr::instance()->getControlDevice(ControllerDefine::cDevice_CafeVPad));
    if (!device)
        return;

    const CafeVPadDevice::VPadInfo& v_pad_info = device->getVPadInfo();

    if (v_pad_info.last_read_length > 0 || (v_pad_info.last_read_error == VPAD_READ_ERR_NO_DATA && mIsConnected && mTimeout < 180))
    {
        const VPADStatus& status = v_pad_info.status[0];
        u32 hold = status.hold;

        mPadHold.makeAllZero();
        mPadHold.change(1 << Controller::cPadIdx_A,      hold & VPAD_BUTTON_A);
        mPadHold.change(1 << Controller::cPadIdx_B,      hold & VPAD_BUTTON_B);
        mPadHold.change(1 << Controller::cPadIdx_X,      hold & VPAD_BUTTON_X);
        mPadHold.change(1 << Controller::cPadIdx_Y,      hold & VPAD_BUTTON_Y);
        mPadHold.change(1 << Controller::cPadIdx_Plus |
                        1 << Controller::cPadIdx_Start,  hold & VPAD_BUTTON_PLUS);
        mPadHold.change(1 << Controller::cPadIdx_Minus |
                        1 << Controller::cPadIdx_Select, hold & VPAD_BUTTON_MINUS);
        mPadHold.change(1 << Controller::cPadIdx_L,      hold & VPAD_TRIGGER_L);
        mPadHold.change(1 << Controller::cPadIdx_R,      hold & VPAD_TRIGGER_R);
        mPadHold.change(1 << Controller::cPadIdx_ZL,     hold & VPAD_TRIGGER_ZL);
        mPadHold.change(1 << Controller::cPadIdx_ZR,     hold & VPAD_TRIGGER_ZR);
        mPadHold.change(1 << Controller::cPadIdx_Home,   hold & VPAD_BUTTON_HOME);
        mPadHold.change(1 << Controller::cPadIdx_Up,     hold & VPAD_BUTTON_UP);
        mPadHold.change(1 << Controller::cPadIdx_Down,   hold & VPAD_BUTTON_DOWN);
        mPadHold.change(1 << Controller::cPadIdx_Left,   hold & VPAD_BUTTON_LEFT);
        mPadHold.change(1 << Controller::cPadIdx_Right,  hold & VPAD_BUTTON_RIGHT);
        mPadHold.change(1 << Controller::cPadIdx_2,      hold & VPAD_BUTTON_STICK_R);
        mPadHold.change(1 << Controller::cPadIdx_1,      hold & VPAD_BUTTON_STICK_L);

        mLeftStick.set(status.lStick.x, status.lStick.y);
        mRightStick.set(status.rStick.x, status.rStick.y);

        mLeftAnalogTrigger = 0.0f;
        mRightAnalogTrigger = 0.0f;

        bool tp_valid_x = false;
        bool tp_valid_y = false;

        u32 sample_num = Mathu::clamp2(1, v_pad_info.last_read_length, mTouchPanelSamplingUseNum);
        VPADTPData tp_data;

        for (u32 i = 0; i < sample_num; i++)
        {
            const VPADTPData& tp_sample_data = v_pad_info.status[i].tpData;
            if (tp_sample_data.validity == VPAD_TP_VALIDITY_INVALID_XY)
                continue;

            VPADTPData tp_calibrated_data;
            VPADGetTPCalibratedPoint(0, &tp_calibrated_data, &tp_sample_data);

            if (!(tp_sample_data.validity & VPAD_TP_VALIDITY_INVALID_X))
            {
                tp_data.x = tp_calibrated_data.x;
                tp_valid_x = true;
            }

            if (!(tp_sample_data.validity & VPAD_TP_VALIDITY_INVALID_Y))
            {
                tp_data.y = tp_calibrated_data.y;
                tp_valid_y = true;
            }

            if (tp_valid_x && tp_valid_y)
                break;
        }

        updateTouchPanelInfo_(&mTouchPanelInfos[0], tp_valid_x && tp_valid_y, tp_data.x, tp_data.y);

        VPADGetTPCalibratedPoint(0, &tp_data, &v_pad_info.status[0].tpFilteredData1);
        updateTouchPanelInfo_(&mTouchPanelInfos[1], tp_data.touch == VPAD_TP_TOUCH_ON, tp_data.x, tp_data.y);

        VPADGetTPCalibratedPoint(0, &tp_data, &v_pad_info.status[0].tpFilteredData2);
        updateTouchPanelInfo_(&mTouchPanelInfos[2], tp_data.touch == VPAD_TP_TOUCH_ON, tp_data.x, tp_data.y);

        const TouchPanelInfo& tp_info = mTouchPanelInfos[mCurrentTouchPanel];
        setPointerWithBound_(tp_info.flags.isOn(cPointerOn), true, tp_info.pointer);

        if (v_pad_info.last_read_error == VPAD_READ_ERR_NO_DATA)
            mTimeout++;
        else
            mTimeout = 0;

        mIsConnected = true;
    }
    else
    {
        mIsConnected = false;
        mTimeout = 0;
    }
}

void CafeDRCController::updateTouchPanelInfo_(TouchPanelInfo* tp_info, bool tp_touch_on, f32 x, f32 y)
{
    bool prev_pointer_on = tp_info->flags.isOn(cPointerOn);

    tp_info->flags.change(cPointerOn, tp_touch_on);

    tp_info->flags.change(cPointerOnNow, !prev_pointer_on && tp_touch_on);
    tp_info->flags.change(cPointerOffNow, prev_pointer_on && !tp_touch_on);

    if (tp_touch_on)
        tp_info->pointer.set(x, y);
}

} // namespace sead
