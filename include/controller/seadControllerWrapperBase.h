#ifndef SEAD_CONTROLLER_WRAPPER_BASE_H_
#define SEAD_CONTROLLER_WRAPPER_BASE_H_

#include <controller/seadControllerBase.h>
#include <heap/seadDisposer.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class Controller;

class ControllerWrapperBase : public ControllerBase, public IDisposer
{
    SEAD_RTTI_BASE(ControllerWrapperBase)

public:
    ControllerWrapperBase();
    virtual ~ControllerWrapperBase();

    virtual void calc(u32, bool) = 0;
    virtual void setIdle();
    virtual bool isIdle_();

    void registerWith(Controller* controller, bool copy_repeat);
    void unregister();
    void copyRepeatSetting(const Controller* controller);
    void setEnable(bool enable);
    void setEnableOtherWrappers(bool enable) const;

    Controller* mController;
    bool mIsEnable;
    ListNode mListNode;
    u8 mPadConfig[32];
};
#ifdef cafe
static_assert(sizeof(ControllerWrapperBase) == 0x174, "sead::ControllerWrapperBase size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_WRAPPER_BASE_H_
