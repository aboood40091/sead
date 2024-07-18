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
        mColor_ = cRed_;
    }

    virtual ~TreeMapNode()
    {
    }

    virtual void erase_() = 0;

    TreeMapNode<Key>* mLeft_;
    TreeMapNode<Key>* mRight_;
    bool mColor_;
    Key mKey_;

    static const bool cRed_ = true;
    static const bool cBlack_ = false;
};

template <typename Key>
class TreeMapImpl
{
public:
    typedef TreeMapNode<Key> Node;

public:
    TreeMapImpl()
        : mRoot(nullptr)
    {
    }

    void insert(Node* node)
    {
        mRoot = insert(mRoot, node);
        mRoot->mColor_ = Node::cBlack_;
    }

    Node* insert(Node* h, Node* node);

    Node* find(const Key& key) const
    {
        return find(mRoot, key);
    }

    Node* find(Node* node, const Key& key) const;

    bool contains(const Key& key) const
    {
        return find(key) != nullptr;
    }

    static Node* rotateLeft(Node* h);
    static Node* rotateRight(Node* h);
    static void flipColors(Node* h);
    static bool isRed(Node* h);

    template <typename T> // T = {*}Delegate1<..., TreeMapNode<Key>*>
    void forEach(const T& fun) const
    {
        if (mRoot != nullptr)
            forEach(mRoot, fun);
    }

    template <typename T> // T = {*}Delegate1<..., TreeMapNode<Key>*>
    static void forEach(Node* node, const T& fun)
    {
        if (node->mLeft_ != nullptr)
            forEach(node->mLeft_, fun);

        fun(node);

        if (node->mRight_ != nullptr)
            forEach(node->mRight_, fun);
    }

protected:
    Node* mRoot;
};

} // namespace sead

#ifdef __cplusplus

#include <container/seadTreeMapImpl.hpp>

#endif // __cplusplus

#endif // SEAD_TREE_MAP_IMPL_H_
