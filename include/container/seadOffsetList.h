#ifndef SEAD_OFFSET_LIST_H_
#define SEAD_OFFSET_LIST_H_

#include <container/seadListImpl.h>

namespace sead {

template <typename T>
class OffsetList : public ListImpl
{
public:
    OffsetList()
        : ListImpl(), mOffset(-1)
    {
    }

    void pushBack(T* item)
    {
        ListImpl::pushBack(objToListNode(item));
    }

    void erase(T* item)
    {
        ListImpl::erase(objToListNode(item));
    }

    class iterator
    {
    public:
        iterator(T* ptr, s32 offset)
            : mPtr(ptr), mOffset(offset)
        {
        }

        bool operator==(const iterator& other) const
        {
            return mPtr == other.mPtr;
        }

        bool operator!=(const iterator& other) const
        {
            return !(*this == other);
        }

        iterator& operator++()
        {
            ListNode* node = static_cast<ListNode*>(static_cast<void*>(mPtr) + mOffset)->mNext;
            mPtr = static_cast<T*>(static_cast<void*>(node) + -mOffset);
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

        T* mPtr;
        s32 mOffset;
    };

    iterator begin() const
    {
        return iterator(listNodeToObj(mStartEnd.mNext), mOffset);
    }

    iterator end() const
    {
        return iterator(listNodeToObj(const_cast<ListNode*>(&mStartEnd)), mOffset);
    }

    ListNode* objToListNode(T* obj) const
    {
        return static_cast<ListNode*>(static_cast<void*>(obj) + mOffset);
    }

    const ListNode* objToListNode(const T* obj) const
    {
        return static_cast<const ListNode*>(static_cast<const void*>(obj) + mOffset);
    }

    T* listNodeToObj(ListNode* node) const
    {
        return static_cast<T*>(static_cast<void*>(node) + -mOffset);
    }

    const T* listNodeToObj(const ListNode* node) const
    {
        return static_cast<const T*>(static_cast<const void*>(node) + -mOffset);
    }

    s32 mOffset;
};

} // namespace sead

#endif // SEAD_OFFSET_LIST_H_
