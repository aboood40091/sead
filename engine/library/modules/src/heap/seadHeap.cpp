#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <prim/seadScopedLock.h>

namespace sead {

void Heap::appendDisposer_(IDisposer* o)
{
    ConditionalScopedLock<CriticalSection> lock(&mCS, mFlag.isOnBit(0));
    mDisposerList.pushBack(o);
}

void Heap::removeDisposer_(IDisposer* o)
{
    ConditionalScopedLock<CriticalSection> lock(&mCS, mFlag.isOnBit(0));
    mDisposerList.erase(o);
}

Heap*
Heap::findContainHeap_(const void* ptr)
{
    ScopedLock<CriticalSection> lock_tree(HeapMgr::getHeapTreeLockCS_());

    if (!isInclude(ptr))
        return nullptr;

    ConditionalScopedLock<CriticalSection> lock(&mCS, mFlag.isOnBit(0));

    for (Heap::HeapList::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
    {
        Heap& heap = *it;
        if (heap.isInclude(ptr))
            return heap.findContainHeap_(ptr);
    }

    return this;
}

} // namespace sead
