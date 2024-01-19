#include <framework/seadMethodTree.h>
#include <thread/seadCriticalSection.h>

namespace sead {

void
MethodTreeNode::attachMutexRec_(
    CriticalSection* m
) const
{
    mCriticalSection = m;

    MethodTreeNode* child = getChild();
    if (child != nullptr)
        child->attachMutexRec_(m);

    MethodTreeNode* next = getNext();
    if (next != nullptr)
        next->attachMutexRec_(m);
}

void MethodTreeNode::detachAll()
{
    CriticalSection* cs = mCriticalSection;
    attachMutexRec_(nullptr);
    mCriticalSection = cs;

    lock_();
    TreeNode::detachAll();
    unlock_();

    mCriticalSection = nullptr;
}

void MethodTreeNode::lock_()
{
    if (mCriticalSection != nullptr)
        mCriticalSection->lock();
}

void MethodTreeNode::unlock_()
{
    if (mCriticalSection != nullptr)
        mCriticalSection->unlock();
}

} // namespace sead
