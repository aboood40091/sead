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

private:
    Heap* mHeaps[4];
    bool mAdjusted[4];
    s32 mPrimaryIndex;

    friend class TaskMgr;
};

struct HeapPolicy
{
    HeapPolicy()
        : parent(NULL)
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

} // namespace sead

#endif // SEAD_HEAP_POLICIES_H_
