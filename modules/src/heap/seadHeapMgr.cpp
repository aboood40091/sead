#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace sead {

HeapMgr* HeapMgr::sInstancePtr = NULL;

HeapMgr HeapMgr::sInstance;
Arena HeapMgr::sDefaultArena;
CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::RootHeaps HeapMgr::sRootHeaps;
HeapMgr::IndependentHeaps HeapMgr::sIndependentHeaps;

HeapMgr::HeapMgr()
    : mAllocFailedCallback(NULL)
{
}

Heap*
HeapMgr::findContainHeap(const void* ptr) const
{
    sHeapTreeLockCS.lock();

    for (RootHeaps::iterator it = sRootHeaps.begin(); it != sRootHeaps.end(); ++it)
    {
        Heap* containHeap = it->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    for (IndependentHeaps::iterator it = sIndependentHeaps.begin(); it != sIndependentHeaps.end(); ++it)
    {
        Heap* containHeap = it->findContainHeap_(ptr);
        if (containHeap != NULL)
        {
            sHeapTreeLockCS.unlock();
            return containHeap;
        }
    }

    sHeapTreeLockCS.unlock();
    return NULL;
}

} // namespace sead
