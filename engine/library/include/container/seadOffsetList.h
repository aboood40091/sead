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

    void initOffset(s32 offset) { mOffset = offset; }

    void clear()
    {
        ListImpl::clear();
    }

    void unsafeClear()
    {
        ListImpl::unsafeClear();
    }

    void pushBack(T* obj)
    {
        // SEAD_ASSRT(mOffset >= 0);
        ListImpl::pushBack(objToListNode(obj));
    }

    void pushFront(T* obj)
    {
        // SEAD_ASSRT(mOffset >= 0);
        ListImpl::pushFront(objToListNode(obj));
    }

    T* popBack() { return listNodeToObjWithNullCheck(ListImpl::popBack()); }
    T* popFront() { return listNodeToObjWithNullCheck(ListImpl::popFront()); }

    void insertBefore(const T* basis, T* obj)
    {
        ListImpl::insertBefore(objToListNode(basis), objToListNode(obj));
    }

    void insertAfter(const T* basis, T* obj)
    {
        ListImpl::insertAfter(objToListNode(basis), objToListNode(obj));
    }

    void erase(T* obj)
    {
        ListImpl::erase(objToListNode(obj));
    }

    T* front() const { return listNodeToObjWithNullCheck(ListImpl::front()); }
    T* back() const { return listNodeToObjWithNullCheck(ListImpl::back()); }

    T* prev(const T* obj) const
    {
        ListNode* node = objToListNode(obj)->prev();
        if (node == &mStartEnd)
            return nullptr;

        return listNodeToObj(node);
    }

    T* next(const T* obj) const
    {
        ListNode* node = objToListNode(obj)->next();
        if (node == &mStartEnd)
            return nullptr;

        return listNodeToObj(node);
    }

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
            : mPtr(ptr)
            , mOffset(offset)
        {
        }

        iterator& operator++()
        {
            ListNode* next = reinterpret_cast<ListNode*>(PtrUtil::addOffset(mPtr, mOffset))->next();
            mPtr = reinterpret_cast<T*>(PtrUtil::addOffset(next, -mOffset));
            return *this;
        }

    public:
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
            return it1.mPtr != it2.mPtr;
        }

    protected:
        T* mPtr;
        s32 mOffset;
    };

    // TODO
    class constIterator { };

    class robustIterator
    {
    public:
        robustIterator(T* ptr, s32 offset)
            : mPtr(ptr)
            , mNext(reinterpret_cast<ListNode*>(PtrUtil::addOffset(ptr, offset))->next())
            , mOffset(offset)
        {
        }

        robustIterator& operator++()
        {
            mPtr = reinterpret_cast<T*>(PtrUtil::addOffset(mNext, -mOffset));
            mNext = mNext->next();
            return *this;
        }

    public:
        T& operator*() const
        {
            return *mPtr;
        }

        T* operator->() const
        {
            return mPtr;
        }

        friend bool operator==(const robustIterator& it1, const robustIterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const robustIterator& it1, const robustIterator& it2)
        {
            return it1.mPtr != it2.mPtr;
        }

    protected:
        T* mPtr;
        ListNode* mNext;
        s32 mOffset;
    };

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
        return iterator(listNodeToObj(&mStartEnd), mOffset);
    }

    iterator toIterator(T*) const;

    constIterator constBegin() const;
    constIterator constEnd() const;

    constIterator toConstIterator(const T*) const;

    robustIterator robustBegin()
    {
        return robustIterator(listNodeToObj(mStartEnd.next()), mOffset);
    }

    robustIterator robustEnd()
    {
        return robustIterator(listNodeToObj(&mStartEnd), mOffset);
    }

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
        return reinterpret_cast<ListNode*>(PtrUtil::addOffset(obj, mOffset));
    }

    T* listNodeToObj(const ListNode* node) const
    {
        return reinterpret_cast<T*>(PtrUtil::addOffset(node, -mOffset));
    }

    T* listNodeToObjWithNullCheck(const ListNode* node) const
    {
        return node ? listNodeToObj(node) : nullptr;
    }

protected:
    s32 mOffset;
};
#ifdef cafe
static_assert(sizeof(OffsetList<int>) == 0x10, "sead::OffsetList<T> size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_OFFSET_LIST_H_
