#include <basis/seadNew.h>
#include <controller/cafe/seadCafeWPadDeviceCafe.h>

namespace sead {

CafeWPadDevice::CafeWPadDevice(ControllerMgr* mgr, Heap* heap)
    : ControlDevice(mgr)
    , mFreezeCounter(0)
{
    mId = ControllerDefine::cDevice_CafeWPad;

    KPADInitEx(mUniRingBufs, WPAD_MAX_CONTROLLERS * KPAD_MAX_READ_BUFS);
    KPADSetMplsWorkarea(new (heap) u8[KPADGetMplsWorkSize()]);
}

CafeWPadDevice::~CafeWPadDevice()
{
    KPADShutdown();
}

void CafeWPadDevice::calc()
{
    for (u32 i = 0; i < WPAD_MAX_CONTROLLERS; i++)
    {
        if (mFreezeCounter > 0)
        {
            KPADStatus status;
            s32 err;
            (void)KPADReadEx(
                i,
                &status,
                1,
                &err
            );
        }
        else
        {
            mKPadInfos[i].last_read_length = KPADReadEx(
                i,
                mKPadInfos[i].status,
                KPAD_MAX_READ_BUFS,
                &mKPadInfos[i].last_read_error
            );
        }
    }

    if (mFreezeCounter > 0)
        mFreezeCounter--;
}

} // namespace sead
