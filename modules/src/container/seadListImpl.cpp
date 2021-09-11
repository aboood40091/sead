#include <container/seadListImpl.h>

namespace sead {

void ListNode::insertFront_(ListNode* n)
{
    //SEAD_ASSERT_MSG(n->mNext == NULL && n->mPrev == NULL, "node is already linked.");

    ListNode* prev = mPrev;

    this->mPrev = n;
    n->mPrev = prev;
    n->mNext = this;

    if (prev == NULL)
        return;

    prev->mNext = n;
}

void ListNode::erase_()
{
    //SEAD_ASSERT_MSG(n->mNext != NULL || n->mPrev != NULL, "node is not linked.");

    ListNode* prev = mPrev;
    if (prev != NULL)
        prev->mNext = mNext;

    ListNode* next = mNext;
    if (next != NULL)
        next->mPrev = mPrev;

    mNext = NULL;
    mPrev = NULL;
}

} // namespace sead
