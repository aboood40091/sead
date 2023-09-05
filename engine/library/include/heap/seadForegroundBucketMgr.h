#ifndef SEAD_FOREGROUND_BUCKET_MGR_H_
#define SEAD_FOREGROUND_BUCKET_MGR_H_

#include <basis/seadTypes.h>

namespace sead {

class Heap;
class SeparateHeap;

class ForegroundBucketMgr
{
public:
    static ForegroundBucketMgr* instance() { return &sForegroundBucketMgr; }

public:
    ForegroundBucketMgr()
        : mSeparateHeap(nullptr)
    {
    }

    void initialize(Heap* heap, u32 block_max, u32 alloc_area_size, bool enable_lock = false);

    SeparateHeap* getSeparateHeap() const
    {
        return mSeparateHeap;
    }

private:
    SeparateHeap* mSeparateHeap;

    static ForegroundBucketMgr sForegroundBucketMgr;
};
#ifdef cafe
static_assert(sizeof(ForegroundBucketMgr) == 4, "sead::ForegroundBucketMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_FOREGROUND_BUCKET_MGR_H_
