#ifndef SEAD_CONTROLLER_BASE_H_
#define SEAD_CONTROLLER_BASE_H_

#include <math/seadBoundBox.h>
#include <math/seadVector.h>
#include <prim/seadBitFlag.h>

namespace sead {

class ControllerBase
{
public:
    static const f32 cStickHoldThresholdDefault;
    static const f32 cStickReleaseThresholdDefault;
    static const Vector2f cInvalidPointer;
    static const Vector2i cInvalidPointerS32;

public:
    ControllerBase(s32, s32, s32, s32);

    u32 getPadHoldCount(s32 bit) const;
    void setPadRepeat(u32 mask, u8 delay, u8 pulse);
    void setLeftStickCrossThreshold(f32 hold_threshold, f32 release_threshold);
    void setRightStickCrossThreshold(f32 hold_threshold, f32 release_threshold);
    void setPointerBound(const BoundBox2f& bound);
    void setPointerWithBound_(bool, bool, const Vector2f& pointer);
    u32 getStickHold_(u32 prev_hold, const Vector2f& stick, f32 hold_threshold, f32 release_threshold, s32 start_bit);
    u32 createStickCrossMask_();
    void updateDerivativeParams_(u32, bool);
    bool isIdleBase_();
    void setIdleBase_();

    bool isTrigWithRepeat(u32 mask) const
    {
        u32 trig = mPadTrig.getDirect();
        u32 repeat = mPadRepeat.getDirect();
        return (repeat | trig) & mask;
    }

    BitFlag32 mPadTrig;
    BitFlag32 mPadRelease;
    BitFlag32 mPadRepeat;
    BitFlag32 mPointerFlag;
    Vector2i mPointerS32;
    BoundBox2f mPointerBound;
    u32 mPadHoldCounts[32];
    u8 mPadRepeatDelays[32];
    u8 mPadRepeatPulses[32];
    f32 mLeftStickHoldThreshold;
    f32 mRightStickHoldThreshold;
    f32 mLeftStickReleaseThreshold;
    f32 mRightStickReleaseThreshold;
    s32 mPadBitMax;
    s32 mLeftStickCrossStartBit;
    s32 mRightStickCrossStartBit;
    s32 mTouchKeyBit;
    s32 mIdleFrame;
    BitFlag32 mPadHold;
    Vector2f mPointer;
    Vector2f mLeftStick;
    Vector2f mRightStick;
    f32 mLeftAnalogTrigger;
    f32 mRightAnalogTrigger;
};
#ifdef cafe
static_assert(sizeof(ControllerBase) == 0x130, "sead::ControllerBase size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_BASE_H_
