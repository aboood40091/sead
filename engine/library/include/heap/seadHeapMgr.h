#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

#include <container/seadPtrArray.h>
#include <heap/seadArena.h>
#include <thread/seadCriticalSection.h>

#define NUM_ROOT_HEAPS_MAX         4
#define NUM_INDEPENDENT_HEAPS_MAX  4

namespace sead {

class CurrentHeapSetter;
class Heap;

template <typename A>
class IDelegate1;

class HeapMgr
{
public:
  //struct AllocCallbackArg;
    struct AllocFailedCallbackArg;
  //struct FreeCallbackArg;
  //struct CreateCallbackArg;
  //struct DestroyCallbackArg;

private:
  //typedef IDelegate1<const AllocCallbackArg*>         IAllocCallback;
    typedef IDelegate1<const AllocFailedCallbackArg*>   IAllocFailedCallback;
  //typedef IDelegate1<const FreeCallbackArg*>          IFreeCallback;
  //typedef IDelegate1<const CreateCallbackArg*>        ICreateCallback;
  //typedef IDelegate1<const DestroyCallbackArg*>       IDestroyCallback;

    typedef FixedPtrArray<Heap,        NUM_ROOT_HEAPS_MAX> RootHeaps;
    typedef FixedPtrArray<Heap, NUM_INDEPENDENT_HEAPS_MAX> IndependentHeaps;

public:
    HeapMgr();
    virtual ~HeapMgr();

    static void initialize(u32 size);
    static void initialize(Arena* arena);
    static void destroy();

  //void initHostIO();

    static bool isInitialized() { return sInstancePtr != nullptr; }
    static HeapMgr* instance() { return sInstancePtr; }

    Heap* findContainHeap(const void* ptr) const;
    Heap* getCurrentHeap() const;

    static Heap* getRootHeap(s32 n);
    static s32 getRootHeapNum() { return sRootHeaps.size(); }
    static void addRootHeap(Heap* heap);
    static const Arena* getArena();
    static IndependentHeaps* getIndependentHeaps();
    static bool isContainedInAnyHeap(const void* ptr) { return sInstancePtr->findContainHeap(ptr) != nullptr; }

  //IAllocCallback* setAllocCallback(IAllocCallback*);
  //IAllocCallback* getAllocCallback();
    IAllocFailedCallback* setAllocFailedCallback(IAllocFailedCallback* callback);
    IAllocFailedCallback* getAllocFailedCallback();
  //IFreeCallback* setFreeCallback(IFreeCallback*);
  //IFreeCallback* getFreeCallback();
  //ICreateCallback* setCreateCallback(ICreateCallback*);
  //ICreateCallback* getCreateCallback();
  //IDestroyCallback* setDestroyCallback(IDestroyCallback*);
  //IDestroyCallback* getDestroyCallback();
    void callCreateCallback_(Heap*) { }
  //void callDestroyCallback_(Heap*);
  //void callFreeCallback_(const FreeCallbackArg&);

    static CriticalSection* getHeapTreeLockCS_() { return &sHeapTreeLockCS; }

protected:
    Heap* setCurrentHeap_(Heap* heap);

    friend class CurrentHeapSetter;

    static void createRootHeap_();
    static void initializeImpl_();

protected:
  //IAllocCallback* mAllocCallback;
    IAllocFailedCallback* mAllocFailedCallback;
  //IFreeCallback* mFreeCallback;
  //ICreateCallback* mCreateCallback;
  //IDestroyCallback* mDestroyCallback;

    static HeapMgr sInstance;
    static HeapMgr* sInstancePtr;
    static Arena* sArena;
    static Arena sDefaultArena;
    static RootHeaps sRootHeaps;
    static CriticalSection sHeapTreeLockCS;
    static IndependentHeaps sIndependentHeaps;
};
#ifdef cafe
static_assert(sizeof(HeapMgr) == 8, "sead::HeapMgr size mismatch");
#endif // cafe

class CurrentHeapSetter
{
public:
    CurrentHeapSetter(Heap* heap)
        : mPrevHeap(nullptr)
    {
        if (heap == nullptr)
            mPrevHeap = reinterpret_cast<Heap*>(1);

        else
            mPrevHeap = HeapMgr::instance()->setCurrentHeap_(heap);
    }

    ~CurrentHeapSetter()
    {
        if (mPrevHeap != reinterpret_cast<Heap*>(1))
            static_cast<void>(HeapMgr::instance()->setCurrentHeap_(mPrevHeap));
    }

private:
    Heap* mPrevHeap;
};
static_assert(sizeof(CurrentHeapSetter) == 4, "sead::CurrentHeapSetter size mismatch");

} // namespace sead

#undef NUM_ROOT_HEAPS_MAX
#undef NUM_INDEPENDENT_HEAPS_MAX

#endif // SEAD_HEAPMGR_H_
