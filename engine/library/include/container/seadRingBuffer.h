#ifndef SEAD_RING_BUFFER_H_
#define SEAD_RING_BUFFER_H_

#include <basis/seadTypes.h>

namespace sead {

class Heap;

template <typename T>
class RingBuffer
{
public:
    RingBuffer()
        : mBuffer(nullptr)
        , mNum(0)
        , mHead(0)
        , mNumMax(0)
    {
    }

    void setBuffer(s32 numMax, T* buf)
    {
        if (numMax > 0)
        {
            if (buf != nullptr)
            {
                mBuffer = buf;
                mNum = 0;
                mHead = 0;
                mNumMax = numMax;
            }
            else
            {
                // SEAD_ASSERT_MSG(false, "buf is null");
            }
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "numMax[%d] must be larger than zero", numMax);
        }
    }

    void allocBuffer(s32 numMax, Heap* heap, s32 alignment = 4);
    bool tryAllocBuffer(s32 numMax, Heap* heap, s32 alignment = 4);
    void freeBuffer();
    bool isBufferReady() const { return mBuffer != nullptr; }
    bool isEmpty() const { return mNum == 0; }
    bool isFull() const { return mNum >= mNumMax; }
    s32 size() const { return mNum; }
    s32 maxSize() const { return mNumMax; }

    void clear()
    {
        mNum = 0;
        mHead = 0;
    }

    // ...

    T* get(s32 x)
    {
        if (static_cast<u32>(x) < static_cast<u32>(mNum))
        {
            return &mBuffer[calcRealIndex(x)];
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "index exceeded [%d/%d/%d]", x, mNum, mNumMax);
            return nullptr;
        }
    }

    const T* get(s32 x) const
    {
        if (static_cast<u32>(x) < static_cast<u32>(mNum))
        {
            return &mBuffer[calcRealIndex(x)];
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "index exceeded [%d/%d/%d]", x, mNum, mNumMax);
            return nullptr;
        }
    }

    T* unsafeGet(s32 x)
    {
        // SEAD_ASSERT_MSG(static_cast<u32>(x) < static_cast<u32>(mNum), "index exceeded [%d/%d/%d]", x, mNum, mNumMax);
        return &mBuffer[calcRealIndex(x)];
    }

    const T* unsafeGet(s32 x) const
    {
        // SEAD_ASSERT_MSG(static_cast<u32>(x) < static_cast<u32>(mNum), "index exceeded [%d/%d/%d]", x, mNum, mNumMax);
        return &mBuffer[calcRealIndex(x)];
    }

    // ...

    void pushBack(const T& obj)
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "list is full.");
            return;
        }

        *unsafeGet(mNum++) = obj;
    }

    // ...

protected:
    s32 calcRealIndex(s32 x) const
    {
        s32 idx = mHead + x;
        if (idx >= mNumMax)
            idx -= mNumMax;
        return idx;
    }

public:
    class iterator
    {
    public:
        explicit iterator(RingBuffer* buffer, s32 index = 0)
            : mIndex(index),
            mBuffer(buffer)
        {
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs)
        {
            return lhs.mBuffer == rhs.mBuffer && lhs.mIndex == rhs.mIndex;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs)
        {
            return lhs.mBuffer != rhs.mBuffer || lhs.mIndex != rhs.mIndex;
        }

        iterator& operator++()
        {
            ++mIndex;
            return *this;
        }

        T& operator*() const { return *mBuffer->unsafeGet(mIndex); }
        T* operator->() const { return mBuffer->unsafeGet(mIndex); }

        s32 getIndex() const { return mIndex; }

    private:
        s32 mIndex;
        RingBuffer* mBuffer;
    };

public:
    iterator begin()
    {
        return iterator(this);
    }

    iterator end()
    {
        return iterator(this, size());
    }

protected:
    T* mBuffer;
    s32 mNumMax;
    s32 mHead;
    s32 mNum;
};

template <typename T, s32 N>
class FixedRingBuffer : public RingBuffer<T>
{
public:
    FixedRingBuffer()
        : RingBuffer<T>()
    {
        RingBuffer<T>::setBuffer(N, mWork);
    }

protected:
    void setBuffer(s32 numMax, T* buf);
    void allocBuffer(s32 numMax, Heap* heap, s32 alignment = 4);
    bool tryAllocBuffer(s32 numMax, Heap* heap, s32 alignment = 4);
    void freeBuffer();

protected:
    T mWork[N];
};

}

#endif // SEAD_RING_BUFFER_H_
