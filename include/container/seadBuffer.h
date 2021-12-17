#ifndef SEAD_BUFFER_H_
#define SEAD_BUFFER_H_

#include <basis/seadTypes.h>

namespace sead {

class Heap;

template <typename T>
class Buffer
{
private:
    typedef s32 (*CompareCallback)(const T*, const T*);

public:
    Buffer()
        : mSize(0)
        , mBuffer(NULL)
    {
    }

    Buffer(s32, T*);

public:
    // TODO
    class iterator { };

    // TODO
    class constIterator { };

    // TODO
    class reverseIterator { };

    // TODO
    class reverseConstIterator { };

public:
    iterator begin();
    constIterator begin() const;
    iterator end();
    constIterator end() const;
    iterator toIterator(s32);
    constIterator toIterator(s32) const;
    constIterator constBegin() const;
    constIterator constEnd() const;
    constIterator toConstIterator(s32) const;
    reverseIterator reverseBegin();
    reverseConstIterator reverseBegin() const;
    reverseIterator reverseEnd();
    reverseConstIterator reverseEnd() const;
    reverseIterator toReverseIterator(s32);
    reverseConstIterator toReverseIterator(s32) const;
    reverseConstIterator reverseConstBegin() const;
    reverseConstIterator reverseConstEnd() const;
    reverseConstIterator toReverseConstIterator(s32) const;

public:
    void allocBuffer(s32 size, s32 alignment);
    void allocBuffer(s32 size, Heap* heap, s32 alignment);
    bool tryAllocBuffer(s32 size, s32 alignment);
    bool tryAllocBuffer(s32 size, Heap* heap, s32 alignment);
    void freeBuffer();

    void setBuffer(s32 size, T* bufferptr)
    {
        if (size > 0)
        {
            if (bufferptr != NULL)
            {
                mSize = size;
                mBuffer = bufferptr;
            }
            else
            {
                //SEAD_ASSERT_MSG(false, "bufferptr is null");
            }
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "size[%d] must be larger than zero", size);
        }
    }

    bool isBufferReady() const;
    T& operator()(s32);
    const T& operator()(s32) const;

    T& operator[] (s32 x)
    {
        if (x < mSize)
            return mBuffer[x];

        //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
        return mBuffer[0];
    }

    const T& operator[] (s32 x) const
    {
        if (x < mSize)
            return mBuffer[x];

        //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
        return mBuffer[0];
    }

    T* get(s32 x)
    {
        if (x < mSize)
            return &mBuffer[x];

        //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
        return NULL;
    }

    const T* get(s32 x) const
    {
        if (x < mSize)
            return &mBuffer[x];

        //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
        return NULL;
    }

    T* unsafeGet(s32 x)
    {
        //SEAD_ASSERT_MSG(x < mSize, "index exceeded [%d/%d]", x, mSize);
        return &mBuffer[x];
    }

    const T* unsafeGet(s32 x) const
    {
        //SEAD_ASSERT_MSG(x < mSize, "index exceeded [%d/%d]", x, mSize);
        return &mBuffer[x];
    }

    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    s32 size() const;
    s32 getSize() const { return mSize; }
    T* getBufferPtr() { return mBuffer; }
    const T* getBufferPtr() const { return mBuffer; }
    u32 getByteSize() const { return mSize*sizeof(T); }
    Buffer<T>& operator=(const Buffer<T>&);
    bool isRangeValid(s32) const;
    void fill(const T&);
    void heapSort(CompareCallback, s32, s32);
    void heapSort(s32, s32);
    s32 binarySearch(const T&) const;

protected:
    static s32 compareT(const T*, const T*);
    static s32 defaultBinarySearchCompare(const T&, const T&);

public:
    template <typename Key>
    s32 binarySearch(const Key&, s32 (*)(const T&, const Key&)) const;

protected:
    s32 mSize;
    T* mBuffer;
};
#ifdef cafe
static_assert(sizeof(Buffer<int>) == 8, "sead::Buffer<T> size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_BUFFER_H_
