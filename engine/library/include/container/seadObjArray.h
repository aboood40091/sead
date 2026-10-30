#ifndef SEAD_OBJ_ARRAY_H_
#define SEAD_OBJ_ARRAY_H_

#include <basis/seadNew.h>
#include <container/seadFreeList.h>
#include <container/seadPtrArray.h>
#include <prim/seadPtrUtil.h>

namespace sead {

template <typename T>
class ObjArray : public PtrArrayImpl
{
public:
    ObjArray()
        : PtrArrayImpl()
        , mFreeList()
    {
    }

    void setBuffer(s32 ptrNumMax, void* buf)
    {
        if (ptrNumMax <= 0)
        {
            // SEAD_ASSERT_MSG(false, "ptrNumMax[%d] must be larger than zero", ptrNumMax);
            return;
        }

        if (!buf)
        {
            // SEAD_ASSERT_MSG(false, "buf is null");
            return;
        }

        mFreeList.init(buf, sizeof(T), ptrNumMax);
        PtrArrayImpl::setBuffer(ptrNumMax, PtrUtil::addOffset(buf, ptrNumMax * sizeof(T)));
    }

    void allocBuffer(s32 limit_num, Heap* heap, s32 alignment = 4);
    bool tryAllocBuffer(s32 limit_num, Heap* heap, s32 alignment = 4);
    void freeBuffer();

    T* at(s32 n) const { return static_cast<T*>(PtrArrayImpl::at(n)); }
    T* unsafeAt(s32 n) const { return static_cast<T*>(PtrArrayImpl::unsafeAt(n)); }
    T* operator[](s32 n) const { return at(n); }
    T* front() const { return static_cast<T*>(PtrArrayImpl::front()); }
    T* back() const { return static_cast<T*>(PtrArrayImpl::back()); }

    T* birthBack()
    {
        if (PtrArrayImpl::isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        T* obj = new (mFreeList.get()) T;
        PtrArrayImpl::pushBack(obj);
        return obj;
    }

    void pushBack(const T& obj)
    {
        if (PtrArrayImpl::isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return;
        }

        T* ptr = new (mFreeList.get()) T(obj);
        PtrArrayImpl::pushBack(ptr);
    }

    void popBack()
    {
        resize(this->mPtrNum - 1);
    }

    void erase(s32 pos)
    {
        erase(pos, 1);
    }

    void erase(s32 pos, s32 num)
    {
        if (pos + num <= this->mPtrNum)
        {
            for (s32 i = pos; i < pos + num; i++)
            {
                T* obj = static_cast<T*>(this->mPtrs[i]);
                obj->~T();
                mFreeList.put(obj);
            }
        }

        PtrArrayImpl::erase(pos, num);
    }

    void resize(s32 size)
    {
        if (static_cast<u32>(size) > static_cast<u32>(this->mPtrNum))
        {
            // SEAD_ASSERT_MSG(false, ...);
            return;
        }

        for (s32 i = size; i < this->mPtrNum; i++)
        {
            T* obj = static_cast<T*>(this->mPtrs[i]);
            obj->~T();
            mFreeList.put(obj);
        }

        this->mPtrNum = size;
    }

    void unsafeResize(s32);

    void clear()
    {
        for (s32 i = 0; i < this->mPtrNum; i++)
        {
            T* obj = static_cast<T*>(this->mPtrs[i]);
            obj->~T();
            mFreeList.put(obj);
        }

        PtrArrayImpl::clear();
    }

    s32 indexOf(const T* ptr) const { return PtrArrayImpl::indexOf(ptr); }

public:
    class iterator
    {
    public:
        iterator(T* const* pptr)
            : mPPtr(pptr)
        {
        }

    public:
        iterator& operator++()
        {
            ++mPPtr;
            return *this;
        }

        T& operator*() const
        {
            return **mPPtr;
        }

        T* operator->() const
        {
            return *mPPtr;
        }

        friend bool operator==(const iterator& it1, const iterator& it2)
        {
            return it1.mPPtr == it2.mPPtr;
        }

        friend bool operator!=(const iterator& it1, const iterator& it2)
        {
            return !(it1 == it2);
        }

        T* getPtr() const
        {
            return *mPPtr;
        }

    protected:
        T* const* mPPtr;
    };

    class constIterator
    {
    public:
        constIterator(const T* const* pptr)
            : mPPtr(pptr)
        {
        }

    public:
        constIterator& operator++()
        {
            ++mPPtr;
            return *this;
        }

        const T& operator*() const
        {
            return **mPPtr;
        }

        const T* operator->() const
        {
            return *mPPtr;
        }

        friend bool operator==(const constIterator& it1, const constIterator& it2)
        {
            return it1.mPPtr == it2.mPPtr;
        }

        friend bool operator!=(const constIterator& it1, const constIterator& it2)
        {
            return !(it1 == it2);
        }

        const T* getPtr() const
        {
            return *mPPtr;
        }

    private:
        const T* const* mPPtr;
    };

    // TODO
    class reverseIterator { };

    // TODO
    class reverseConstIterator { };

public:
    iterator begin() const
    {
        return iterator(reinterpret_cast<T**>(mPtrs));
    }

    iterator end() const
    {
        return iterator(reinterpret_cast<T**>(mPtrs) + mPtrNum);
    }

    iterator toIterator(s32) const;

    constIterator constBegin() const
    {
        return constIterator(reinterpret_cast<T**>(mPtrs));
    }
    constIterator constEnd() const
    {
        return constIterator(reinterpret_cast<T**>(mPtrs) + mPtrNum);
    }

    constIterator toConstIterator(s32) const;

    reverseIterator reverseBegin() const;
    reverseIterator reverseEnd() const;

    reverseIterator toReverseIterator(s32) const;

    reverseConstIterator reverseConstBegin() const;
    reverseConstIterator reverseConstEnd() const;

    reverseConstIterator toReverseConstIterator(s32) const;

protected:
    FreeList mFreeList;
};
#ifdef cafe
static_assert(sizeof(ObjArray<s32>) == 0x14, "sead::ObjArray<T> size mismatch");
#endif // cafe

template <typename T, s32 N>
class FixedObjArray : public ObjArray<T>
{
public:
    FixedObjArray()
        : ObjArray<T>()
    {
        ObjArray<T>::setBuffer(N, mWork);
    }

protected:
    void setBuffer(s32 ptrNumMax, void* buf);
    void allocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment = 4);
    bool tryAllocBuffer(s32 ptrNumMax, Heap* heap, s32 alignment = 4);
    void freeBuffer();

protected:
    u8 mWork[N * (sizeof(T) + sizeof(void*))];
};

} // namespace sead

#endif // SEAD_OBJ_ARRAY_H_
