#include <container/seadListImpl.h>
#include <random/seadRandom.h>

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

void ListImpl::swap(ListNode* n1, ListNode* n2)
{
    //SEAD_ASSERT(n1->mPrev && n1->mNext && n2->mPrev && n2->mNext);
    if (n1 != n2)
    {
        ListNode* n1_prev = n1->mPrev;
        ListNode* n2_prev = n2->mPrev;
        if (n2_prev != n1)
        {
            n1->erase_();
            n2_prev->insertBack_(n1);
        }
        if (n1_prev != n2)
        {
            n2->erase_();
            n1_prev->insertBack_(n2);
        }
    }
}

ListNode* ListImpl::nth(s32 index) const
{
    if ((u32)index < (u32)mCount)
    {
        ListNode* node = mStartEnd.mNext;
        for (s32 i = 0; i < index; i++)
            node = node->mNext;
        return node;
    }
    else
    {
        //SEAD_ASSERT_MSG(false, "index exceeded[%d/%d]", index, mCount);
        return nullptr;
    }
}

void ListImpl::shuffle(Random* random)
{
    //SEAD_ASSERT(random);
    s32 count = mCount;
    while (count > 1)
    {
        s32 index = random->getS32Range(0, count);
        count--;
        swap(nth(count), nth(index));
    }
}

void ListImpl::sort(s32 offset, CompareCallbackImpl cmp)
{
    if (mCount > 1)
    {
        ListNode* node = nth(1);
        while (node != &mStartEnd)
        {
            ListNode* node_next = node->mNext;
            ListNode* node_prev = node->mPrev;

            if (cmp((u8*)node_prev - offset, (u8*)node - offset) > 0)
            {
                while ((node_prev = node_prev->mPrev) != &mStartEnd)
                {
                    if (!(cmp((u8*)node_prev - offset, (u8*)node - offset) > 0))
                        break;
                }

                node->erase_();
                node_prev->insertBack_(node);
            }

            node = node_next;
        }
    }
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

s32 ListImpl::indexOf(const ListNode* n) const
{
    ListNode* node = mStartEnd.mNext;
    s32 index = 0;
    while (node != &mStartEnd)
    {
        if (node == n)
            return index;

        node = node->mNext;
        index++;
    }

    return -1;
}

void ListImpl::moveAfter(ListNode* basis, ListNode* n)
{
    //SEAD_ASSERT(basis->mPrev && basis->mNext && n->mPrev && n->mNext);
    if (basis != n)
    {
        n->erase_();
        basis->insertBack_(n);
    }
}

void ListImpl::moveBefore(ListNode* basis, ListNode* n)
{
    //SEAD_ASSERT(basis->mPrev && basis->mNext && n->mPrev && n->mNext);
    if (basis != n)
    {
        n->erase_();
        basis->insertFront_(n);
    }
}

ListNode* ListImpl::find(const void* ptr, s32 offset, CompareCallbackImpl cmp) const
{
    ListNode* node = mStartEnd.mNext;
    while (node != &mStartEnd)
    {
        if (cmp((u8*)node - offset, ptr) == 0)
            return node;

        node = node->mNext;
    }

    return nullptr;
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
