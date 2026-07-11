#include <basis/seadNew.h>
#include <container/seadPtrArray.h>
#include <prim/seadMemUtil.h>
#include <random/seadRandom.h>

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

void PtrArrayImpl::erase(s32 pos, s32 num)
{
    if (pos < 0)
    {
        //SEAD_ASSERT_MSG(false, "illegal position[%d]", pos);
        return;
    }

    if (num < 0)
    {
        //SEAD_ASSERT_MSG(false, "illegal number[%d]", num);
        return;
    }

    s32 end = pos + num;
    if (end > mPtrNum)
    {
        //SEAD_ASSERT_MSG(false, "pos[%d] + num[%d] exceed size[%d]", pos, num, mPtrNum);
        return;
    }

    if (end < mPtrNum)
        MemUtil::copy(mPtrs + pos, mPtrs + end, sizeof(void*) * (mPtrNum - end));

    mPtrNum -= num;
}

void PtrArrayImpl::shuffle(Random* random)
{
    //SEAD_ASSERT(random);
    s32 count = mPtrNum;
    while (count > 1)
    {
        s32 index = random->getS32Range(0, count);
        count--;
        swap(count, index);
    }
}

bool PtrArrayImpl::checkInsert(s32 pos, s32 num)
{
    if (pos < 0)
        /* SEAD_ASSERT_MSG(false, "illegal pos[%d]", pos) */;

    else if (mPtrNum + num <= mPtrNumMax)
    {
        if (pos <= mPtrNum)
            return true;
        else
            /* SEAD_ASSERT_MSG(false, "pos[%d] exceed size[%d]", pos, mPtrNum) */;
    }
    else
    {
        //SEAD_ASSERT_MSG(false, "list is full.");
    }

    return false;
}

void PtrArrayImpl::insert(s32 pos, void* ptr)
{
    if (checkInsert(pos, 1))
    {
        if (pos < mPtrNum)
            MemUtil::copy(mPtrs + (pos + 1), mPtrs + pos, sizeof(void*) * (mPtrNum - pos));

        mPtrs[pos] = ptr;
        mPtrNum++;
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
                x = ptrs[i - 1];
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

void PtrArrayImpl::heapSort(CompareCallbackImpl cmp)
{
    s32 count = mPtrNum;
    void** ptrs = mPtrs;

    for (s32 mid = count / 2; mid > 0; mid--)
    {
        void* x = ptrs[mid - 1];
        s32 parent = mid;
        s32 child;
        while ((child = parent * 2) <= count)
        {
            if (child < count && (*cmp)(ptrs[child - 1], ptrs[child]) < 0)
                child++;
            if ((*cmp)(x, ptrs[child - 1]) >= 0)
                break;
            ptrs[parent - 1] = ptrs[child - 1];
            parent = child;
        }
        ptrs[parent - 1] = x;
    }

    while (count > 1)
    {
        void* x = ptrs[count - 1];
        ptrs[count - 1] = ptrs[0];
        count--;
        s32 parent = 1;
        s32 child;
        while ((child = parent * 2) <= count)
        {
            if (child < count && (*cmp)(ptrs[child - 1], ptrs[child]) < 0)
                child++;
            if ((*cmp)(x, ptrs[child - 1]) >= 0)
                break;
            ptrs[parent - 1] = ptrs[child - 1];
            parent = child;
        }
        ptrs[parent - 1] = x;
    }
}

} // namespace sead
