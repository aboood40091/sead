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
        return node;
    }

    s32 cmp = node->mKey_.compare(h->mKey_);
    if (cmp < 0)
        h->mLeft_ = insert(h->mLeft_, node);
    else if (cmp > 0)
        h->mRight_ = insert(h->mRight_, node);
    else
    {
        if (h != node)
        {
            node->mRight_ = h->mRight_;
            node->mLeft_ = h->mLeft_;
            node->mColor_ = h->mColor_;
            h->erase_();
        }
        h = node;
    }

    if (isRed(h->mRight_) && !isRed(h->mLeft_))
        h = rotateLeft(h);

    if (isRed(h->mLeft_) && isRed(h->mLeft_->mLeft_))
        h = rotateRight(h);

    if (isRed(h->mLeft_) && isRed(h->mRight_))
        flipColors(h);

    return h;
}

template <typename Key>
TreeMapNode<Key>*
TreeMapImpl<Key>::erase(Node* h, const Key& key)
{
    if (key.compare(h->mKey_) < 0)
    {
        if (!isRed(h->mLeft_) && !isRed(h->mLeft_->mLeft_))
            h = moveRedLeft(h);

        h->mLeft_ = erase(h->mLeft_, key);
    }
    else
    {
        if (isRed(h->mLeft_))
            h = rotateRight(h);

        if (key.compare(h->mKey_) == 0 && h->mRight_ == nullptr)
        {
            h->erase_();
            return nullptr;
        }

        if (!isRed(h->mRight_) && !isRed(h->mRight_->mLeft_))
            h = moveRedRight(h);

        if (key.compare(h->mKey_) == 0)
        {
            Node* node = find(h->mRight_, min(h->mRight_)->mKey_);
            node->mRight_ = eraseMin(h->mRight_);
            node->mLeft_ = h->mLeft_;
            node->mColor_ = h->mColor_;
            h->erase_();
            h = node;
        }
        else
        {
            h->mRight_ = erase(h->mRight_, key);
        }
    }

    return fixUp(h);
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
TreeMapImpl<Key>::min(Node* h)
{
    while (h->mLeft_ != nullptr)
        h = h->mLeft_;

    return h;
}

template <typename Key>
inline TreeMapNode<Key>*
TreeMapImpl<Key>::eraseMin(Node* h)
{
    if (h->mLeft_ == nullptr)
        return nullptr;

    if (!isRed(h->mLeft_) && !isRed(h->mLeft_->mLeft_))
        h = moveRedLeft(h);

    h->mLeft_ = eraseMin(h->mLeft_);

    return fixUp(h);
}

template <typename Key>
inline TreeMapNode<Key>*
TreeMapImpl<Key>::moveRedLeft(Node* h)
{
    flipColors(h);
    if (isRed(h->mRight_->mLeft_))
    {
        h->mRight_ = rotateRight(h->mRight_);
        h = rotateLeft(h);
        flipColors(h);
    }
    return h;
}

template <typename Key>
inline TreeMapNode<Key>*
TreeMapImpl<Key>::moveRedRight(Node* h)
{
    flipColors(h);
    if (isRed(h->mLeft_->mLeft_))
    {
        h = rotateRight(h);
        flipColors(h);
    }
    return h;
}

template <typename Key>
inline TreeMapNode<Key>*
TreeMapImpl<Key>::fixUp(Node* h)
{
    if (isRed(h->mRight_))
        h = rotateLeft(h);

    if (isRed(h->mLeft_) && isRed(h->mLeft_->mLeft_))
        h = rotateRight(h);

    if (isRed(h->mLeft_) && isRed(h->mRight_))
        flipColors(h);

    return h;
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
