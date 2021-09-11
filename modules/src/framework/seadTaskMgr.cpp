#include <framework/seadFramework.h>
#include <framework/seadMethodTreeMgr.h>
#include <framework/seadTaskBase.h>
#include <framework/seadTaskMgr.h>
#include <prim/seadSafeString.h>
#include <resource/seadResourceMgr.h>
#include <thread/seadDelegateThread.h>

namespace sead {

bool TaskMgr::changeTaskState_(TaskBase* task, TaskBase::State state)
{
    mCriticalSection.lock();

    TaskBase::State curr_state = task->mState;
    if (curr_state == state)
    {
        mCriticalSection.unlock();
        return false;
    }

    switch (state)
    {
    case TaskBase::cPrepare:
        if (curr_state == TaskBase::cCreated)
        {
            task->mState = TaskBase::cPrepare;
            appendToList_(mPrepareList, task);

            if (mPrepareThread != NULL)
            {
                if (mPrepareThread->sendMessage(1, 1))
                {
                    mCriticalSection.unlock();
                    return true;
                }
                else
                {
                    //SEAD_ASSERT_MSG(false, "failed to wakeup prepare thread for[%s]", task->getName().cstr());
                    mCriticalSection.unlock();
                    return false;
                }
            }
            else
            {
                mCriticalSection.unlock();
                return true;
            }
        }

        //SEAD_ASSERT_MSG(false, "err");
        mCriticalSection.unlock();
        return false;

    case TaskBase::cPrepareDone:
        //SEAD_ASSERT(curr_state == TaskBase::cPrepare);
        task->mState = TaskBase::cPrepareDone;
        task->mTaskListNode.erase();

        mCriticalSection.unlock();
        return true;

    case TaskBase::cRunning:
        //SEAD_ASSERT(curr_state == TaskBase::cPrepareDone);
        task->mState = TaskBase::cRunning;
        task->mTaskListNode.erase();
        appendToList_(mActiveList, task);

        if (ResourceMgr::instance() != NULL)
            ResourceMgr::instance()->postCreate();

        task->enterCommon();

        mCriticalSection.unlock();
        return true;

    case TaskBase::cDying:
        task->mState = TaskBase::cDying;

        mCriticalSection.unlock();
        return true;

    case TaskBase::cDestroyable:
        if (curr_state != TaskBase::cRunning)
            break;

        task->mState = TaskBase::cDestroyable;
        task->detachCalcImpl();
        task->detachDrawImpl();
        appendToList_(mDestroyableList, task);

        mCriticalSection.unlock();
        return true;

    case TaskBase::cDead:
        task->exit();
        task->mState = TaskBase::cDead;
        task->mTaskListNode.erase();

        mCriticalSection.unlock();
        return true;

    case TaskBase::cCreated:
        //SEAD_ASSERT_MSG(false, "Cannot Change State to cCreated\n");
        break;

    case TaskBase::cSleep: // ?
    default:
        //SEAD_ASSERT_MSG(false, "Unknown State %d\n", (s32)state);
    }

    mCriticalSection.unlock();
    return false;
}

void TaskMgr::destroyTaskSync(TaskBase* task)
{
    bool b = mParentFramework->mMethodTreeMgr->mCS.tryLock();
    //SEAD_ASSERT(b);

    if (b)
    {
        doDestroyTask_(task);
        mParentFramework->mMethodTreeMgr->mCS.unlock();
    }
}

void TaskMgr::doDestroyTask_(TaskBase* task)
{
    mCriticalSection.lock();

    TreeNode* node = task->child();
    while (node != NULL)
    {
        doDestroyTask_(static_cast<TTreeNode<TaskBase*>*>(node)->val());
        node = task->child();
    }

    if (changeTaskState_(task, TaskBase::cDead))
    {
        task->detachAll();

        HeapArray heapArray(task->mHeapArray);
        for (s32 i = 0; i < HeapMgr::getRootHeapNum(); i++)
        {
            Heap* heap = heapArray.mHeaps[i];
            if (heap != NULL)
                heap->destroy();
        }
    }

    mCriticalSection.unlock();
}

void TaskMgr::finalize()
{
    if (mPrepareThread != NULL)
    {
        mPrepareThread->quitAndDestroySingleThread(false);
        delete mPrepareThread;
        mPrepareThread = NULL;
    }

    if (mRootTask != NULL)
    {
        destroyTaskSync(mRootTask);
        mRootTask = NULL;
    }

    for (s32 i = 0; i < HeapMgr::getRootHeapNum(); i++)
    {
        Heap* heap = mHeapArray.mHeaps[i];
        if (heap)
        {
            heap->destroy();
            mHeapArray.mHeaps[i] = NULL;
        }
    }
}

} // namespace sead
