#ifndef SEAD_HEAP_POLICIES_H_
#define SEAD_HEAP_POLICIES_H_

#include <heap/seadHeapMgr.h>

namespace sead {

class HeapArray
{
public:
    HeapArray()
        : mPrimaryIndex(0)
    {
        for (u32 i = 0; i < 4; i++)
            mAdjusted[i] = false;
    }

    Heap* getHeap(s32 idx) const
    {
        if (idx < HeapMgr::getRootHeapNum())
            return mHeaps[idx];

        //SEAD_ASSERT_MSG(false, "illegal idx: %d", idx);
        return mHeaps[0];
    }

    Heap* getHeap(s32 idx)
    {
        return const_cast<const HeapArray*>(this)->getHeap(idx);
    }

    Heap* getPrimaryHeap()
    {
        return mHeaps[mPrimaryIndex];
    }

private:
    Heap* mHeaps[4];
    bool mAdjusted[4];
    s32 mPrimaryIndex;

    friend class TaskMgr;
};
#ifdef cafe
static_assert(sizeof(HeapArray) == 0x18, "sead::HeapArray size mismatch");
#endif // cafe

struct HeapPolicy
{
    HeapPolicy()
        : parent(nullptr)
        , size(0)
        , create_slack(0)
        , adjust_slack(0)
        , adjust(0)
        , temporary(0)
        , dont_create(0)
    {
    }

    Heap* parent;
    u32 size;
    u32 create_slack;
    u32 adjust_slack;
    u8 adjust;
    u8 temporary;
    u8 dont_create;
};
#ifdef cafe
static_assert(sizeof(HeapPolicy) == 0x14, "sead::HeapPolicy size mismatch");
#endif // cafe

class HeapPolicies
{
public:
    HeapPolicies()
        : mPrimaryIndex(0)
    {
    }

    HeapPolicy mPolicies[4];
    s32 mPrimaryIndex;
};
#ifdef cafe
static_assert(sizeof(HeapPolicies) == 0x54, "sead::HeapPolicies size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_HEAP_POLICIES_H_
