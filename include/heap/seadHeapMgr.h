#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

#include <container/seadPtrArray.h>
#include <heap/seadArena.h>
#include <thread/seadCriticalSection.h>

#define NUM_ROOT_HEAPS_MAX         4
#define NUM_INDEPENDENT_HEAPS_MAX  4

namespace sead {

class Heap;

class HeapMgr
{
public:
    struct AllocCallbackArg;
    struct AllocFailedCallbackArg;
    struct FreeCallbackArg;
    struct CreateCallbackArg;
    struct DestroyCallbackArg;

private:
    typedef void IAllocCallback;       //typedef IDelegate1<const AllocCallbackArg*> IAllocCallback;
    typedef void IAllocFailedCallback; //typedef IDelegate1<const AllocFailedCallbackArg*> IAllocFailedCallback;
    typedef void IFreeCallback;        //typedef IDelegate1<const FreeCallbackArg*> IFreeCallback;
    typedef void ICreateCallback;      //typedef IDelegate1<const CreateCallbackArg*> ICreateCallback;
    typedef void IDestroyCallback;     //typedef IDelegate1<const DestroyCallbackArg*> IDestroyCallback;

    typedef FixedPtrArray<Heap,        NUM_ROOT_HEAPS_MAX> RootHeaps;
    typedef FixedPtrArray<Heap, NUM_INDEPENDENT_HEAPS_MAX> IndependentHeaps;

public:
    HeapMgr();
    virtual ~HeapMgr();

    static void initialize(u32 size);
    static void initialize(Arena* arena);
    static void destroy();
    //void initHostIO();
    static bool isInitialized() { return sInstancePtr != NULL; }
    static HeapMgr* instance() { return sInstancePtr; }
    Heap* findContainHeap(const void* ptr) const;
    Heap* getCurrentHeap() const;
    static Heap* getRootHeap(s32 n);
    static s32 getRootHeapNum() { return sRootHeaps.size(); }
    static void addRootHeap(Heap* heap);
    static const Arena* getArena();
    static RootHeaps* getIndependentHeaps();
    static bool isContainedInAnyHeap(const void* ptr) { return sInstancePtr->findContainHeap(ptr) != NULL; }
    //void setDebugFillHeapCreate(u8);
    //void setDebugFillAlloc(u8);
    //void setDebugFillFree(u8);
    //void setDebugFillHeapDestroy(u8);
    //u8 getDebugFillHeapCreate() const;
    //u8 getDebugFillAlloc() const;
    //u8 getDebugFillFree() const;
    //u8 getDebugFillHeapDestroy() const;
    //void setEnableDebugFillHeapCreate(bool);
    //bool isEnableDebugFillHeapCreate();
    //void setEnableDebugFillAlloc(bool);
    //bool isEnableDebugFillAlloc();
    //void setEnableDebugFillFree(bool);
    //bool isEnableDebugFillFree();
    //void setEnableDebugFillHeapDestroy(bool);
    //bool isEnableDebugFillHeapDestroy();
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
    //void callCreateCallback_(Heap*);
    //void callDestroyCallback_(Heap*);
    //void callFreeCallback_(const FreeCallbackArg&);
    static CriticalSection* getHeapTreeLockCS_() { return &sHeapTreeLockCS; }

protected:
    Heap* setCurrentHeap_(Heap* heap);
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

} // namespace sead

#undef NUM_ROOT_HEAPS_MAX
#undef NUM_INDEPENDENT_HEAPS_MAX

#endif // SEAD_HEAPMGR_H_
