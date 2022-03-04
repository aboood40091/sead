#include <controller/cafe/seadCafeDRCControllerCafe.h>
#include <controller/cafe/seadCafeDRCPatternRumbleAddonCafe.h>
#include <prim/seadMemUtil.h>

namespace sead {

CafeDRCPatternRumbleAddon::CafeDRCPatternRumbleAddon(CafeDRCController* controller)
    : PatternRumbleAddon(controller)
    , mPatternLength(0)
    , mEndTime()
{
}

bool CafeDRCPatternRumbleAddon::calc()
{
    if (mPattern)
    {
        if (mPatternIdx == 0)
        {
            {
                u32 i = 0;
                MemUtil::fillZero(mPatternBuf, VPAD_MOTOR_PATTERN_SIZE_MAX);

                while (mPattern[i] != '\0')
                {
                    if (mPattern[i] == '*')
                        mPatternBuf[i / 8] |= 1 << (i & 7);

                    if (++i == VPAD_MOTOR_PATTERN_LENGTH_MAX)
                        break;
                }

                mPatternLength = i;
            }

            VPADControlMotor(0, mPatternBuf, mPatternLength);

            mEndTime.setNow();
            mEndTime += TickSpan::makeFromMicroSeconds(mPatternLength * 8333);
        }

        if (mEndTime.diffToNow().toS64() >= 0)
        {
            mPatternIdx = 0;

            if (mPatternDuration)
            {
                if (mPatternDuration == 1)
                {
                    mPattern = NULL;
                    mPatternDuration = 0;

                    return false;
                }

                mPatternDuration--;
            }

            VPADControlMotor(0, mPatternBuf, mPatternLength);

            mEndTime.setNow();
            mEndTime += TickSpan::makeFromMicroSeconds(mPatternLength * 8333);
        }

        mPatternIdx++;
    }
    else
    {
        if (mPatternIdx)
        {
            VPADStopMotor(0);
            mPatternIdx = 0;
        }
    }

    return false;
}

void CafeDRCPatternRumbleAddon::startRumbleImpl_()
{
}

void CafeDRCPatternRumbleAddon::stopRumbleImpl_()
{
}

} // namespace sead
