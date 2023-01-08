#include <stddef.h>

#include <basis/seadNew.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

#include <cafe.h>

void* operator new(size_t size)
{
    const s32 alignment = 4;

    if (sead::HeapMgr::isInitialized())
    {
        sead::Heap* heap = sead::HeapMgr::instance()->getCurrentHeap();
        if (heap == nullptr)
        {
            //SEAD_ASSERT_MSG(false, "Current heap is null. Cannot alloc.");
            return nullptr;
        }

        void* ret = heap->tryAlloc(size, alignment);
        //SEAD_ASSERT_MSG(ret != nullptr, "alloc failed. size: %u, allocatable size: %u, alignment: %d, heap: %s",
        //                size, heap->getMaxAllocatableSize(alignment), alignment, heap->getName().cstr());
        return ret;
    }

    //SEAD_WARNING(false, "alloced[%d] before sead system initialize", size);
    return (*MEMAllocFromDefaultHeap)(size);
}

void* operator new[](size_t size)
{
    const s32 alignment = 4;

    if (sead::HeapMgr::isInitialized())
    {
        sead::Heap* heap = sead::HeapMgr::instance()->getCurrentHeap();
        if (heap == nullptr)
        {
            //SEAD_ASSERT_MSG(false, "Current heap is null. Cannot alloc.");
            return nullptr;
        }

        void* ret = heap->tryAlloc(size, alignment);
        //SEAD_ASSERT_MSG(ret != nullptr, "alloc failed. size: %u, allocatable size: %u, alignment: %d, heap: %s",
        //                size, heap->getMaxAllocatableSize(alignment), alignment, heap->getName().cstr());
        return ret;
    }

    //SEAD_WARNING(false, "alloced[%d] before sead system initialize", size);
    return (*MEMAllocFromDefaultHeap)(size);
}

void operator delete(void* ptr)
{
    if (ptr != nullptr)
    {
        if (sead::HeapMgr::isInitialized())
        {
            sead::Heap* heap = sead::HeapMgr::instance()->findContainHeap(ptr);
            if (heap == nullptr)
            {
                //SEAD_ASSERT_MSG(false, "delete bad pointer [0x%p]", ptr);
                return;
            }

            return heap->free(ptr);
        }

        //SEAD_WARNING(false, "free[0x%p] before sead system initialize", ptr);
        return (*MEMFreeToDefaultHeap)(ptr);
    }
}

void operator delete[](void* ptr)
{
    if (ptr != nullptr)
    {
        if (sead::HeapMgr::isInitialized())
        {
            sead::Heap* heap = sead::HeapMgr::instance()->findContainHeap(ptr);
            if (heap == nullptr)
            {
                //SEAD_ASSERT_MSG(false, "delete bad pointer [0x%p]", ptr);
                return;
            }

            return heap->free(ptr);
        }

        //SEAD_WARNING(false, "free[0x%p] before sead system initialize", ptr);
        return (*MEMFreeToDefaultHeap)(ptr);
    }
}

void* operator new(size_t size, sead::Heap* heap, s32 alignment)
{
    if (heap == nullptr)
    {
        heap = sead::HeapMgr::instance()->getCurrentHeap();
        if (heap == nullptr)
        {
            //SEAD_ASSERT_MSG(false, "Current heap is null. Cannot alloc.");
            return nullptr;
        }
    }

    void* ret = heap->tryAlloc(size, alignment);
    //SEAD_ASSERT_MSG(ret != nullptr, "alloc failed. size: %u, allocatable size: %u, alignment: %d, heap: %s",
    //                size, heap->getMaxAllocatableSize(alignment), alignment, heap->getName().cstr());
    return ret;
}

void* operator new[](size_t size, sead::Heap* heap, s32 alignment)
{
    if (heap == nullptr)
    {
        heap = sead::HeapMgr::instance()->getCurrentHeap();
        if (heap == nullptr)
        {
            //SEAD_ASSERT_MSG(false, "Current heap is null. Cannot alloc.");
            return nullptr;
        }
    }

    void* ret = heap->tryAlloc(size, alignment);
    //SEAD_ASSERT_MSG(ret != nullptr, "alloc failed. size: %u, allocatable size: %u, alignment: %d, heap: %s",
    //                size, heap->getMaxAllocatableSize(alignment), alignment, heap->getName().cstr());
    return ret;
}
