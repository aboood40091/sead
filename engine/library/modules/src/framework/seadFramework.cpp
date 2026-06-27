#include <framework/seadFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadTaskMgr.h>
#include <heap/seadExpHeap.h>
#include <mc/seadCoreInfo.h>
#include <random/seadGlobalRandom.h>
#include <thread/seadThread.h>

namespace sead {

Framework::Framework()
    : mReserveReset(false)
    , mResetParameter(nullptr)
    , mResetEvent()
    , mTaskMgr(nullptr)
    , mMethodTreeMgr(nullptr)
    , mMethodTreeMgrHeap(nullptr)
{
}

Framework::~Framework()
{
    if (mTaskMgr != nullptr)
    {
        mTaskMgr->finalize();
        delete mTaskMgr;
        mTaskMgr = nullptr;
    }

    if (mMethodTreeMgr != nullptr)
    {
        delete mMethodTreeMgr;
        mMethodTreeMgr = nullptr;
    }

    if (mMethodTreeMgrHeap != nullptr)
        mMethodTreeMgrHeap->destroy();
}

Framework::InitializeArg::InitializeArg()
    : heap_size(0x3000000)  // 48 MiB
    , arena(nullptr)
{
}

Framework::RunArg::RunArg()
    : prepare_stack_size(0)
    , prepare_priority(-1)
{
}

void Framework::initialize(const InitializeArg& arg)
{
    if (arg.arena)
        HeapMgr::initialize(arg.arena);
    else
        HeapMgr::initialize(arg.heap_size);

    Heap* root_heap = HeapMgr::instance()->getRootHeap(0);

    Heap* thread_mgr_heap = ExpHeap::create(0, "sead::ThreadMgr", root_heap);
    ThreadMgr::createInstance(thread_mgr_heap);
    ThreadMgr::instance()->initialize(thread_mgr_heap);
    thread_mgr_heap->adjust();

    GlobalRandom::createInstance(root_heap);

    CoreInfo::configure();
}

void Framework::run(Heap* heap, const TaskBase::CreateArg& root_create_arg, const RunArg& run_arg)
{
    initRun_(heap);

    Heap* method_tree_mgr_heap = ExpHeap::create(0, "sead::MethodTreeMgr", heap);
    mMethodTreeMgr = createMethodTreeMgr_(method_tree_mgr_heap);
    method_tree_mgr_heap->adjust();
    mMethodTreeMgrHeap = method_tree_mgr_heap;

    TaskMgr::InitializeArg task_mgr_init_arg(root_create_arg);
    task_mgr_init_arg.heap = heap;
    task_mgr_init_arg.parent_framework = this;
    if (run_arg.prepare_stack_size != 0)
        task_mgr_init_arg.prepare_stack_size = run_arg.prepare_stack_size;
    if (run_arg.prepare_priority != -1)
        task_mgr_init_arg.prepare_priority = run_arg.prepare_priority;
    mTaskMgr = TaskMgr::initialize(task_mgr_init_arg);

    runImpl_();
}

void Framework::createSystemTasks(TaskBase* root_task, const CreateSystemTaskArg& arg)
{
}

void Framework::procReset_()
{
    if (mReserveReset)
    {
        mResetEvent.fire(mResetParameter);
        mTaskMgr->destroyAllAndCreateRoot();
        mReserveReset = false;
        mResetParameter = nullptr;
    }
}

} // namespace sead
