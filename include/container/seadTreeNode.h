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
    bool isRoot() const;
    bool isLeaf() const;
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

template <typename T>
class TTreeNode : public TreeNode
{
public:
    TTreeNode();
    TTreeNode(T);

    TTreeNode<T>* parent() const { return static_cast<TTreeNode<T>*>(mParent); }
    TTreeNode<T>* child() const { return static_cast<TTreeNode<T>*>(mChild); }
    TTreeNode<T>* next() const { return static_cast<TTreeNode<T>*>(mNext); }
    TTreeNode<T>* prev() const { return static_cast<TTreeNode<T>*>(mPrev); }
    TTreeNode<T>* findRoot();
    const TTreeNode<T>* findRoot() const;
    void pushBackSibling(TTreeNode<T>*);
    void pushBackChild(TTreeNode<T>*);
    void pushFrontChild(TTreeNode<T>*);
    void insertBeforeSelf(TTreeNode<T>*);
    void insertAfterSelf(TTreeNode<T>*);
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

} // namespace sead

#endif // SEAD_TREENODE_H_
