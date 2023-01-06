#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <thread/seadThread.h>

namespace sead {

HeapMgr* HeapMgr::sInstancePtr = nullptr;
HeapMgr HeapMgr::sInstance;

Arena* HeapMgr::sArena = nullptr;
Arena HeapMgr::sDefaultArena;

CriticalSection HeapMgr::sHeapTreeLockCS;

HeapMgr::RootHeaps HeapMgr::sRootHeaps;
HeapMgr::IndependentHeaps HeapMgr::sIndependentHeaps;

HeapMgr::HeapMgr()
    : mAllocFailedCallback(nullptr)
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
        if (heap != nullptr)
        {
            sHeapTreeLockCS.unlock();
            return heap;
        }
    }

    for (IndependentHeaps::iterator it = sIndependentHeaps.begin(); it != sIndependentHeaps.end(); ++it)
    {
        Heap* heap = it->findContainHeap_(ptr);
        if (heap != nullptr)
        {
            sHeapTreeLockCS.unlock();
            return heap;
        }
    }

    sHeapTreeLockCS.unlock();
    return nullptr;
}

Heap*
HeapMgr::getCurrentHeap() const
{
    return ThreadMgr::instance()->getCurrentThread()->getCurrentHeap();
}

} // namespace sead
