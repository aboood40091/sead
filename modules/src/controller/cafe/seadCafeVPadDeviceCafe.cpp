#include <controller/cafe/seadCafeVPadDeviceCafe.h>
#include <prim/seadMemUtil.h>

namespace sead {

CafeVPadDevice::VPadInfo::VPadInfo()
    : last_read_length(0)
    , last_read_error(VPAD_READ_ERR_NO_CONTROLLER)
{
    MemUtil::fillZero(status, sizeof(VPADStatus) * VPAD_MAX_READ_BUFS);
}

CafeVPadDevice::CafeVPadDevice(ControllerMgr* mgr)
    : ControlDevice(mgr)
{
    mId = ControllerDefine::cDevice_CafeVPad;
}

CafeVPadDevice::~CafeVPadDevice()
{
}

void CafeVPadDevice::calc()
{
    mVPadInfo.last_read_length = VPADRead(0, mVPadInfo.status, VPAD_MAX_READ_BUFS, &mVPadInfo.last_read_error);
}

} // namespace sead
