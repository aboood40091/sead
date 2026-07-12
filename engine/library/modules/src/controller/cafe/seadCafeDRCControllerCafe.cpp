#include <controller/cafe/seadCafeDRCControllerCafe.h>
#include <controller/cafe/seadCafeVPadDeviceCafe.h>
#include <controller/seadControllerMgr.h>

namespace sead {

const f32 CafeDRCController::cTouchPadSizeX = 1280.0f;
const f32 CafeDRCController::cTouchPadSizeY = 720.0f;

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
        if (hold & VPAD_BUTTON_A)       mPadHold.set(cPadFlag_A);
        if (hold & VPAD_BUTTON_B)       mPadHold.set(cPadFlag_B);
        if (hold & VPAD_BUTTON_X)       mPadHold.set(cPadFlag_X);
        if (hold & VPAD_BUTTON_Y)       mPadHold.set(cPadFlag_Y);
        if (hold & VPAD_BUTTON_PLUS)    mPadHold.set(cPadFlag_Plus |
                                                     cPadFlag_Start);
        if (hold & VPAD_BUTTON_MINUS)   mPadHold.set(cPadFlag_Minus |
                                                     cPadFlag_Select);
        if (hold & VPAD_TRIGGER_L)      mPadHold.set(cPadFlag_L);
        if (hold & VPAD_TRIGGER_R)      mPadHold.set(cPadFlag_R);
        if (hold & VPAD_TRIGGER_ZL)     mPadHold.set(cPadFlag_ZL);
        if (hold & VPAD_TRIGGER_ZR)     mPadHold.set(cPadFlag_ZR);
        if (hold & VPAD_BUTTON_HOME)    mPadHold.set(cPadFlag_Home);
        if (hold & VPAD_BUTTON_UP)      mPadHold.set(cPadFlag_Up);
        if (hold & VPAD_BUTTON_DOWN)    mPadHold.set(cPadFlag_Down);
        if (hold & VPAD_BUTTON_LEFT)    mPadHold.set(cPadFlag_Left);
        if (hold & VPAD_BUTTON_RIGHT)   mPadHold.set(cPadFlag_Right);
        if (hold & VPAD_BUTTON_STICK_R) mPadHold.set(cPadFlag_2);
        if (hold & VPAD_BUTTON_STICK_L) mPadHold.set(cPadFlag_1);

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

        VPADGetTPCalibratedPoint(0, &tp_data, &status.tpFilteredData1);
        updateTouchPanelInfo_(&mTouchPanelInfos[1], tp_data.touch == VPAD_TP_TOUCH_ON, tp_data.x, tp_data.y);

        VPADGetTPCalibratedPoint(0, &tp_data, &status.tpFilteredData2);
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
