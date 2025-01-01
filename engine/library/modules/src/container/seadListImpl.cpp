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

void ListImpl::mergeSortImpl(ListNode* front, ListNode* back, s32 num, s32 offset, CompareCallbackImpl cmp)
{
    if (num > 8)
    {
        s32 front_count;
        ListNode* middle;
        s32 back_count;
        ListNode* merge_top;
        ListNode* back_top;
        bool take_from_front;

        front_count = num >> 1;

        middle = front;
        for (s32 i = 1; i < front_count; i++)
            middle = middle->next();

        back_count = num - front_count;

        merge_top = front->prev();
        back_top = middle->next();
        mergeSortImpl(front, middle, front_count, offset, cmp);
        front = merge_top->next();
        middle = back_top->prev();
        mergeSortImpl(back_top, back, back_count, offset, cmp);
        back_top = middle->next();

        while (front_count > 0 || back_count > 0)
        {
            if (front_count == 0)
                take_from_front = false;
            else if (back_count == 0)
                take_from_front = true;
            else if (cmp((u8*)front - offset, (u8*)back_top - offset) > 0)
                take_from_front = false;
            else
                take_from_front = true;

            if (take_from_front)
            {
                ListNode* temp = front->next();
                front->erase_();
                merge_top->insertBack_(front);
                merge_top = front;
                front = temp;
                front_count--;
            }
            else
            {
                ListNode* temp = back_top->next();
                back_top->erase_();
                merge_top->insertBack_(back_top);
                merge_top = back_top;
                back_top = temp;
                back_count--;
            }
        }
    }
    else if (num >= 2)
    {
        ListNode* start = front->prev();
        ListNode* node = front->next();
        ListNode* end = back->next();

        while (node != end)
        {
            ListNode* next = node->mNext;
            ListNode* prev = node->mPrev;

            if (cmp((u8*)prev - offset, (u8*)node - offset) > 0)
            {
                while ((prev = prev->mPrev) != start)
                {
                    if (cmp((u8*)prev - offset, (u8*)node - offset) <= 0)
                        break;
                }

                node->erase_();
                prev->insertBack_(node);
            }

            node = next;
        }
    }
}

} // namespace sead
