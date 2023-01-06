#ifndef SEAD_CAFE_V_PAD_DEVICE_CAFE_H_
#define SEAD_CAFE_V_PAD_DEVICE_CAFE_H_

#include <controller/seadControlDevice.h>

#include <cafe/vpad.h>

namespace sead {

class CafeVPadDevice : public ControlDevice
{
    SEAD_RTTI_OVERRIDE(CafeVPadDevice, ControlDevice)

public:
    struct VPadInfo
    {
        VPadInfo();

        VPADStatus status[VPAD_MAX_READ_BUFS];
        s32 last_read_length;
        s32 last_read_error;
    };
    static_assert(sizeof(VPadInfo) == 0xAC8, "sead::CafeVPadDevice::VPadInfo size mismatch");

public:
    CafeVPadDevice(ControllerMgr* mgr);
    virtual ~CafeVPadDevice();

    virtual void calc();

    const VPadInfo& getVPadInfo() const
    {
        return mVPadInfo;
    }

private:
    VPadInfo mVPadInfo;
};
static_assert(sizeof(CafeVPadDevice) == 0xADC, "sead::CafeVPadDevice size mismatch");

} // namespace sead

#endif // SEAD_CAFE_V_PAD_DEVICE_CAFE_H_
