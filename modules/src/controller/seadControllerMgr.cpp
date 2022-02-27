#include <controller/seadControlDevice.h>
#include <controller/seadControllerMgr.h>
#include <prim/seadDelegate.h>

namespace sead {

SEAD_TASK_SINGLETON_DISPOSER_IMPL(ControllerMgr)

ControllerMgr::ControllerMgr()
    : CalculateTask(ConstructArg(), "sead::ControllerMgr")
{
    mDevices.initOffset(offsetof(ControlDevice, mListNode));
}

ControllerMgr::ControllerMgr(const TaskConstructArg& arg)
    : CalculateTask(arg, "sead::ControllerMgr")
{
    mDevices.initOffset(offsetof(ControlDevice, mListNode));
}

void ControllerMgr::prepare()
{
    Parameter* parameter = DynamicCast<Parameter>(mParameter);
    if (parameter)
    {
        initialize(parameter->controllerMax, NULL);
        if (parameter->proc)
            parameter->proc->invoke(this);
    }
    else
    {
        initializeDefault(NULL);
    }
}

void ControllerMgr::initialize(s32 controller_max, Heap* heap)
{
    mControllers.allocBuffer(controller_max, heap);
}

void ControllerMgr::finalize()
{
    mControllers.freeBuffer();
}

void ControllerMgr::initializeDefault(Heap* heap)
{
    initialize(6, heap);

    // TODO: CafeDebugPadDevice, CafeDebugController,
    //       CafeWPadDevice, CafeRemoteController, CafeRemotePatternRumbleAddon
    //       CafeVPadDevice, CafeDRCController, CafeDRCPatternRumbleAddon

    //mDevices.pushBack(new (heap) CafeDebugPadDevice(this));
    //{
    //    mControllers.pushBack(new (heap) CafeDebugController(this, 0));
    //}

    //mDevices.pushBack(new (heap) CafeWPadDevice(this, heap));
    //for (u32 i = 0; i < 4; i++)
    //{
    //    Controller* controller = new (heap) CafeRemoteController(this, i);
    //    controller->mAddons.pushBack(new (heap) CafeRemotePatternRumbleAddon(controller));
    //    mControllers.pushBack(controller);
    //}

    //mDevices.pushBack(new (heap) CafeVPadDevice(this));
    //{
    //    Controller* controller = new (heap) CafeDRCController(this);
    //    controller->mAddons.pushBack(new (heap) CafeDRCPatternRumbleAddon(controller));
    //    mControllers.pushBack(controller);
    //}
}

void ControllerMgr::finalizeDefault()
{
    // TODO: CafeDebugPadDevice, CafeDebugController,
    //       CafeWPadDevice, CafeRemoteController, CafeRemotePatternRumbleAddon
    //       CafeVPadDevice, CafeDRCController, CafeDRCPatternRumbleAddon

    // ...

    finalize();
}

} // namespace sead
