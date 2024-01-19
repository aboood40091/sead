#ifndef SEAD_SAFE_ARRAY_H_
#define SEAD_SAFE_ARRAY_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename T, s32 N>
class SafeArray;

template <typename T>
class SafeArray<T, 0>
{
};

template <typename T, s32 N>
class SafeArray
{
public:
    T& operator[](s32 x)
    {
        if (static_cast<u32>(x) < static_cast<u32>(N))
        {
            return mBuffer[x];
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "range over [0, %d) : %d", N, x);
            return mBuffer[0];
        }
    }

    const T& operator[](s32 x) const
    {
        if (static_cast<u32>(x) < static_cast<u32>(N))
        {
            return mBuffer[x];
        }
        else
        {
            //SEAD_ASSERT_MSG(false, "range over [0, %d) : %d", N, x);
            return mBuffer[0];
        }
    }

    T& operator()(s32);
    const T& operator()(s32) const;

    T& front() { return mBuffer[0]; }
    const T& front() const { return mBuffer[0]; }

    T& back() { return mBuffer[N - 1]; }
    const T& back() const { return mBuffer[N - 1]; }

    s32 size() const { return N; }
    u32 getByteSize() const { return N * sizeof(T); }
    T* getBufferPtr() { return mBuffer; }
    const T* getBufferPtr() const { return mBuffer; }

    void fill(const T& value)
    {
        for (s32 i = 0; i < N; i++)
            mBuffer[i] = value;
    }

public:
    class iterator
    {
    public:
        explicit iterator(T* buffer, s32 index = 0)
            : mPtr(buffer + index)
            , mStart(buffer)
        {
        }

        bool operator==(const iterator& rhs) const
        {
            return mPtr == rhs.mPtr;
        }

        bool operator!=(const iterator& rhs) const
        {
            return mPtr != rhs.mPtr;
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

        T& operator*() const { return *mPtr; }
        T* operator->() const { return mPtr; }

        s32 getIndex() const { return ((uintptr_t)mPtr - (uintptr_t)mStart) / sizeof(T); }

    private:
        T* mPtr;
        T* mStart;
    };

    class constIterator
    {
    public:
        explicit constIterator(const T* buffer, s32 index = 0)
            : mPtr(buffer + index)
            , mStart(buffer)
        {
        }

        bool operator==(const constIterator& rhs) const
        {
            return mPtr == rhs.mPtr;
        }

        bool operator!=(const constIterator& rhs) const
        {
            return mPtr != rhs.mPtr;
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

        const T& operator*() const { return *mPtr; }
        const T* operator->() const { return mPtr; }

        s32 getIndex() const { return ((uintptr_t)mPtr - (uintptr_t)mStart) / sizeof(T); }

    private:
        const T* mPtr;
        const T* mStart;
    };

    // TODO
    class reverseIterator { };

    // TODO
    class reverseConstIterator { };

public:
    iterator begin() { return iterator(mBuffer); }
    constIterator begin() const { return constIterator(mBuffer); }

    iterator end() { return iterator(mBuffer, N); }
    constIterator end() const { return constIterator(mBuffer, N); }

    iterator toIterator(s32 x)
    {
        if (static_cast<u32>(x) <= static_cast<u32>(N))
        {
            return iterator(mBuffer, x);
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "range over [0,%d] : %d", N, x);
            return end();
        }
    }

    constIterator toIterator(s32 x) const
    {
        if (static_cast<u32>(x) <= static_cast<u32>(N))
        {
            return constIterator(mBuffer, x);
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "range over [0,%d] : %d", N, x);
            return end();
        }
    }


    constIterator constBegin() const { return constIterator(mBuffer); }

    constIterator constEnd() const { return constIterator(mBuffer, N); }

    constIterator toConstIterator(s32 x) const
    {
        if (static_cast<u32>(x) <= static_cast<u32>(N))
        {
            return constIterator(mBuffer, x);
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "range over [0,%d] : %d", N, x);
            return constEnd();
        }
    }

    reverseIterator reverseBegin();
    reverseConstIterator reverseBegin() const;

    reverseIterator reverseEnd();
    reverseConstIterator reverseEnd() const;

    reverseIterator toReverseIterator(s32);
    reverseConstIterator toReverseIterator(s32) const;

    reverseConstIterator reverseConstBegin() const;
    reverseConstIterator reverseConstEnd() const;

    reverseConstIterator toReverseConstIterator(s32) const;

    T mBuffer[N];
};

template <typename T, s32 N>
class UnsafeArray
{
public:
    T& operator[](s32 x) { return mBuffer[x]; }
    const T& operator[](s32 x) const { return mBuffer[x]; }

    T& operator()(s32);
    const T& operator()(s32) const;

    T& front() { return mBuffer[0]; }
    const T& front() const { return mBuffer[0]; }

    T& back() { return mBuffer[N - 1]; }
    const T& back() const { return mBuffer[N - 1]; }

    s32 size() const { return N; }
    u32 getByteSize() const { return N * sizeof(T); }
    T* getBufferPtr() { return mBuffer; }
    const T* getBufferPtr() const { return mBuffer; }

    void fill(const T& value)
    {
        for (s32 i = 0; i < N; i++)
            mBuffer[i] = value;
    }

public:
    typedef typename SafeArray<T, N>::iterator iterator;
    typedef typename SafeArray<T, N>::constIterator constIterator;
    typedef typename SafeArray<T, N>::reverseIterator reverseIterator;
    typedef typename SafeArray<T, N>::reverseConstIterator reverseConstIterator;

public:
    iterator begin() { return iterator(mBuffer); }
    constIterator begin() const { return constIterator(mBuffer); }

    iterator end() { return iterator(mBuffer, N); }
    constIterator end() const { return constIterator(mBuffer, N); }

    iterator toIterator(s32 x) { return iterator(mBuffer, x); }
    constIterator toIterator(s32 x) const { return constIterator(mBuffer, x); }

    constIterator constBegin() const { return constIterator(mBuffer); }

    constIterator constEnd() const { return constIterator(mBuffer, N); }

    constIterator toConstIterator(s32 x) const { return constIterator(mBuffer, x); }

    reverseIterator reverseBegin();
    reverseConstIterator reverseBegin() const;

    reverseIterator reverseEnd();
    reverseConstIterator reverseEnd() const;

    reverseIterator toReverseIterator(s32);
    reverseConstIterator toReverseIterator(s32) const;

    reverseConstIterator reverseConstBegin() const;
    reverseConstIterator reverseConstEnd() const;

    reverseConstIterator toReverseConstIterator(s32) const;

    T mBuffer[N];
};

} // namespace sead

#endif // SEAD_SAFE_ARRAY_H_
