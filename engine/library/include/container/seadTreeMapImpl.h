#ifndef SEAD_TREE_MAP_IMPL_H_
#define SEAD_TREE_MAP_IMPL_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename Key>
class TreeMapNode
{
public:
    TreeMapNode()
    {
        mLeft_ = nullptr;
        mRight_ = nullptr;
        mColor_ = true;
    }

    virtual ~TreeMapNode()
    {
    }

    virtual void erase_() = 0;

    TreeMapNode<Key>* mLeft_;
    TreeMapNode<Key>* mRight_;
    bool mColor_;
    Key mKey_;
};

template <typename Key>
class TreeMapImpl
{
public:
    TreeMapImpl()
        : mRoot(nullptr)
    {
    }

    void insert(TreeMapNode<Key>* node)
    {
        mRoot = insert(mRoot, node);
        mRoot->mColor_ = false;
    }

    TreeMapNode<Key>* insert(TreeMapNode<Key>* h, TreeMapNode<Key>* node);

    TreeMapNode<Key>* find(const Key& key) const
    {
        return find(mRoot, key);
    }

    TreeMapNode<Key>* find(TreeMapNode<Key>* node, const Key& key) const;

    static TreeMapNode<Key>* rotateLeft(TreeMapNode<Key>* h);
    static TreeMapNode<Key>* rotateRight(TreeMapNode<Key>* h);
    static void flipColors(TreeMapNode<Key>* h);
    static bool isRed(TreeMapNode<Key>* h);

    template <typename T> // T = {*}Delegate1<..., TreeMapNode<Key>*>
    void forEach(const T& fun) const
    {
        if (mRoot != nullptr)
            forEach(mRoot, fun);
    }

    template <typename T> // T = {*}Delegate1<..., TreeMapNode<Key>*>
    static void forEach(TreeMapNode<Key>* node, const T& fun)
    {
        if (node->mLeft_ != nullptr)
            forEach(node->mLeft_, fun);

        fun(node);

        if (node->mRight_ != nullptr)
            forEach(node->mRight_, fun);
    }

protected:
    TreeMapNode<Key>* mRoot;
};

} // namespace sead

#ifdef __cplusplus

#include <container/seadTreeMapImpl.hpp>

#endif // __cplusplus

#endif // SEAD_TREE_MAP_IMPL_H_
