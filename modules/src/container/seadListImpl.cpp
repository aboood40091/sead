#include <container/seadListImpl.h>

namespace sead {

void ListNode::insertFront_(ListNode* node)
{
    ListNode* prev = mPrev;
    this->mPrev = node;
    node->mPrev = prev;
    node->mNext = this;
    if (prev == NULL)
        return;

    prev->mNext = node;
}

void ListNode::erase_()
{
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
