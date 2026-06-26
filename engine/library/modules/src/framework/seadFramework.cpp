#include <framework/seadFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadTaskMgr.h>
#include <heap/seadHeap.h>

namespace sead {

Framework::CreateSystemTaskArg::CreateSystemTaskArg()
    : hostio_parameter(nullptr)
    , infloop_detection_span()
{
}

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

} // namespace sead
