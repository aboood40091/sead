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

    ListNode* prev = mPrev;
    if (prev != nullptr)
        prev->mNext = mNext;

    ListNode* next = mNext;
    if (next != nullptr)
        next->mPrev = mPrev;

    mNext = nullptr;
    mPrev = nullptr;
}

} // namespace sead
