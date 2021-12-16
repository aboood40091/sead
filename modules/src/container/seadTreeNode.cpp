#include <container/seadTreeNode.h>

namespace sead {

TreeNode::TreeNode()
{
    clearLinks();
}

void TreeNode::clearLinks()
{
    mParent = mChild = mNext = mPrev = NULL;
}

void TreeNode::pushBackChild(TreeNode* n)
{
    n->detachSubTree();

    if (mChild != NULL)
    {
        mChild->pushBackSibling(n);
        return;
    }

    mChild = n;
    n->mParent = this;
}

void TreeNode::pushBackSibling(TreeNode* n)
{
    n->detachSubTree();

    TreeNode* node = this;
    while (node->mNext != NULL) { node = node->mNext; }

    node->mNext = n;
    n->mPrev = node;

    n->mParent = node->mParent;
}

void TreeNode::pushFrontChild(TreeNode* n)
{
    n->detachSubTree();

    if (mChild != NULL)
    {
        n->mNext = mChild;
        mChild->mPrev = n;
    }

    mChild = n;
    n->mParent = this;
}

void TreeNode::insertBeforeSelf(TreeNode* n)
{
    n->detachSubTree();

    TreeNode* prev = mPrev;
    mPrev = n;

    n->mPrev = prev;
    n->mNext = this;

    if (prev != NULL)
    {
        prev->mNext = n;
    }
    else if (mParent != NULL)
    {
        mParent->mChild = n;
    }

    n->mParent = mParent;
}

void TreeNode::insertAfterSelf(TreeNode* n)
{
    n->detachSubTree();

    TreeNode* next = mNext;
    mNext = n;

    n->mPrev = this;
    n->mNext = next;

    if (next != NULL)
    {
        next->mPrev = n;
    }

    n->mParent = mParent;
}

void TreeNode::detachAll()
{
    detachSubTree();
    clearChildLinksRecursively_();
    clearLinks();
}

void TreeNode::detachSubTree()
{
    if (mPrev != NULL)
    {
        mPrev->mNext = mNext;
        if (mNext != NULL)
        {
            mNext->mPrev = mPrev;
            mNext = NULL;
        }
        mPrev = NULL;
        mParent = NULL;
    }
    else
    {
        if (mParent != NULL)
        {
            mParent->mChild = mNext;
            mParent = NULL;
        }
        if (mNext != NULL)
        {
            mNext->mPrev = mPrev;
            mNext = NULL;
        }
    }
}

TreeNode* TreeNode::findRoot()
{
    TreeNode* node = this;
    TreeNode* p = node->mParent;
    while (p != NULL)
    {
        //SEAD_ASSERT(p != this);
        node = p;
        p = p->mParent;
    }

    return node;
}

const TreeNode* TreeNode::findRoot() const
{
    const TreeNode* node = this;
    const TreeNode* p = node->mParent;
    while (p != NULL)
    {
        //SEAD_ASSERT(p != this);
        node = p;
        p = p->mParent;
    }

    return node;
}

s32 TreeNode::countChildren() const
{
    s32 i = 0;

    TreeNode* node = mChild;
    while (node != NULL)
    {
        i++;
        node = node->mNext;
    }

    return i;
}

void TreeNode::clearChildLinksRecursively_()
{
    TreeNode* node = mChild;
    while (node != NULL)
    {
        TreeNode* n = node;
        node = node->mNext;
        n->clearChildLinksRecursively_();
        n->clearLinks();
    }
}

} // namespace sead
