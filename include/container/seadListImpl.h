#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

#include <basis/seadTypes.h>

namespace sead {

class Random;

class ListNode
{
public:
    ListNode()
        : mPrev(NULL)
        , mNext(NULL)
    {
    }

private:
    ListNode(const ListNode&);
    const ListNode& operator=(const ListNode&);

public:
    ListNode* next() const { return mNext; }
    ListNode* prev() const { return mPrev; }
    bool isLinked() const { return mNext != NULL && mPrev != NULL; }

private:
    void init_();
    void insertBack_(ListNode* n);
    void insertFront_(ListNode* n);
    void erase_();

private:
    ListNode* mPrev;
    ListNode* mNext;

    friend class ListImpl;
};
#ifdef cafe
static_assert(sizeof(ListNode) == 8, "sead::ListNode size mismatch");
#endif // cafe

class ListImpl
{
private:
    typedef s32 (*CompareCallbackImpl)(const void*, const void*);

public:
    ListImpl()
        : mStartEnd(), mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

protected:
    ListImpl(const ListImpl&);

public:
    bool isEmpty() const;
    s32 size() const;
    void reverse();
    void shuffle();
    void shuffle(Random* random);
    bool checkLinks() const;

protected:
    void sort(s32 offset, CompareCallbackImpl cmp);
    void mergeSort(s32 offset, CompareCallbackImpl cmp);

    void pushBack(ListNode* n)
    {
        mStartEnd.insertFront_(n);
        mCount += 1;
    }

    void pushFront(ListNode* n);
    ListNode* popBack();
    ListNode* popFront();
    void insertBefore(ListNode* basis, ListNode* n);
    void insertAfter(ListNode* basis, ListNode* n);

    void erase(ListNode* n)
    {
        n->erase_();
        mCount -= 1;
    }

    ListNode* front() const;
    ListNode* back() const;
    ListNode* nth(s32 index) const;
    s32 indexOf(const ListNode* n) const;
    void swap(ListNode* n1, ListNode* n2);
    void moveAfter(ListNode* basis, ListNode* n);
    void moveBefore(ListNode* basis, ListNode* n);
    ListNode* find(const void* ptr, s32 offset, CompareCallbackImpl cmp) const;
    void uniq(s32 offset, CompareCallbackImpl cmp);
    void clear();
    void unsafeClear();
    static void mergeSortImpl(ListNode* front, ListNode* back, s32 num, s32 offset, CompareCallbackImpl cmp);
    ListImpl& operator=(const ListImpl&);

protected:
    ListNode mStartEnd;
    s32 mCount;
};
#ifdef cafe
static_assert(sizeof(ListImpl) == 0xC, "sead::ListImpl size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_LIST_IMPL_H_
