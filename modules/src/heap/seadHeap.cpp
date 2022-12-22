#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace sead {

void Heap::appendDisposer_(IDisposer* o)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = nullptr;
    if (mFlag.isOnBit(0))
    {
        cs = &mCS;
        cs->lock();
    }

    mDisposerList.pushBack(o);

    if (cs != nullptr)
        cs->unlock();
}

void Heap::removeDisposer_(IDisposer* o)
{
    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = nullptr;
    if (mFlag.isOnBit(0))
    {
        cs = &mCS;
        cs->lock();
    }

    mDisposerList.erase(o);

    if (cs != nullptr)
        cs->unlock();
}

Heap*
Heap::findContainHeap_(const void* ptr)
{
    HeapMgr::getHeapTreeLockCS_()->lock();

    if (!isInclude(ptr))
    {
        HeapMgr::getHeapTreeLockCS_()->unlock();
        return nullptr;
    }

    // sead::ConditionalScopedLock<sead::CriticalSection>*
    CriticalSection* cs = nullptr;
    if (mFlag.isOnBit(0))
    {
        cs = &mCS;
        cs->lock();
    }

    for (Heap::HeapList::iterator it = mChildren.begin(); it != mChildren.end(); ++it)
    {
        if (it->isInclude(ptr))
        {
            Heap* heap = it->findContainHeap_(ptr);
            if (cs != nullptr)
                cs->unlock();

            HeapMgr::getHeapTreeLockCS_()->unlock();
            return heap;
        }
    }

    if (cs != nullptr)
        cs->unlock();

    HeapMgr::getHeapTreeLockCS_()->unlock();
    return this;
}

} // namespace sead
