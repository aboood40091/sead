#ifndef SEAD_CAFE_DEBUG_PAD_DEVICE_CAFE_H_
#define SEAD_CAFE_DEBUG_PAD_DEVICE_CAFE_H_

#include <container/seadSafeArray.h>
#include <controller/seadControlDevice.h>

#include <cafe/pad.h>

namespace sead {

class CafeDebugPadDevice : public ControlDevice
{
    SEAD_RTTI_OVERRIDE(CafeDebugPadDevice, ControlDevice)

public:
    explicit CafeDebugPadDevice(ControllerMgr* mgr);

    virtual void calc();

    const PADStatus& getPadStatus(s32 channel) const
    {
        return mPADStatuses[channel];
    }

private:
    sead::SafeArray<PADStatus, PAD_MAX_CONTROLLERS> mPADStatuses;
};
static_assert(sizeof(CafeDebugPadDevice) == 0x44, "sead::CafeDebugPadDevice size mismatch");

} // namespace sead

#endif // SEAD_CAFE_DEBUG_PAD_DEVICE_CAFE_H_
