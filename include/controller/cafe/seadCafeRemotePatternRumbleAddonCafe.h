#ifndef SEAD_CAFE_REMOTE_PATTERN_RUMBLE_ADDON_CAFE_H_
#define SEAD_CAFE_REMOTE_PATTERN_RUMBLE_ADDON_CAFE_H_

#include <controller/seadPatternRumbleAddon.h>

namespace sead {

class CafeRemoteController;

class CafeRemotePatternRumbleAddon : public PatternRumbleAddon
{
    SEAD_RTTI_OVERRIDE(CafeRemotePatternRumbleAddon, PatternRumbleAddon)

public:
    CafeRemotePatternRumbleAddon(CafeRemoteController* controller);
    virtual ~CafeRemotePatternRumbleAddon() { }

private:
    virtual void startRumbleImpl_();
    virtual void stopRumbleImpl_();
};
static_assert(sizeof(CafeRemotePatternRumbleAddon) == 0x20, "sead::CafeRemotePatternRumbleAddon size mismatch");

} // namespace sead

#endif // SEAD_CAFE_REMOTE_PATTERN_RUMBLE_ADDON_CAFE_H_
