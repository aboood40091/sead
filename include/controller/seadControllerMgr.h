#ifndef SEAD_CONTROLLER_MGR_H_
#define SEAD_CONTROLLER_MGR_H_

#include <framework/seadCalculateTask.h>
#include <framework/seadTaskMgr.h>

namespace sead {

class ControlDevice;

class ControllerMgr : public CalculateTask
{
    SEAD_TASK_SINGLETON_DISPOSER(ControllerMgr)

public:
    ControllerMgr();
    ControllerMgr(const TaskConstructArg& arg);
    virtual ~ControllerMgr() { }

    virtual void prepare();
    virtual void calc();

    void initialize(s32 controller_max, Heap* heap);
    void finalize();

    void initializeDefault(Heap* heap);
    void finalizeDefault();

    Controller* getControllerByOrder(ControllerDefine::ControllerId id, s32 index) const;
    ControlDevice* getControlDevice(ControllerDefine::DeviceId id) const;
    ControllerAddon* getControllerAddon(s32 index, ControllerDefine::AddonId id) const;

    s32 findControllerPort(const Controller* controller) const;

private:
    OffsetList<ControlDevice> mDevices;
    PtrArray<Controller> mControllers;
};
#ifdef cafe
static_assert(sizeof(ControllerMgr) == 0xE8, "sead::ControllerMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CONTROLLER_MGR_H_
