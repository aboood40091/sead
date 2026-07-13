#include <controller/cafe/seadCafeDebugPadDeviceCafe.h>

namespace sead {

CafeDebugPadDevice::CafeDebugPadDevice(ControllerMgr* mgr)
    : ControlDevice(mgr)
{
    mId = ControllerDefine::cDevice_CafeDebugPad;

    PADInit();

    for (sead::SafeArray<PADStatus, PAD_MAX_CONTROLLERS>::iterator itr_end = mPADStatuses.end(), itr = mPADStatuses.begin(); itr != itr_end; ++itr)
        itr->err = PAD_ERR_NO_CONTROLLER;
}

void CafeDebugPadDevice::calc()
{
    PADRead(mPADStatuses.getBufferPtr());
    PADClampCircle(mPADStatuses.getBufferPtr());
}

} // namespace sead
