#ifndef SEAD_OBJ_LIST_H_
#define SEAD_OBJ_LIST_H_

#include <basis/seadNew.h>
#include <container/seadFreeList.h>
#include <container/seadListImpl.h>
#include <prim/seadPtrUtil.h>

#include <cstddef>

namespace sead {

class Heap;

template <typename T>
class ObjList : public ListImpl
{
protected:
    class Node
    {
    public:
        Node()
        {
        }

        Node(const T& obj)
            : item(obj)
        {
        }

        T item;
        ListNode link;
    };

private:
    typedef s32 (*CompareCallback)(const T*, const T*);

public:
    ObjList()
        : ListImpl()
        , mFreeList()
        , mLimitNum(0)
    {
    }

    ObjList(s32 limitNum, void* buf)
        : ListImpl()
        , mFreeList()
        , mLimitNum(0)
    {
        setBuffer(limitNum, buf);
    }

    void setBuffer(s32 limitNum, void* buf)
    {
        if (limitNum <= 0)
        {
            // SEAD_ASSERT_MSG(false, "limitNum[%d] must be larger than zero", limitNum);
            return;
        }

        if (!buf)
        {
            // SEAD_ASSERT_MSG(false, "buf is null");
            return;
        }

        mFreeList.init(buf, sizeof(Node), limitNum);
        mLimitNum = limitNum;
    }

    void allocBuffer(s32 limitNum, Heap* heap, s32 alignment = 4);
    bool tryAllocBuffer(s32 limitNum, Heap* heap, s32 alignment = 4);
    void freeBuffer();

    bool isBufferReady() const { return mFreeList.work() != nullptr; }
    bool isFull() const { return mCount >= mLimitNum; }

    s32 maxSize() const { return mLimitNum; }

    T* birthBack()
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new (mFreeList.get()) Node;
        ListImpl::pushBack(&node->link);
        return &node->item;
    }

    T* birthFront()
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new (mFreeList.get()) Node;
        ListImpl::pushFront(&node->link);
        return &node->item;
    }

    T* birthBefore(T* basis)
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new (mFreeList.get()) Node;
        ListImpl::insertBefore(objToListNode(basis), &node->link);
        return &node->item;
    }

    T* birthAfter(T* basis)
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new (mFreeList.get()) Node;
        ListImpl::insertAfter(objToListNode(basis), &node->link);
        return &node->item;
    }

    T* pushBack(const T& obj)
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new (mFreeList.get()) Node(obj);
        ListImpl::pushBack(&node->link);
        return &node->item;
    }

    T* pushFront(const T& obj)
    {
        if (isFull())
        {
            // SEAD_ASSERT_MSG(false, "buffer full.");
            return nullptr;
        }

        Node* node = new (mFreeList.get()) Node(obj);
        ListImpl::pushFront(&node->link);
        return &node->item;
    }

    void popBack()
    {
        T* obj = back();
        if (obj != nullptr)
            erase(obj);
    }

    void popFront()
    {
        T* obj = front();
        if (obj != nullptr)
            erase(obj);
    }

    void erase(T* obj)
    {
        ListImpl::erase(objToListNode(obj));
        obj->~T();
        mFreeList.put(obj);
    }

    void clear()
    {
        ListNode* node = mStartEnd.next();
        while (node != &mStartEnd)
        {
            ListNode* next = node->next();
            ListImpl::erase(node);
            T* obj = listNodeToObj(node);
            obj->~T();
            mFreeList.put(obj);
            node = next;
        }
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

    T* nth(s32 index) const { return listNodeToObjWithNullCheck(ListImpl::nth(index)); }
    s32 indexOf(const T* obj) const { return ListImpl::indexOf(objToListNode(obj)); }
    bool isNodeLinked(const T* obj) const { return objToListNode(obj)->isLinked(); }
    void swap(T* obj1, T* obj2) { ListImpl::swap(objToListNode(obj1), objToListNode(obj2)); }
    void moveAfter(T* basis, T* obj) { ListImpl::moveAfter(objToListNode(basis), objToListNode(obj)); }
    void moveBefore(T* basis, T* obj) { ListImpl::moveBefore(objToListNode(basis), objToListNode(obj)); }
    void sort() { sort(compareT); }
    void sort(CompareCallback cmp) { ListImpl::sort(cOffset, (s32 (*)(const void*, const void*))cmp); }
    void mergeSort() { mergeSort(compareT); }
    void mergeSort(CompareCallback cmp) { ListImpl::mergeSort(cOffset, (s32 (*)(const void*, const void*))cmp); }
    T* find(const T* obj) const { return find(obj, compareT); }
    T* find(const T* obj, CompareCallback cmp) const { return listNodeToObjWithNullCheck(ListImpl::find(obj, cOffset, (s32 (*)(const void*, const void*))cmp)); }
    void uniq() { uniq(compareT); }
    void uniq(CompareCallback cmp) { ListImpl::uniq(cOffset, (s32 (*)(const void*, const void*))cmp); }

public:
    class iterator
    {
    public:
        iterator(T* ptr)
            : mPtr(ptr)
        {
        }

        iterator& operator++()
        {
            ListNode* next = reinterpret_cast<ListNode*>(PtrUtil::addOffset(mPtr, cOffset))->next();
            mPtr = reinterpret_cast<T*>(PtrUtil::addOffset(next, -cOffset));
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

        friend class constIterator;
    };

    class constIterator
    {
    public:
        constIterator(const T* ptr)
            : mPtr(ptr)
        {
        }

        constIterator(const iterator& it)
            : mPtr(it.mPtr)
        {
        }

        constIterator& operator++()
        {
            const ListNode* next = static_cast<const ListNode*>(PtrUtil::addOffset(mPtr, cOffset))->next();
            mPtr = static_cast<const T*>(PtrUtil::addOffset(next, -cOffset));
            return *this;
        }

    public:
        const T& operator*() const
        {
            return *mPtr;
        }

        const T* operator->() const
        {
            return mPtr;
        }

        friend bool operator==(const constIterator& it1, const constIterator& it2)
        {
            return it1.mPtr == it2.mPtr;
        }

        friend bool operator!=(const constIterator& it1, const constIterator& it2)
        {
            return it1.mPtr != it2.mPtr;
        }

    protected:
        const T* mPtr;
    };

    class robustIterator
    {
    public:
        robustIterator(T* ptr)
            : mPtr(ptr)
            , mNext(reinterpret_cast<ListNode*>(PtrUtil::addOffset(ptr, cOffset))->next())
        {
        }

        robustIterator& operator++()
        {
            mPtr = reinterpret_cast<T*>(PtrUtil::addOffset(mNext, -cOffset));
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
    };

public:
    iterator begin() const
    {
        return iterator(listNodeToObj(mStartEnd.next()));
    }

    iterator end() const
    {
        return iterator(listNodeToObj(&mStartEnd));
    }

    iterator toIterator(T* obj) const
    {
        return iterator(obj);
    }

    constIterator constBegin() const
    {
        return constIterator(listNodeToObj(mStartEnd.next()));
    }

    constIterator constEnd() const
    {
        return constIterator(listNodeToObj(&mStartEnd));
    }

    constIterator toConstIterator(const T* obj) const
    {
        return constIterator(obj);
    }

    robustIterator robustBegin()
    {
        return robustIterator(listNodeToObj(mStartEnd.next()));
    }

    robustIterator robustEnd()
    {
        return robustIterator(listNodeToObj(&mStartEnd));
    }

    robustIterator toRobustIterator(T* obj)
    {
        return robustIterator(obj);
    }

protected:
    static s32 compareT(const T* a, const T* b)
    {
        if (*a < *b)
            return -1;

        if (*b < *a)
            return 1;

        return 0;
    }

    static ListNode* objToListNode(const T* obj)
    {
        return reinterpret_cast<ListNode*>(PtrUtil::addOffset(obj, cOffset));
    }

    static T* listNodeToObj(const ListNode* node)
    {
        return reinterpret_cast<T*>(PtrUtil::addOffset(node, -cOffset));
    }

    static T* listNodeToObjWithNullCheck(const ListNode* node)
    {
        return node ? listNodeToObj(node) : nullptr;
    }

protected:
    static const s32 cOffset = offsetof(Node, link);

protected:
    FreeList mFreeList;
    s32 mLimitNum;
};
#ifdef cafe
static_assert(sizeof(ObjList<s32>) == 0x18, "sead::ObjList<T> size mismatch");
#endif // cafe

template <typename T, s32 N>
class FixedObjList : public ObjList<T>
{
public:
    FixedObjList()
        : ObjList<T>()
    {
        ObjList<T>::setBuffer(N, mWork);
    }

protected:
    void setBuffer(s32 limitNum, void* buf);
    void allocBuffer(s32 limitNum, Heap* heap, s32 alignment = 4);
    bool tryAllocBuffer(s32 limitNum, Heap* heap, s32 alignment = 4);
    void freeBuffer();

protected:
    u8 mWork[N * sizeof(typename ObjList<T>::Node)];
};

} // namespace sead

#endif // SEAD_OBJ_LIST_H_
