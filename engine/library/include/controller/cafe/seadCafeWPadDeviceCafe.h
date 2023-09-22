#ifndef SEAD_CAFE_W_PAD_DEVICE_CAFE_H_
#define SEAD_CAFE_W_PAD_DEVICE_CAFE_H_

#include <container/seadSafeArray.h>
#include <controller/seadControlDevice.h>
#include <prim/seadMemUtil.h>

#include <cafe/pads/kpad/kpad.h>

namespace sead {

class Heap;

class CafeWPadDevice : public ControlDevice
{
    SEAD_RTTI_OVERRIDE(CafeWPadDevice, ControlDevice)

public:
    struct KPadInfo
    {
        KPadInfo()
            : last_read_length(0)
            , last_read_error(KPAD_READ_ERR_NO_CONTROLLER)
        {
            MemUtil::fillZero(status, sizeof(KPADStatus) * KPAD_MAX_READ_BUFS);
            for (u32 i = 0; i < KPAD_MAX_READ_BUFS; i++)
                status[i].dev_type = WPAD_DEV_NOT_FOUND;
        }

        KPADStatus status[KPAD_MAX_READ_BUFS];
        s32 last_read_length;
        s32 last_read_error;
    };
    static_assert(sizeof(KPadInfo) == 0xF08, "sead::CafeWPadDevice::KPadInfo size mismatch");

public:
    CafeWPadDevice(ControllerMgr* mgr, Heap* heap);
    virtual ~CafeWPadDevice();

    virtual void calc();

    const KPadInfo& getKPadInfo(s32 channel) const
    {
        return mKPadInfos[channel];
    }

    void setFreezeCounter(s32 value)
    {
        mFreezeCounter = value;
    }

private:
    KPADUnifiedWpadStatus mUniRingBufs[WPAD_MAX_CONTROLLERS * KPAD_MAX_READ_BUFS];
    u8 _1114[4];
    SafeArray<KPadInfo, WPAD_MAX_CONTROLLERS> mKPadInfos;
    s32 mFreezeCounter;
    u8 _4d3c[4];
};
static_assert(sizeof(CafeWPadDevice) == 0x4D40, "sead::CafeWPadDevice size mismatch");

} // namespace sead

#endif // SEAD_CAFE_W_PAD_DEVICE_CAFE_H_
