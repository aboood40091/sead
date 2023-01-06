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

    u8 getInternalDeviceType() const { return mDeviceTypeInternal; }
    u8 getDeviceType() const { return mDeviceType; }

    bool isEnableCL() const { return mIsEnableCL; }
    void setEnableCL(bool enable) { mIsEnableCL = enable; }

private:
    virtual void calcImpl_();

    s32 mChannel;
    u8 mDeviceTypeInternal;
    u8 mDeviceType;
    bool mIsConnected;
    bool mIsEnableCL;
};
static_assert(sizeof(CafeRemoteController) == 0x164, "sead::CafeRemoteController size mismatch");

} // namespace sead

#endif // SEAD_CAFE_REMOTE_CONTROLLER_CAFE_H_
