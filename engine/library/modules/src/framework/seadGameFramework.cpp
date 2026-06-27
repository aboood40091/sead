#include <controller/seadControllerMgr.h>
#include <filedevice/seadFileDeviceMgr.h>
#include <framework/seadGameFramework.h>
#include <framework/seadInfLoopCheckerThread.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadProcessMeter.h>
#include <gfx/seadGraphics.h>
#include <heap/seadExpHeap.h>
#include <resource/seadResourceMgr.h>

namespace sead {

GameFramework::GameFramework()
    : Framework()
    , mDisplayState(cDisplayState_Hide)
    , mCalcMeter("calc", Color4f::cRed)
    , mDrawMeter("draw", Color4f::cGreen)
    , mGPUMeter("waitGPU", Color4f::cMagenta)
    , mCheckerThread(nullptr)
{
}

GameFramework::~GameFramework()
{
    if (mCheckerThread != nullptr)
    {
        mCheckerThread->quitAndDestroySingleThread(false);
        delete mCheckerThread;
    }
}

void GameFramework::startDisplay()
{
    if (mDisplayState == cDisplayState_Hide)
        mDisplayState = cDisplayState_Ready;
}

void GameFramework::createSystemTasks(TaskBase* root_task, const CreateSystemTaskArg& arg)
{
    CreateSystemTaskArg base_arg;
    Framework::createSystemTasks(root_task, base_arg);

    createControllerMgr(root_task);
    createProcessMeter(root_task);
    createSeadMenuMgr(root_task);
    createHostIOMgr(root_task, arg.hostio_parameter);
    createInfLoopChecker(root_task, arg.infloop_detection_span);
}

void GameFramework::createControllerMgr(TaskBase* root_task)
{
    TaskBase::SystemMgrTaskArg sys_mgr_create_arg(&TTaskFactory<ControllerMgr>);
    sys_mgr_create_arg.parent = root_task;

    TaskBase::CreateArg create_arg = static_cast<const TaskBase::CreateArg&>(sys_mgr_create_arg);
    create_arg.instance_cb = ControllerMgr::setInstance_;

    mTaskMgr->createSingletonTaskSync<ControllerMgr>(create_arg);
}

void GameFramework::createHostIOMgr(TaskBase* root_task, HostIOMgr::Parameter* param)
{
    // The whole function is stripped in the release build
    (void)root_task;
    (void)param;
}

void GameFramework::createProcessMeter(TaskBase* root_task)
{
#if 0 // defined(SEAD_TARGET_DEBUG)
    // In debug, ProcessMeter is a singleton task, not a regular singleton
    // ...
#else
    ProcessMeter::createInstance(root_task->getHeap());
#endif
}

void GameFramework::createSeadMenuMgr(TaskBase* root_task)
{
    // The whole function is stripped in the release build
    (void)root_task;
}

void GameFramework::createInfLoopChecker(TaskBase* root_task, const TickSpan& span)
{
    // The whole function is stripped in the release build
    (void)root_task;
    (void)span;
}

void GameFramework::initialize(const InitializeArg& arg)
{
    Framework::initialize(arg);

    Heap* root_heap = HeapMgr::instance()->getRootHeap(0);

    Heap* mgrs_heap = ExpHeap::create(root_heap->getMaxAllocatableSize(), "sead::SystemManagers", root_heap);

    Heap* res_mgr_heap = ExpHeap::create(mgrs_heap->getMaxAllocatableSize(), "sead::ResourceMgr", mgrs_heap);
    {
        CurrentHeapSetter chs(res_mgr_heap);
        ResourceMgr::createInstance(res_mgr_heap);
        res_mgr_heap->adjust();
    }

    Heap* filedevice_mgr_heap = ExpHeap::create(mgrs_heap->getMaxAllocatableSize(), "sead::FileDeviceMgr", mgrs_heap);
    {
        CurrentHeapSetter chs(filedevice_mgr_heap);
        FileDeviceMgr::createInstance(filedevice_mgr_heap);
        filedevice_mgr_heap->adjust();
    }

    // HIOInit();

    mgrs_heap->adjust();
}

void GameFramework::waitStartDisplayLoop_()
{
    while (true)
    {
        Graphics::instance()->lockDrawContext();
        {
            mTaskMgr->beforeCalc();
            mTaskMgr->afterCalc();
        }
        Graphics::instance()->unlockDrawContext();

        if (mTaskMgr->getRootTask() != nullptr || mDisplayState != cDisplayState_Hide)
            break;

        TickSpan ms_10;
        ms_10.setMilliSeconds(10);
        Thread::sleep(ms_10);
    }

    mMethodTreeMgr->pauseAll(false);

    startDisplay();
}

} // namespace sead
