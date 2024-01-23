#include <container/seadListImpl.h>

namespace sead {

void ListNode::insertBack_(ListNode* n)
{
    // SEAD_ASSERT_MSG(!n->isLinked(), "node is already linked.");

    ListNode* next = mNext;

    mNext = n;
    n->mPrev = this;
    n->mNext = next;

    if (next)
        next->mPrev = n;
}

void ListNode::insertFront_(ListNode* n)
{
    // SEAD_ASSERT_MSG(!n->isLinked(), "node is already linked.");

    ListNode* prev = mPrev;

    mPrev = n;
    n->mPrev = prev;
    n->mNext = this;

    if (prev)
        prev->mNext = n;
}

void ListNode::erase_()
{
    // SEAD_ASSERT_MSG(n->isLinked(), "node is not linked.");

    if (mPrev != nullptr)
        mPrev->mNext = mNext;

    if (mNext != nullptr)
        mNext->mPrev = mPrev;

    mNext = nullptr;
    mPrev = nullptr;
}

ListNode* ListImpl::popBack()
{
    if (mCount > 0)
    {
        ListNode* n = mStartEnd.mPrev;
        n->erase_();
        mCount--;
        return n;
    }
    else
    {
        return nullptr;
    }
}

ListNode* ListImpl::popFront()
{
    if (mCount > 0)
    {
        ListNode* n = mStartEnd.mNext;
        n->erase_();
        mCount--;
        return n;
    }
    else
    {
        return nullptr;
    }
}

void ListImpl::clear()
{
    ListNode* node = mStartEnd.mNext;
    while (node != &mStartEnd)
    {
        ListNode* n = node;
        node = node->mNext;
        n->init_();
    }
    mCount = 0;
    mStartEnd.mNext = &mStartEnd;
    mStartEnd.mPrev = &mStartEnd;
}

} // namespace sead
