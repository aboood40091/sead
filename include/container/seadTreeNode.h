#ifndef SEAD_TREENODE_H_
#define SEAD_TREENODE_H_

#include <basis/seadTypes.h>

namespace sead {

class TreeNode
{
public:
    TreeNode();

    void clearLinks();
    void detachAll();
    void detachSubTree();
    void pushBackSibling(TreeNode* n);
    void pushBackChild(TreeNode* n);
    void pushFrontChild(TreeNode* n);
    void insertBeforeSelf(TreeNode* n);
    void insertAfterSelf(TreeNode* n);
    bool isRoot() const { return mParent == NULL; }
    bool isLeaf() const { return mChild == NULL; }
    const TreeNode* findRoot() const;
    TreeNode* findRoot();
    s32 countChildren() const;

protected:
    void clearChildLinksRecursively_();

protected:
    TreeNode* mParent;
    TreeNode* mChild;
    TreeNode* mNext;
    TreeNode* mPrev;
};
#ifdef cafe
static_assert(sizeof(TreeNode) == 0x10, "sead::TreeNode size mismatch");
#endif // cafe

template <typename T>
class TTreeNode : public TreeNode
{
public:
    TTreeNode()
        : TreeNode()
        , mData(NULL)
    {
    }

    TTreeNode(T ptr)
        : TreeNode()
        , mData(ptr)
    {
    }

    TTreeNode<T>* parent() const { return static_cast<TTreeNode<T>*>(mParent); }
    TTreeNode<T>* child() const { return static_cast<TTreeNode<T>*>(mChild); }
    TTreeNode<T>* next() const { return static_cast<TTreeNode<T>*>(mNext); }
    TTreeNode<T>* prev() const { return static_cast<TTreeNode<T>*>(mPrev); }
    TTreeNode<T>* findRoot() { return static_cast<TTreeNode<T>*>(TreeNode::findRoot()); }
    const TTreeNode<T>* findRoot() const { return static_cast<TTreeNode<T>*>(TreeNode::findRoot()); }
    void pushBackSibling(TTreeNode<T>* o) { TreeNode::pushBackSibling(o); }
    void pushBackChild(TTreeNode<T>* o) { TreeNode::pushBackChild(o); }
    void pushFrontChild(TTreeNode<T>* o) { TreeNode::pushFrontChild(o); }
    void insertBeforeSelf(TTreeNode<T>* o) { TreeNode::insertBeforeSelf(o); }
    void insertAfterSelf(TTreeNode<T>* o) { TreeNode::insertAfterSelf(o); }
    T& val() { return mData; }
    const T& val() const { return mData; }

public:
    // TODO
    class nextIterator { };

    // TODO
    class nextConstIterator { };

    // TODO
    class depthFirstIterator { };

    // TODO
    class depthFirstConstIterator { };

public:
    nextIterator nextBegin() const;
    nextConstIterator nextConstBegin() const;
    nextIterator nextEnd() const;
    nextConstIterator nextConstEnd() const;
    nextIterator childBegin() const;
    nextConstIterator childConstBegin() const;
    nextIterator childEnd() const;
    nextConstIterator childConstEnd() const;
    depthFirstIterator depthFirstBegin() const;
    depthFirstConstIterator depthFirstConstBegin() const;
    depthFirstIterator depthFirstWithoutRootBegin() const;
    depthFirstConstIterator depthFirstWithoutRootConstBegin() const;
    depthFirstIterator depthFirstEnd() const;
    depthFirstConstIterator depthFirstConstEnd() const;

protected:
    T mData;
};
#ifdef cafe
static_assert(sizeof(TTreeNode<int*>) == 0x14, "sead::TTreeNode<T> size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_TREENODE_H_
