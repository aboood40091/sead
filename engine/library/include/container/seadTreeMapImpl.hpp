#pragma once

namespace sead {

template <typename Key>
TreeMapNode<Key>*
TreeMapImpl<Key>::insert(Node* h, Node* node)
{
    if (h == nullptr)
    {
        node->mColor_ = Node::cRed_;
        node->mLeft_ = nullptr;
        node->mRight_ = nullptr;
    }
    else
    {
        s32 cmp = node->mKey_.compare(h->mKey_);
        if (cmp < 0)
        {
            h->mLeft_ = insert(h->mLeft_, node);
            node = h;
        }
        else if (cmp > 0)
        {
            h->mRight_ = insert(h->mRight_, node);
            node = h;
        }
        else
        {
            if (h != node)
            {
                node->mRight_ = h->mRight_;
                node->mLeft_ = h->mLeft_;
                node->mColor_ = h->mColor_;
                h->erase_();
            }
        }
        if (isRed(node->mRight_) && !isRed(node->mLeft_))
        {
            node = rotateLeft(node);
        }
        if (isRed(node->mLeft_) && isRed(node->mLeft_->mLeft_))
        {
            node = rotateRight(node);
        }
        if (isRed(node->mLeft_) && isRed(node->mRight_))
        {
            flipColors(node);
        }
    }
    return node;
}

template <typename Key>
TreeMapNode<Key>*
TreeMapImpl<Key>::find(Node* node, const Key& key) const
{
    while (node != nullptr)
    {
        s32 cmp = key.compare(node->mKey_);
        if (cmp < 0)
            node = node->mLeft_;
        else if (cmp > 0)
            node = node->mRight_;
        else
            return node;
    }
    return nullptr;
}

template <typename Key>
inline TreeMapNode<Key>*
TreeMapImpl<Key>::rotateLeft(Node* h)
{
    Node* n = h->mRight_;
    h->mRight_ = n->mLeft_;
    n->mLeft_ = h;
    n->mColor_ = h->mColor_;
    h->mColor_ = Node::cRed_;
    return n;
}

template <typename Key>
inline TreeMapNode<Key>*
TreeMapImpl<Key>::rotateRight(Node* h)
{
    Node* n = h->mLeft_;
    h->mLeft_ = n->mRight_;
    n->mRight_ = h;
    n->mColor_ = h->mColor_;
    h->mColor_ = Node::cRed_;
    return n;
}

template <typename Key>
inline void
TreeMapImpl<Key>::flipColors(Node* h)
{
    h->mColor_ = !h->mColor_;
    h->mLeft_->mColor_ = !h->mLeft_->mColor_;
    h->mRight_->mColor_ = !h->mRight_->mColor_;
}

template <typename Key>
inline bool
TreeMapImpl<Key>::isRed(Node* h)
{
    if (h == nullptr)
        return false;
    return h->mColor_ == Node::cRed_;
}

} // namespace sead
