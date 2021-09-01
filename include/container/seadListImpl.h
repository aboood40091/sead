#ifndef SEAD_LIST_IMPL_H_
#define SEAD_LIST_IMPL_H_

#include <basis/seadTypes.h>

namespace sead {

class ListNode
{
public:
    ListNode()
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
    ListImpl()
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
