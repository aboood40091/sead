#include <basis/seadNew.h>
#include <controller/cafe/seadCafeWPadDeviceCafe.h>
#include <prim/seadMemUtil.h>

namespace sead {

CafeWPadDevice::KPadInfo::KPadInfo()
    : last_read_length(0)
    , last_read_error(KPAD_READ_ERR_NO_CONTROLLER)
{
    MemUtil::fillZero(status, sizeof(KPADStatus) * KPAD_MAX_READ_BUFS);
    for (u32 i = 0; i < KPAD_MAX_READ_BUFS; i++)
        status[i].dev_type = WPAD_DEV_NOT_FOUND;
}

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
    for (sead::SafeArray<KPadInfo, WPAD_MAX_CONTROLLERS>::iterator itr_end = mKPadInfos.end(), itr = mKPadInfos.begin(); itr != itr_end; ++itr)
    {
        if (mFreezeCounter > 0)
        {
            KPADStatus status;
            s32 err;
            (void)KPADReadEx(
                itr.getIndex(),
                &status,
                1,
                &err
            );
        }
        else
        {
            itr->last_read_length = KPADReadEx(
                itr.getIndex(),
                itr->status,
                KPAD_MAX_READ_BUFS,
                &itr->last_read_error
            );
        }
    }

    if (mFreezeCounter > 0)
        mFreezeCounter--;
}

} // namespace sead
