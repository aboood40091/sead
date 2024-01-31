#include <framework/seadFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadTaskBase.h>
#include <framework/seadTaskMgr.h>
#include <prim/seadSafeString.h>
#include <prim/seadScopedLock.h>
#include <resource/seadResourceMgr.h>
#include <thread/seadDelegateThread.h>

namespace sead {

bool TaskMgr::changeTaskState_(TaskBase* task, TaskBase::State state)
{
    ScopedLock<CriticalSection> lock(&mCriticalSection);

    TaskBase::State curr_state = task->mState;
    if (curr_state == state)
        return false;

    switch (state)
    {
    case TaskBase::cPrepare:
        if (curr_state == TaskBase::cCreated)
        {
            task->mState = TaskBase::cPrepare;
            appendToList_(mPrepareList, task);

            if (mPrepareThread != nullptr)
            {
                if (mPrepareThread->sendMessage(1, MessageQueue::cNoBlock))
                    return true;

                else
                {
                    //SEAD_ASSERT_MSG(false, "failed to wakeup prepare thread for[%s]", task->getName().cstr());
                    return false;
                }
            }
            else
            {
                return true;
            }
        }

        //SEAD_ASSERT_MSG(false, "err");
        return false;

    case TaskBase::cPrepareDone:
        //SEAD_ASSERT(curr_state == TaskBase::cPrepare);
        task->mState = TaskBase::cPrepareDone;
        task->mTaskListNode.erase();

        return true;

    case TaskBase::cRunning:
        //SEAD_ASSERT(curr_state == TaskBase::cPrepareDone);
        task->mState = TaskBase::cRunning;
        task->mTaskListNode.erase();
        appendToList_(mActiveList, task);

        if (ResourceMgr::instance() != nullptr)
            ResourceMgr::instance()->postCreate();

        task->enterCommon();

        return true;

    case TaskBase::cDying:
        task->mState = TaskBase::cDying;

        return true;

    case TaskBase::cDestroyable:
        if (curr_state == TaskBase::cRunning)
        {
            task->mState = TaskBase::cDestroyable;
            task->detachCalcDraw();
            appendToList_(mDestroyableList, task);

            return true;
        }
        else
        {
            break;
        }

    case TaskBase::cDead:
        task->exit();
        task->mState = TaskBase::cDead;
        task->mTaskListNode.erase();

        return true;

    case TaskBase::cCreated:
        //SEAD_ASSERT_MSG(false, "Cannot Change State to cCreated\n");
        break;

    case TaskBase::cSleep: // ?
    default:
        //SEAD_ASSERT_MSG(false, "Unknown State %d\n", (s32)state);
    }

    return false;
}

void TaskMgr::destroyTaskSync(TaskBase* task)
{
    bool b = getFramework()->getMethodTreeMgr()->getTreeCriticalSection()->tryLock();
    //SEAD_ASSERT(b);

    if (b)
    {
        doDestroyTask_(task);
        getFramework()->getMethodTreeMgr()->getTreeCriticalSection()->unlock();
    }
}

void TaskMgr::doDestroyTask_(TaskBase* task)
{
    ScopedLock<CriticalSection> lock(&mCriticalSection);

    while (task->child() != nullptr)
        doDestroyTask_(task->child()->val());

    if (changeTaskState_(task, TaskBase::cDead))
    {
        task->detachAll();

        HeapArray ha(task->mHeapArray);
        s32 num_root_heaps = HeapMgr::getRootHeapNum();
        for (s32 i = 0; i < num_root_heaps; i++)
        {
            Heap* heap = ha.mHeaps[i];
            if (heap != nullptr)
                heap->destroy();
        }
    }
}

void TaskMgr::finalize()
{
    if (mPrepareThread != nullptr)
    {
        mPrepareThread->quitAndDestroySingleThread(false);
        delete mPrepareThread;
        mPrepareThread = nullptr;
    }

    if (mRootTask != nullptr)
    {
        destroyTaskSync(mRootTask);
        mRootTask = nullptr;
    }

    s32 num_root_heaps = HeapMgr::getRootHeapNum();
    for (s32 i = 0; i < num_root_heaps; i++)
    {
        Heap* heap = mHeapArray.mHeaps[i];
        if (heap)
        {
            heap->destroy();
            mHeapArray.mHeaps[i] = nullptr;
        }
    }
}

} // namespace sead
