#include <heap/seadFrameHeap.h>
#include <heap/seadHeapMgr.h>
#include <math/seadMathCalcCommon.h>
#include <prim/seadPtrUtil.h>
#include <prim/seadScopedLock.h>

namespace sead {

FrameHeap*
FrameHeap::tryCreate(u32 size, const SafeString& name, Heap* parent, HeapDirection direction, bool enable_lock)
{
    ScopedLock<CriticalSection> lock(HeapMgr::getHeapTreeLockCS_());

    if (parent == nullptr)
    {
        parent = HeapMgr::instance()->getCurrentHeap();
        if (parent == nullptr)
        {
            // SEAD_ASSERT_MSG(false, "current heap is null");
            return nullptr;
        }
    }

    if (size == 0)
        size = parent->getMaxAllocatableSize();

    size = Mathi::roundUpPow2(size, 4);
    if (size < sizeof(FrameHeap))
    {
        // SEAD_WARNING(false, "size must be able to include manage area: size=%d\n", size);
        return nullptr;
    }

  //SEAD_ASSERT(isEnableDebugFillSystem_(parent));
  //setEnableDebugFillSystem_(parent, false);
    void* ptr = parent->tryAlloc(size, s32(direction) * 4);
  //setEnableDebugFillSystem_(parent, true);

    if (ptr == nullptr)
    {
        // SEAD_WARNING(false, "heap alloc failed\n");
        return nullptr;
    }

    FrameHeap* heap;

    if (direction == cHeapDirection_Forward)
    {
        heap = new (ptr) FrameHeap(
            name, parent, ptr, size,
            cHeapDirection_Forward,
            enable_lock
        );
    }
    else
    {
        heap = new (PtrUtil::addOffset(ptr, size - sizeof(FrameHeap))) FrameHeap(
            name, parent, ptr, size,
            direction,
            enable_lock
        );
    }

    heap->initialize_();

    HeapMgr* heap_mgr = HeapMgr::instance();
    if (heap_mgr != nullptr)
        heap_mgr->callCreateCallback_(heap);

    return heap;
}

} // namespace sead
