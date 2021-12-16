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

void TreeNode::pushBackSibling(TreeNode* n)
{
    n->detachSubTree();

    TreeNode* node = this;
    while (node->mNext != NULL)
        node = node->mNext;

    node->mNext = n;
    n->mPrev = node;
    n->mParent = node->mParent;
}

void TreeNode::clearChildLinksRecursively_()
{
    TreeNode* node = this->mChild;
    while (node != NULL)
    {
        TreeNode* n = node;
        node = node->mNext;
        n->clearChildLinksRecursively_();
        n->clearLinks();
    }
}

} // namespace sead
