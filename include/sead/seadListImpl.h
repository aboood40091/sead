#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

namespace sead {

class ListNode
{
public:
    __attribute__((always_inline)) ListNode()
        : mPrev(NULL)
        , mNext(NULL)
    {
    }

    void insertFront_(ListNode* node);
    void erase_();

    ListNode* mPrev;
    ListNode* mNext;
};

class ListImpl
{
public:
    __attribute__((always_inline)) ListImpl()
        : mStartEnd(), mCount(0)
    {
        mStartEnd.mNext = &mStartEnd;
        mStartEnd.mPrev = &mStartEnd;
    }

    void pushBack(ListNode* n)
    {
        mStartEnd.insertFront_(n);
        mCount += 1;
    }

    void erase(ListNode* n)
    {
        n->erase_();
        mCount -= 1;
    }

    ListNode mStartEnd;
    s32 mCount;
};

} // namespace sead

#endif // SEAD_LIST_IMPL_H_
