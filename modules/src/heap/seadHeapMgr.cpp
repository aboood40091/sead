#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace sead {

HeapMgr* HeapMgr::sInstancePtr = NULL;
HeapMgr HeapMgr::sInstance;

Arena* HeapMgr::sArena = NULL;
Arena HeapMgr::sDefaultArena;

CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::RootHeaps HeapMgr::sRootHeaps;
HeapMgr::IndependentHeaps HeapMgr::sIndependentHeaps;

HeapMgr::HeapMgr()
    : mAllocFailedCallback(NULL)
{
}

HeapMgr::~HeapMgr()
{
}

Heap*
HeapMgr::findContainHeap(const void* ptr) const
{
    // ScopedLock<CriticalSection>
    sHeapTreeLockCS.lock();

    for (RootHeaps::iterator it = sRootHeaps.begin(); it != sRootHeaps.end(); ++it)
    {
        Heap* heap = it->findContainHeap_(ptr);
        if (heap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return heap;
        }
    }

    for (IndependentHeaps::iterator it = sIndependentHeaps.begin(); it != sIndependentHeaps.end(); ++it)
    {
        Heap* heap = it->findContainHeap_(ptr);
        if (heap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return heap;
        }
    }

    sHeapTreeLockCS.unlock();
    return NULL;
}

} // namespace sead
