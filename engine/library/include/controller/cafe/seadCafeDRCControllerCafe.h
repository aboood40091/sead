#ifndef SEAD_CAFE_DRC_CONTROLLER_CAFE_H_
#define SEAD_CAFE_DRC_CONTROLLER_CAFE_H_

#include <controller/seadController.h>

namespace sead {

class CafeDRCController : public Controller
{
    SEAD_RTTI_OVERRIDE(CafeDRCController, Controller)

public:
    struct TouchPanelInfo
    {
        TouchPanelInfo()
            : pointer(cInvalidPointer)
            , flags(0)
        {
        }

        Vector2f pointer;
        BitFlag32 flags;
    };
    static_assert(sizeof(TouchPanelInfo) == 0xC, "sead::CafeDRCController::TouchPanelInfo size mismatch");

public:
    CafeDRCController(ControllerMgr* mgr);
    virtual ~CafeDRCController() { }

    virtual bool isConnected() const { return mIsConnected; }

    void setTouchPanelSamplingUseNum(u32 num);

private:
    virtual void calcImpl_();

    static void updateTouchPanelInfo_(TouchPanelInfo* tp_info, bool tp_touch_on, f32 x, f32 y);

    bool mIsConnected;
    u32 mTouchPanelSamplingUseNum;
    u32 mTimeout;
    u32 mCurrentTouchPanel;
    TouchPanelInfo mTouchPanelInfos[3];
};
static_assert(sizeof(CafeDRCController) == 0x190, "sead::CafeDRCController size mismatch");

} // namespace sead

#endif // SEAD_CAFE_DRC_CONTROLLER_CAFE_H_
