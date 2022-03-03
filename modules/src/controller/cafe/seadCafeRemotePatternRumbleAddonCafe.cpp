#include <controller/cafe/seadCafeRemoteControllerCafe.h>
#include <controller/cafe/seadCafeRemotePatternRumbleAddonCafe.h>
#include <cafe/pads/wpad/wpad.h>

namespace sead {

CafeRemotePatternRumbleAddon::CafeRemotePatternRumbleAddon(CafeRemoteController* controller)
    : PatternRumbleAddon(controller)
{
}

void CafeRemotePatternRumbleAddon::startRumbleImpl_()
{
    WPADStartMotor(static_cast<CafeRemoteController*>(mController)->getChannel());
}
void CafeRemotePatternRumbleAddon::stopRumbleImpl_()
{
    WPADStopMotor(static_cast<CafeRemoteController*>(mController)->getChannel());
}

} // namespace sead
