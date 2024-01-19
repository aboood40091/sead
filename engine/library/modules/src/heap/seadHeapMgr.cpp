#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <prim/seadScopedLock.h>
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
HeapMgr::findContainHeap(const void* memBlock) const
{
    ScopedLock<CriticalSection> lock(&sHeapTreeLockCS);

    for (RootHeaps::iterator it_end = sRootHeaps.end(), it = sRootHeaps.begin(); it != it_end; ++it)
    {
        Heap* found = it->findContainHeap_(memBlock);
        if (found != nullptr)
            return found;
    }

    for (IndependentHeaps::iterator it_end = sIndependentHeaps.end(), it = sIndependentHeaps.begin(); it != it_end; ++it)
    {
        Heap* found = it->findContainHeap_(memBlock);
        if (found != nullptr)
            return found;
    }

    return nullptr;
}

Heap*
HeapMgr::getCurrentHeap() const
{
    ThreadMgr* thread_mgr = ThreadMgr::instance();
  //SEAD_ASSERT(thread_mgr);
    return thread_mgr->getCurrentThread()->getCurrentHeap();
}

} // namespace sead
