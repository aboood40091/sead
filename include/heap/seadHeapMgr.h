#ifndef SEAD_HEAPMGR_H_
#define SEAD_HEAPMGR_H_

#include <container/seadPtrArray.h>
#include <heap/seadArena.h>
#include <heap/seadHeap.h>
#include <thread/seadCriticalSection.h>

#define NUM_ROOT_HEAPS_MAX         4
#define NUM_INDEPENDENT_HEAPS_MAX  4

namespace sead {

class HeapMgr
{
public:
    HeapMgr();
    virtual ~HeapMgr() { }

    Heap* getCurrentHeap();
    Heap* findContainHeap(const void* ptr) const;

    static HeapMgr sInstance;
    static HeapMgr* sInstancePtr;

    static Arena sDefaultArena;
    static CriticalSection sHeapTreeLockCS;

    typedef FixedPtrArray<Heap,        NUM_ROOT_HEAPS_MAX> RootHeaps;
    typedef FixedPtrArray<Heap, NUM_INDEPENDENT_HEAPS_MAX> IndependentHeaps;

    static RootHeaps sRootHeaps;
    static IndependentHeaps sIndependentHeaps;

    void* mAllocFailedCallback;  // IAllocFailedCallback* = IDelegate1<const AllocFailedCallbackArg*>*
};

} // namespace sead

#undef NUM_ROOT_HEAPS_MAX
#undef NUM_INDEPENDENT_HEAPS_MAX

#endif // SEAD_HEAPMGR_H_
