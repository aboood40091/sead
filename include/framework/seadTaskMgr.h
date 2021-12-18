#ifndef SEAD_TASKMGR_H_
#define SEAD_TASKMGR_H_

#include <framework/seadHeapPolicies.h>
#include <framework/seadMethodTree.h>
#include <framework/seadTaskBase.h>
#include <heap/seadHeapMgr.h>
#include <thread/seadCriticalSection.h>

namespace sead {

class DelegateThread;
class Framework;
class Heap;
class NullFaderTask;

class TaskMgr
{
public:
    struct InitializeArg
    {
      public:
        InitializeArg(const TaskBase::CreateArg& roottask_arg)
            : create_queue_size(0x20)
            , prepare_stack_size(0x8000)
            , prepare_priority(-1)
            , roottask_create_arg(roottask_arg)
            , heap(NULL)
            , parent_framework(NULL)
        {
        }

        u32 create_queue_size;
        u32 prepare_stack_size;
        s32 prepare_priority;
        const TaskBase::CreateArg& roottask_create_arg;
        Heap* heap;
        Framework* parent_framework;
    };
#ifdef cafe
    static_assert(sizeof(InitializeArg) == 0x18, "sead::TaskMgr::InitializeArg size mismatch");
#endif // cafe

    class TaskCreateContextMgr;

public:
    TaskMgr(const InitializeArg& arg);

    void appendToList_(TaskBase::List& ls, TaskBase* task);
    bool changeTaskState_(TaskBase* task, TaskBase::State state);
    void destroyTaskSync(TaskBase* task);
    void doDestroyTask_(TaskBase* task);
    void finalize();

    CriticalSection mCriticalSection;
    Framework* mParentFramework;
    DelegateThread* mPrepareThread;
    NullFaderTask* mNullFaderTask;
    TaskBase::List mPrepareList;
    TaskBase::List mPrepareDoneList;
    TaskBase::List mActiveList;
    TaskBase::List mStaticList;
    TaskBase::List mDyingList;
    TaskBase::List mDestroyableList;
    HeapArray mHeapArray;
    TaskCreateContextMgr* mTaskCreateContextMgr;
    u32 mMaxCreateQueueSize;
    TaskBase* mRootTask;
    TaskBase::CreateArg mRootTaskCreateArg;
    TaskMgr::InitializeArg mInitializeArg;
    MethodTreeNode mCalcDestructionTreeNode;
    u32 _1a0;
    u32 _1a4;
};
#ifdef cafe
static_assert(sizeof(TaskMgr) == 0x1A8, "sead::TaskMgr size mismatch");
#endif // cafe

} // namespace sead

#define SEAD_TASK_SINGLETON_DISPOSER(CLASS)                                       \
    public:                                                                       \
        static CLASS* instance() { return sInstance; }                            \
        static void setInstance_(sead::TaskBase* instance);                       \
        static void deleteInstance();                                             \
                                                                                  \
    protected:                                                                    \
        class SingletonDisposer_                                                  \
        {                                                                         \
        public:                                                                   \
            SingletonDisposer_() : mIsSetAsSingleton_(false) { }                  \
            ~SingletonDisposer_();                                                \
                                                                                  \
            bool mIsSetAsSingleton_;                                              \
        };                                                                        \
                                                                                  \
    private:                                                                      \
        CLASS(const CLASS&);                                                      \
        const CLASS& operator=(const CLASS&);                                     \
                                                                                  \
    protected:                                                                    \
        SingletonDisposer_ mSingletonDisposer_;                                   \
                                                                                  \
        static CLASS* sInstance;                                                  \
                                                                                  \
        friend class SingletonDisposer_;

#define SEAD_TASK_SET_SINGLETON_INSTANCE(CLASS)                                                                  \
    void CLASS::setInstance_(sead::TaskBase* instance)                                                           \
    {                                                                                                            \
        if (CLASS::sInstance == NULL)                                                                            \
        {                                                                                                        \
            CLASS::sInstance = static_cast<CLASS*>(instance);                                                    \
            static_cast<CLASS*>(instance)->mSingletonDisposer_.mIsSetAsSingleton_ = true;                        \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            /*SEAD_ASSERT_MSG(false, "Create Singleton Twice (%s) : addr %x", "CLASS", CLASS::sInstance);*/      \
        }                                                                                                        \
    }

#define SEAD_TASK_DELETE_SINGLETON_INSTANCE(CLASS)                                                  \
    void CLASS::deleteInstance()                                                                    \
    {                                                                                               \
        if (CLASS::sInstance != NULL)                                                               \
        {                                                                                           \
            CLASS::sInstance->mTaskMgr->destroyTaskSync(CLASS::sInstance);                          \
            CLASS::sInstance = NULL;                                                                \
        }                                                                                           \
    }

#define SEAD_TASK_SINGLETON_DISPOSER_IMPL(CLASS)                                      \
    CLASS* CLASS::sInstance = NULL;                                                   \
                                                                                      \
    SEAD_TASK_SET_SINGLETON_INSTANCE(CLASS)                                           \
    SEAD_TASK_DELETE_SINGLETON_INSTANCE(CLASS)                                        \
                                                                                      \
    CLASS::SingletonDisposer_::~SingletonDisposer_()                                  \
    {                                                                                 \
        if (mIsSetAsSingleton_)                                                       \
        {                                                                             \
            CLASS::sInstance = NULL;                                                  \
        }                                                                             \
    }

#endif // SEAD_TASKMGR_H_
