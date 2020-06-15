#ifndef SEAD_TLIST_H_
#define SEAD_TLIST_H_

#include <container/seadListImpl.h>

namespace sead {

template <typename T>
class TListNode;

template <typename T>
class TList : public ListImpl
{
public:
    TList()
        : ListImpl()
    {
    }

    class iterator
    {
    public:
        iterator(TListNode<T>* ptr)
            : mPtr(ptr)
        {
        }

        iterator& operator++()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->mNext);
            return *this;
        }

        iterator operator++(int)
        {
            const iterator it(*this);
            (void)++*this;
            return it;
        }

        iterator& operator--()
        {
            mPtr = static_cast<TListNode<T>*>(mPtr->mPrev);
            return *this;
        }

        iterator operator--(int)
        {
            const iterator it(*this);
            (void)--*this;
            return it;
        }

        friend bool operator==(iterator it1, iterator it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(iterator it1, iterator it2)
        {
            return !(it1 == it2);
        }

        TListNode<T>* mPtr;
    };

    iterator begin() const
    {
        return iterator(static_cast<TListNode<T>*>(mStartEnd.mNext));
    }

    iterator end() const
    {
        return iterator(static_cast<TListNode<T>*>(const_cast<ListNode*>(&mStartEnd)));
    }

    void pushBack(TListNode<T>* obj)
    {
        obj->erase();
        obj->mList = this;
        ListImpl::pushBack(obj);
    }

    void erase(TListNode<T>* obj)
    {
        obj->mList = NULL;
        ListImpl::erase(obj);
    }
};

template <typename T>
class TListNode : public ListNode
{
public:
    TListNode()
        : ListNode()
    {
        mData = static_cast<T>(this);
        mList = NULL;
    }

    void erase()
    {
        TList<T>* list = mList;
        if (list != NULL)
            list->erase(this);
    }

    T mData;
    TList<T>* mList;
};

} // namespace sead

#endif // SEAD_TLIST_H_
