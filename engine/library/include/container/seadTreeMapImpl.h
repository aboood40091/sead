#ifndef SEAD_TREE_MAP_IMPL_H_
#define SEAD_TREE_MAP_IMPL_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename Key>
class TreeMapNode
{
public:
    TreeMapNode();
    virtual ~TreeMapNode();

    TreeMapNode<Key>* mLeft_;
    TreeMapNode<Key>* mRight_;
    bool mColor_;
    Key mKey_;
};

template <typename Key>
class TreeMapImpl
{
public:
    TreeMapImpl();

    void insert(TreeMapNode<Key>*);
    TreeMapNode<Key>* insert(TreeMapNode<Key>*, TreeMapNode<Key>*);
    TreeMapNode<Key>* find(const Key& key) const { return find(mRoot, key); }
    TreeMapNode<Key>* find(TreeMapNode<Key>*, const Key&) const;
    static TreeMapNode<Key>* rotateLeft(TreeMapNode<Key>*);
    static TreeMapNode<Key>* rotateRight(TreeMapNode<Key>*);
    static void flipColors(TreeMapNode<Key>*);
    static bool isRed(TreeMapNode<Key>*);

protected:
    TreeMapNode<Key>* mRoot;
};

} // namespace sead

#endif // SEAD_TREE_MAP_IMPL_H_
