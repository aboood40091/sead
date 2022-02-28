#include <basis/seadNew.h>
#include <controller/seadControlDevice.h>
#include <controller/seadController.h>
#include <controller/seadControllerMgr.h>
#include <prim/seadDelegate.h>

#ifdef cafe
#include <controller/cafe/seadCafeWPadDeviceCafe.h>
#endif // cafe

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
    initialize(
#ifdef cafe
        6,
#else
#error "Unknown Platform"
#endif
        heap
    );

#ifdef cafe
    // TODO: CafeDebugPadDevice, CafeDebugController,
    //       CafeWPadDevice, CafeRemoteController, CafeRemotePatternRumbleAddon
    //       CafeVPadDevice, CafeDRCController, CafeDRCPatternRumbleAddon

    //mDevices.pushBack(new (heap) CafeDebugPadDevice(this));
    //{
    //    mControllers.pushBack(new (heap) CafeDebugController(this, 0));
    //}

    mDevices.pushBack(new (heap) CafeWPadDevice(this, heap));
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
#endif // cafe
}

void ControllerMgr::finalizeDefault()
{
#ifdef cafe
    // TODO: CafeDebugPadDevice, CafeDebugController,
    //       CafeWPadDevice, CafeRemoteController, CafeRemotePatternRumbleAddon
    //       CafeVPadDevice, CafeDRCController, CafeDRCPatternRumbleAddon

    // ...
#endif // cafe

    finalize();
}

Controller* ControllerMgr::getControllerByOrder(ControllerDefine::ControllerId id, s32 index) const
{
    for (PtrArray<Controller>::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
    {
        Controller& controller = *it;
        if (controller.mId == id)
        {
            if (index == 0)
                return &controller;

            index--;
        }
    }

    return NULL;
}

ControlDevice* ControllerMgr::getControlDevice(ControllerDefine::DeviceId id) const
{
    for (OffsetList<ControlDevice>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        ControlDevice& device = *it;
        if (device.mId == id)
            return &device;
    }

    return NULL;
}

ControllerAddon* ControllerMgr::getControllerAddon(s32 index, ControllerDefine::AddonId id) const
{
    Controller* controller = mControllers.at(index);
    if (controller)
        return controller->getAddon(id);

    return NULL;
}

template <typename T>
T ControllerMgr::getControllerByOrderAs(s32 index) const
{
    for (PtrArray<Controller>::iterator it = mControllers.begin(); it != mControllers.end(); ++it)
    {
        T controller = DynamicCast<remove_pointer<T>::type>(&(*it));
        if (controller)
        {
            if (index == 0)
                return controller;

            index--;
        }
    }

    return NULL;
}

template <typename T>
T ControllerMgr::getControlDeviceAs() const
{
    for (OffsetList<ControlDevice>::iterator it = mDevices.begin(); it != mDevices.end(); ++it)
    {
        T device = DynamicCast<remove_pointer<T>::type>(&(*it));
        if (device)
            return device;
    }

    return NULL;
}

template <typename T>
T ControllerMgr::getControllerAddonAs(s32 index) const
{
    Controller* controller = mControllers.at(index);
    if (controller)
        return controller->getAddonAs<T>();

    return NULL;
}

} // namespace sead
