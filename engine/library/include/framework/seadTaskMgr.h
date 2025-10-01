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
class Thread;

class TaskMgr : public hostio::Node
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
            , heap(nullptr)
            , parent_framework(nullptr)
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

    static TaskMgr* initialize(const InitializeArg& arg);
    void finalize();

    Framework* getFramework() const
    {
        return mParentFramework;
    }

    Heap* getHeap()
    {
        return mHeapArray.getPrimaryHeap();
    }

    TaskBase* getRootTask() const
    {
        return mRootTask;
    }

private:
    void doInit_();
    void beginCreateRootTask_();
    void createHeap_(HeapArray* ha, const TaskBase::CreateArg& arg);

public:
    TaskBase* createTaskSync(const TaskBase::CreateArg& arg);

    template <typename T>
    T* createSingletonTaskSync(const TaskBase::CreateArg& arg);

private:
    TaskBase* doCreateTask_(const TaskBase::CreateArg& arg, HeapArray* ha);

public:
    bool requestCreateTask(const TaskBase::CreateArg& arg);

private:
    bool doRequestCreateTask_(const TaskBase::CreateArg& arg, DelegateEvent<TaskBase*>::Slot* system_slot);
    void appendToList_(TaskBase::List& ls, TaskBase* task);
    bool changeTaskState_(TaskBase* task, TaskBase::State state);

public:
    bool requestTakeover(const TaskBase::TakeoverArg& arg);
    bool requestTransition(TaskBase* from, TaskBase* to, FaderTaskBase* fader);
    bool requestPush(const TaskBase::PushArg& arg);
    TaskBase* pushSync(const TaskBase::PushArg& arg);
    bool requestPop(TaskBase* pop_task, FaderTaskBase* fader);
    bool popSync(TaskBase* pop_task);
    bool requestPop(TaskBase* pop_task, TaskBase* to_task, FaderTaskBase* fader);

private:
    void prepare_(Thread*, /* MessageQueue::Element */ s32 msg);

public:
    void destroyTaskSync(TaskBase* task);
    void requestDestroyTask(TaskBase* task, FaderTaskBase* fader);

private:
    bool destroyable_(TaskBase* task);
    void doDestroyTask_(TaskBase* task);
    void calcCreation_();
    void calcDestruction_();

public:
    void destroyAllAndCreateRoot();
    TaskBase* findTask(const TaskClassID& classID);
    void beforeCalc();
    void afterCalc();

private:
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
        if (CLASS::sInstance == nullptr)                                                                         \
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
        if (CLASS::sInstance != nullptr)                                                            \
        {                                                                                           \
            CLASS::sInstance->mTaskMgr->destroyTaskSync(CLASS::sInstance);                          \
            CLASS::sInstance = nullptr;                                                             \
        }                                                                                           \
    }

#define SEAD_TASK_SINGLETON_DISPOSER_IMPL(CLASS)                                      \
    CLASS* CLASS::sInstance = nullptr;                                                \
                                                                                      \
    SEAD_TASK_SET_SINGLETON_INSTANCE(CLASS)                                           \
    SEAD_TASK_DELETE_SINGLETON_INSTANCE(CLASS)                                        \
                                                                                      \
    CLASS::SingletonDisposer_::~SingletonDisposer_()                                  \
    {                                                                                 \
        if (mIsSetAsSingleton_)                                                       \
        {                                                                             \
            CLASS::sInstance = nullptr;                                               \
        }                                                                             \
    }

#endif // SEAD_TASKMGR_H_
