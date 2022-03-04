#ifndef SEAD_CAFE_DRC_PATTERN_RUMBLE_ADDON_CAFE_H_
#define SEAD_CAFE_DRC_PATTERN_RUMBLE_ADDON_CAFE_H_

#include <controller/seadPatternRumbleAddon.h>
#include <time/seadTickTime.h>

#include <cafe/vpad.h>

namespace sead {

class CafeDRCController;

class CafeDRCPatternRumbleAddon : public PatternRumbleAddon
{
    SEAD_RTTI_OVERRIDE(CafeDRCPatternRumbleAddon, PatternRumbleAddon)

public:
    CafeDRCPatternRumbleAddon(CafeDRCController* controller);
    virtual ~CafeDRCPatternRumbleAddon() { }

    virtual bool calc();

private:
    virtual void startRumbleImpl_();
    virtual void stopRumbleImpl_();

    u8 mPatternBuf[VPAD_MOTOR_PATTERN_SIZE_MAX];
    u32 mPatternLength;
    TickTime mEndTime;
};
static_assert(sizeof(CafeDRCPatternRumbleAddon) == 0x40, "sead::CafeDRCPatternRumbleAddon size mismatch");

} // namespace sead

#endif // SEAD_CAFE_DRC_PATTERN_RUMBLE_ADDON_CAFE_H_
