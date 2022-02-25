#include <controller/seadControllerBase.h>

namespace sead {

const f32 ControllerBase::cStickHoldThresholdDefault    = 0.5f;
const f32 ControllerBase::cStickReleaseThresholdDefault = 0.25f;

const Vector2f ControllerBase::cInvalidPointer(Mathf::minNumber(), Mathf::minNumber());
const Vector2i ControllerBase::cInvalidPointerS32(Mathi::minNumber(), Mathi::minNumber());

ControllerBase::ControllerBase(s32 pad_bit_max, s32 left_stick_cross_start_bit, s32 right_stick_cross_start_bit, s32 touch_key_bit)
    : mPadTrig()
    , mPadRelease()
    , mPadRepeat()
    , mPointerFlag()
    , mPointerS32(cInvalidPointerS32)
    , mPointerBound()
    , mLeftStickHoldThreshold(0.5f)
    , mRightStickHoldThreshold(0.5f)
    , mLeftStickReleaseThreshold(0.25f)
    , mRightStickReleaseThreshold(0.25f)
    , mPadBitMax(pad_bit_max)
    , mLeftStickCrossStartBit(left_stick_cross_start_bit)
    , mRightStickCrossStartBit(right_stick_cross_start_bit)
    , mTouchKeyBit(touch_key_bit)
    , mIdleFrame(0)
    , mPadHold()
    , mPointer(cInvalidPointer)
    , mLeftStick(0.0f, 0.0f)
    , mRightStick(0.0f, 0.0f)
    , mLeftAnalogTrigger(0.0f)
    , mRightAnalogTrigger(0.0f)
{
    if (cPadIdxMaxBase < pad_bit_max)
    {
        //SEAD_ASSERT_MSG(false, "illegal padBitMax[%d]", pad_bit_max);
        mPadBitMax = cPadIdxMaxBase;
    }

    for (u32 i = 0; i < cPadIdxMaxBase; i++)
    {
        mPadRepeatDelays[i] = 30;
        mPadRepeatPulses[i] = 1;
        mPadHoldCounts[i] = 0;
    }
}

void ControllerBase::setPointerWithBound_(bool is_on, bool touchkey_hold, const Vector2f& pos)
{
    if (is_on)
    {
        if (!mPointerBound.isUndef())
        {
            if (mPointerBound.isInside(pos))
            {
                mPointer.x = pos.x - mPointerBound.getMin().x;
                mPointer.y = pos.y - mPointerBound.getMin().y;
            }
            else
            {
                is_on = false;
            }
        }
        else
        {
            mPointer.x = pos.x;
            mPointer.y = pos.y;
        }
    }

    mPointerFlag.change(cPointerOn, is_on);

    if (mTouchKeyBit >= 0)
    {
        mPadHold.changeBit(mTouchKeyBit, is_on && touchkey_hold);
    }

    if (mPointerFlag.isOn(cPointerUnkFlag3))
    {
        if (mPointerFlag.isOff(cPointerOn))
        {
            mPointer.x = cInvalidPointer.x;
            mPointer.y = cInvalidPointer.y;
        }

        mPointerFlag.reset(cPointerUnkFlag3);
    }
}

void ControllerBase::updateDerivativeParams_(u32 prev_hold, bool prev_pointer_on)
{
    u32 stick_hold = 0;

    if (mLeftStickCrossStartBit >= 0)
    {
        stick_hold |= getStickHold_(prev_hold, mLeftStick, mLeftStickHoldThreshold, mLeftStickReleaseThreshold, mLeftStickCrossStartBit);
    }

    if (mRightStickCrossStartBit >= 0)
    {
        stick_hold |= getStickHold_(prev_hold, mRightStick, mRightStickHoldThreshold, mRightStickReleaseThreshold, mRightStickCrossStartBit);
    }

    mPadHold.setDirect(mPadHold.getDirect() & ~createStickCrossMask_() | stick_hold);

    mPadTrig.setDirect(~prev_hold & mPadHold.getDirect());
    mPadRelease.setDirect(prev_hold & ~mPadHold.getDirect());
    mPadRepeat.setDirect(0);

    for (s32 i = 0; i < mPadBitMax; i++)
    {
        if (mPadHold.isOnBit(i))
        {
            if (mPadRepeatPulses[i])
            {
                if (mPadHoldCounts[i] == mPadRepeatDelays[i])
                    mPadRepeat.setBit(i);

                else if (mPadRepeatDelays[i] < mPadHoldCounts[i] &&
                         (mPadHoldCounts[i] - mPadRepeatDelays[i]) % mPadRepeatPulses[i] == 0)
                {
                    mPadRepeat.setBit(i);
                }
            }

            mPadHoldCounts[i]++;
        }
        else
        {
            mPadHoldCounts[i] = 0;
        }
    }

    mPointerFlag.change(cPointerOnNow, !prev_pointer_on && mPointerFlag.isOn(cPointerOn));
    mPointerFlag.change(cPointerOffNow, prev_pointer_on && mPointerFlag.isOff(cPointerOn));

    mPointerS32.x = (s32)mPointer.x;
    mPointerS32.y = (s32)mPointer.y;
}

} // namespace sead
