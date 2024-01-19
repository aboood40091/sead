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
    if (isBufferReady())
    {
        delete[] mPtrs;
        mPtrs = nullptr;
        mPtrNum = 0;
        mPtrNumMax = 0;
    }
}

void PtrArrayImpl::sort(CompareCallbackImpl cmp)
{
    // Algorithm used is "Cocktail Sort", also known as "Bidirectional Bubble Sort".

    s32 top;
    s32 bottom;
    s32 last_swap;
    void* x;
    void** ptrs = mPtrs;

    if (mPtrNum <= 1)
        return;

    top = 0;
    bottom = mPtrNum - 1;

    while (true)
    {
        last_swap = top;
        for (s32 i = top; i < bottom; i++)
        {
            if ((*cmp)(ptrs[i], ptrs[i + 1]) > 0)
            {
                x = ptrs[i + 1];
                ptrs[i + 1] = ptrs[i];
                ptrs[i] = x;
                last_swap = i;
            }
        }
        bottom = last_swap;
        if (static_cast<u32>(top) == static_cast<u32>(bottom))
            break;

        last_swap = bottom;
        for (s32 i = bottom; i > top; i--)
        {
            if ((*cmp)(ptrs[i], ptrs[i - 1]) < 0)
            {
                void* x = ptrs[i - 1];
                ptrs[i - 1] = ptrs[i];
                ptrs[i] = x;
                last_swap = i;
            }
        }
        top = last_swap;
        if (static_cast<u32>(top) == static_cast<u32>(bottom))
            break;
    }
}

} // namespace sead
