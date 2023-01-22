#ifndef SEAD_BUFFER_H_
#define SEAD_BUFFER_H_

#include <basis/seadNew.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>

namespace sead {

template <typename T>
class Buffer
{
private:
    typedef s32 (*CompareCallback)(const T*, const T*);

public:
    Buffer()
        : mSize(0)
        , mBuffer(nullptr)
    {
    }

    Buffer(s32 size, T* bufferptr)
    {
        if (size > 0 && bufferptr != nullptr)
        {
            mSize = size;
            mBuffer = bufferptr;
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "illegal param: size[%d] bufferptr[0x%x]", size, bufferptr);
            size = 0;
            bufferptr = nullptr;
        }
    }

public:
    class iterator;
    class constIterator;

    class iterator
    {
    public:
        explicit iterator()
            : mPtr(nullptr)
            , mBuffer(nullptr)
        {
        }

        explicit iterator(Buffer* buffer)
            : mPtr(buffer->mBuffer)
            , mBuffer(buffer->mBuffer)
        {
        }

        iterator(Buffer* buffer, s32 index)
            : mPtr(&buffer->mBuffer[index])
            , mBuffer(buffer->mBuffer)
        {
        }

        iterator& operator++()
        {
            mPtr++;
            return *this;
        }

        iterator& operator--()
        {
            mPtr--;
            return *this;
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs)
        {
            return lhs.mPtr == rhs.mPtr;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs)
        {
            return lhs.mPtr != rhs.mPtr;
        }

        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

        s32 getIndex() const { return (uintptr(mPtr) - uintptr(mBuffer)) / sizeof(T); }

    private:
        T* mPtr;
        T* mBuffer;

        friend class constIterator;
    };

    class constIterator
    {
    public:
        explicit constIterator()
            : mPtr(nullptr)
            , mBuffer(nullptr)
        {
        }

        explicit constIterator(const Buffer* buffer)
            : mPtr(buffer->mBuffer)
            , mBuffer(buffer->mBuffer)
        {
        }

        constIterator(const Buffer* buffer, s32 index)
            : mPtr(&buffer->mBuffer[index])
            , mBuffer(buffer->mBuffer)
        {
        }

        explicit constIterator(iterator it)
            : mPtr(it.mPtr)
            , mBuffer(it.mBuffer)
        {
        }

        constIterator& operator++()
        {
            mPtr++;
            return *this;
        }

        constIterator& operator--()
        {
            mPtr--;
            return *this;
        }

        friend bool operator==(const constIterator& lhs, const constIterator& rhs)
        {
            return lhs.mPtr == rhs.mPtr;
        }

        friend bool operator!=(const constIterator& lhs, const constIterator& rhs)
        {
            return lhs.mPtr != rhs.mPtr;
        }

        const T& operator*() const { return *mPtr; }
        const T* operator->() const { return mPtr; }

        s32 getIndex() const { return (uintptr(mPtr) - uintptr(mBuffer)) / sizeof(T); }

    private:
        const T* mPtr;
        const T* mBuffer;
    };

    // TODO
    class reverseIterator { };

    // TODO
    class reverseConstIterator { };

public:
    iterator begin() { return iterator(this); }
    constIterator begin() const { return constIterator(this); }

    iterator end() { return iterator(this, mSize); }
    constIterator end() const { return constIterator(this, mSize); }

    iterator toIterator(s32);
    constIterator toIterator(s32) const;

    constIterator constBegin() const { return constIterator(this); }

    constIterator constEnd() const { return constIterator(this, mSize); }

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
    void allocBuffer(s32 size, s32 alignment = 4)
    {
        // SEAD_ASSERT(mBuffer == nullptr);

        if (size > 0)
        {
            Heap* heap = HeapMgr::instance()->getCurrentHeap();
            T* buffer = static_cast<T*>(heap->alloc(size * sizeof(T), alignment));

            for (s32 i = 0; i < size; i++)
                new (static_cast<void*>(&buffer[i])) T;

            setBuffer(size, buffer);
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "size[%d] must be larger than zero", size);
        }
    }

    void allocBuffer(s32 size, Heap* heap, s32 alignment = 4)
    {
        // SEAD_ASSERT(mBuffer == nullptr);

        if (size > 0)
        {
            if (heap == nullptr) heap = HeapMgr::instance()->getCurrentHeap();
            T* buffer = static_cast<T*>(heap->alloc(size * sizeof(T), alignment));

            for (s32 i = 0; i < size; i++)
                new (static_cast<void*>(&buffer[i])) T;

            setBuffer(size, buffer);
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "size[%d] must be larger than zero", size);
        }
    }

    bool tryAllocBuffer(s32 size, s32 alignment = 4)
    {
        // SEAD_ASSERT(mBuffer == nullptr);

        if (size > 0)
        {
            Heap* heap = HeapMgr::instance()->getCurrentHeap();
            T* buffer = static_cast<T*>(heap->alloc(size * sizeof(T), alignment));

            for (s32 i = 0; i < size; i++)
                new (static_cast<void*>(&buffer[i])) T;

            setBuffer(size, buffer);
            return true;
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "size[%d] must be larger than zero", size);
            return false;
        }
    }

    bool tryAllocBuffer(s32 size, Heap* heap, s32 alignment = 4)
    {
        // SEAD_ASSERT(mBuffer == nullptr);

        if (size > 0)
        {
            if (heap == nullptr) heap = HeapMgr::instance()->getCurrentHeap();
            T* buffer = static_cast<T*>(heap->alloc(size * sizeof(T), alignment));

            for (s32 i = 0; i < size; i++)
                new (static_cast<void*>(&buffer[i])) T;

            setBuffer(size, buffer);
            return true;
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "size[%d] must be larger than zero", size);
            return false;
        }
    }

    void freeBuffer()
    {
        if (isBufferReady())
        {
            for (s32 i = 0; i < mSize; i++)
                (&mBuffer[i])->~T();

            Heap* heap = HeapMgr::instance()->findContainHeap(mBuffer);
            heap->free(mBuffer);

            mBuffer = nullptr;
            mSize = 0;
        }
    }

    void setBuffer(s32 size, T* bufferptr)
    {
        if (size > 0)
        {
            if (bufferptr != nullptr)
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

    bool isBufferReady() const { return mBuffer != nullptr; }

    T& operator()(s32);
    const T& operator()(s32) const;

    T& operator[] (s32 x)
    {
        if (static_cast<u32>(x) < static_cast<u32>(mSize))
        {
            return mBuffer[x];
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
            return mBuffer[0];
        }
    }

    const T& operator[] (s32 x) const
    {
        if (static_cast<u32>(x) < static_cast<u32>(mSize))
        {
            return mBuffer[x];
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
            return mBuffer[0];
        }
    }

    T* get(s32 x)
    {
        if (static_cast<u32>(x) < static_cast<u32>(mSize))
        {
            return &mBuffer[x];
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
            return nullptr;
        }
    }

    const T* get(s32 x) const
    {
        if (static_cast<u32>(x) < static_cast<u32>(mSize))
        {
            return &mBuffer[x];
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "index exceeded [%d/%d]", x, mSize);
            return nullptr;
        }
    }

    T* unsafeGet(s32 x)
    {
        //SEAD_ASSERT_MSG(static_cast<u32>(x) < static_cast<u32>(mSize), "index exceeded [%d/%d]", x, mSize);
        return &mBuffer[x];
    }

    const T* unsafeGet(s32 x) const
    {
        //SEAD_ASSERT_MSG(static_cast<u32>(x) < static_cast<u32>(mSize), "index exceeded [%d/%d]", x, mSize);
        return &mBuffer[x];
    }

    T& front()
    {
        return mBuffer[0];
    }

    const T& front() const
    {
        return mBuffer[0];
    }

    T& back()
    {
        return mBuffer[mSize - 1];
    }

    const T& back() const
    {
        return mBuffer[mSize - 1];
    }

    s32 size() const { return mSize; }

    s32 getSize() const { return mSize; }
    T* getBufferPtr() { return mBuffer; }
    const T* getBufferPtr() const { return mBuffer; }
    u32 getByteSize() const { return mSize * sizeof(T); }

    Buffer<T>& operator=(const Buffer<T>&);

    bool isRangeValid(s32 x) const
    {
        return static_cast<u32>(x) < static_cast<u32>(mSize);
    }

    void fill(const T& value)
    {
        s32 size = mSize;
        for (s32 i = 0; i < size; i++)
            mBuffer[i] = value;
    }

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
