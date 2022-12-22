#include <basis/seadNew.h>
#include <container/seadPtrArray.h>

namespace sead {

void PtrArrayImpl::setBuffer(s32 ptrNumMax, void* buf)
{
    if (ptrNumMax > 0)
    {
        if (buf != nullptr)
        {
            mPtrNumMax = ptrNumMax;
            mPtrs = static_cast<void**>(buf);
            mPtrNum = 0;
        }
        else
            /* SEAD_ASSERT_MSG(false, "buf is null") */;
    }
    else
        /* SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax) */;
}

void PtrArrayImpl::allocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment)
{
    //SEAD_ASSERT(mPtrs == nullptr);
    if (ptrNumMax > 0)
        setBuffer(ptrNumMax, new (heap, alignment) u8[sizeof(void*) * ptrNumMax]);
    else
        /* SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax) */;
}

void PtrArrayImpl::freeBuffer()
{
    if (mPtrs != nullptr)
    {
        delete[] mPtrs;
        mPtrs = nullptr;
        mPtrNum = 0;
        mPtrNumMax = 0;
    }
}

void PtrArrayImpl::sort(CompareCallbackImpl cmp)
{
    // One day, I will care enough to check which algorithm this is
    if (mPtrNum > 1)
    {
        s32 v0, v1, v2, v3, v4;

        v1 = 0;
        v4 = v1;
        v3 = mPtrNum - 1;
        v2 = v1;
        do
        {
            v0 = v4;
            while (v1 < v3)
            {
                v4 = (*cmp)(mPtrs[v1], mPtrs[v1 + 1]);
                if (v4 > 0)
                {
                    void* ptr = mPtrs[v1 + 1];
                    mPtrs[v1 + 1] = mPtrs[v1];
                    mPtrs[v1] = ptr;
                    v0 = v1;
                }
                v1 = v1 + 1;
                v4 = v0;
            }
            v1 = v0;
            v4 = v0;
            if (v2 == v0)
                return;
            while (v2 < v4)
            {
                v3 = (*cmp)(mPtrs[v4], mPtrs[v4 - 1]);
                if (v3 < 0)
                {
                    void* ptr = mPtrs[v4 - 1];
                    mPtrs[v4 - 1] = mPtrs[v4];
                    mPtrs[v4] = ptr;
                    v1 = v4;
                }
                v4 = v4 - 1;
            }
            v4 = v1;
            v3 = v0;
            v2 = v1;
        }
        while (v1 != v0);
    }
}

} // namespace sead
