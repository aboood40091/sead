#ifndef SEAD_CAFE_REMOTE_CONTROLLER_CAFE_H_
#define SEAD_CAFE_REMOTE_CONTROLLER_CAFE_H_

#include <controller/seadController.h>

namespace sead {

class CafeRemoteController : public Controller
{
    SEAD_RTTI_OVERRIDE(CafeRemoteController, Controller)

public:
    CafeRemoteController(ControllerMgr* mgr, s32 channel);
    virtual ~CafeRemoteController() { }

    virtual bool isConnected() const { return mIsConnected; }

    s32 getChannel() const { return mChannel; }

    u8 getCurrentDeviceType() const { return mDeviceTypeCurrent; }
    u8 getLastConnectedDeviceType() const { return mDeviceTypeLastConnected; }

    bool isEnableCL() const { return mIsEnableCL; }
    void setEnableCL(bool enable) { mIsEnableCL = enable; }

private:
    virtual void calcImpl_();

    s32 mChannel;
    u8 mDeviceTypeCurrent;
    u8 mDeviceTypeLastConnected;
    bool mIsConnected;
    bool mIsEnableCL;
};
static_assert(sizeof(CafeRemoteController) == 0x164, "sead::CafeRemoteController size mismatch");

} // namespace sead

#endif // SEAD_CAFE_REMOTE_CONTROLLER_CAFE_H_
