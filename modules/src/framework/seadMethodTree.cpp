#include <framework/seadMethodTree.h>
#include <thread/seadCriticalSection.h>

namespace sead {

void
MethodTreeNode::attachMutexRec_(
    CriticalSection* m
) const
{
    const MethodTreeNode* node = this;

    do
    {
        MethodTreeNode* child = static_cast<MethodTreeNode*>(node->mChild);
        node->mCriticalSection = m;
        if (child != nullptr && child->mData != nullptr)
            child->mData->attachMutexRec_(m);
    }
    while (node->mNext != nullptr && (node = static_cast<MethodTreeNode*>(node->mNext)->mData, node != nullptr));
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
    if (mCriticalSection == nullptr)
        return;

    mCriticalSection->lock();
}

void MethodTreeNode::unlock_()
{
    if (mCriticalSection == nullptr)
        return;

    mCriticalSection->unlock();
}

} // namespace sead
