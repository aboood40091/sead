#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <container/seadListImpl.h>

namespace sead {

template <typename T>
class OffsetList : public ListImpl
{
private:
    typedef s32 (*CompareCallback)(const T*, const T*);

public:
    OffsetList()
        : ListImpl(), mOffset(-1)
    {
    }

    void initOffset(s32 offset);
    void clear();
    void unsafeClear();

    void pushBack(T* obj)
    {
        //SEAD_ASSRT(mOffset >= 0);
        ListImpl::pushBack(objToListNode(obj));
    }

    void pushFront(T* obj);
    T* popBack();
    T* popFront();
    void insertBefore(const T* basis, T* obj);
    void insertAfter(const T* basis, T* obj);

    void erase(T* obj)
    {
        ListImpl::erase(objToListNode(obj));
    }

    T* front() const;
    T* back() const;
    T* prev(const T* obj) const;
    T* next(const T* obj) const;
    T* nth(s32 index) const;
    s32 indexOf(const T* obj) const;
    bool isNodeLinked(const T* obj) const;
    void swap(T* obj1, T* obj2);
    void moveAfter(T* basis, T* obj);
    void moveBefore(T* basis, T* obj);
    void sort();
    void sort(CompareCallback cmp);
    void mergeSort();
    void mergeSort(CompareCallback cmp);
    T* find(const T* obj) const;
    T* find(const T* obj, CompareCallback cmp) const;
    void uniq();
    void uniq(CompareCallback cmp);

public:
    class iterator
    {
    public:
        iterator(T* ptr, s32 offset)
            : mPtr(ptr), mOffset(offset)
        {
        }

        iterator& operator++()
        {
            ListNode* node = reinterpret_cast<ListNode*>((uintptr_t)mPtr + mOffset)->next();
            mPtr = reinterpret_cast<T*>((uintptr_t)node - mOffset);
            return *this;
        }

        T& operator*() const
        {
            return *mPtr;
        }

        T* operator->() const
        {
            return mPtr;
        }

        friend bool operator==(const iterator& it1, const iterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const iterator& it1, const iterator& it2)
        {
            return !(it1 == it2);
        }

    protected:
        T* mPtr;
        s32 mOffset;
    };

    // TODO
    class constIterator { };

    // TODO
    class robustIterator { };

    // TODO
    class reverseIterator { };

    // TODO
    class reverseConstIterator { };

    // TODO
    class reverseRobustIterator { };

public:
    iterator begin() const
    {
        return iterator(listNodeToObj(mStartEnd.next()), mOffset);
    }

    iterator end() const
    {
        return iterator(listNodeToObj(const_cast<ListNode*>(&mStartEnd)), mOffset);
    }

    iterator toIterator(T*) const;
    constIterator constBegin() const;
    constIterator constEnd() const;
    constIterator toConstIterator(const T*) const;
    robustIterator robustBegin();
    robustIterator robustEnd();
    robustIterator toRobustIterator(T*);
    reverseIterator reverseBegin() const;
    reverseIterator reverseEnd() const;
    reverseIterator toReverseIterator(T*) const;
    reverseConstIterator reverseConstBegin() const;
    reverseConstIterator reverseConstEnd() const;
    reverseConstIterator toReverseConstIterator(const T*) const;
    reverseRobustIterator reverseRobustBegin();
    reverseRobustIterator reverseRobustEnd();
    reverseRobustIterator toReverseRobustIterator(T*);

protected:
    static s32 compareT(const T*, const T*);

    ListNode* objToListNode(const T* obj) const
    {
        return reinterpret_cast<ListNode*>((uintptr_t)obj + mOffset);
    }

    T* listNodeToObj(const ListNode* node) const
    {
        return reinterpret_cast<T*>((uintptr_t)node - mOffset);
    }

    T* listNodeToObjWithNullCheck(const ListNode* node) const;

protected:
    s32 mOffset;
};
#ifdef cafe
static_assert(sizeof(OffsetList<int>) == 0x10, "sead::OffsetList<T> size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_OFFSET_LIST_H_
