#ifndef SEAD_STR_TREE_MAP_H_
#define SEAD_STR_TREE_MAP_H_

#include <container/seadTreeMapImpl.h>
#include <container/seadFreeList.h>
#include <prim/seadDelegate.h>
#include <prim/seadSafeString.h>

namespace sead {

class Heap;

template <s32 KeyStrN, typename Value>
class StrTreeMap : public TreeMapImpl<SafeString>
{
public:
    class Node : public TreeMapNode<SafeString>
    {
    public:
        Node(const SafeString& akey, const Value& avalue, StrTreeMap<KeyStrN, Value>* map)
            : mValue(avalue)
            , mMap(map)
        {
            BufferedSafeString key(mKeyStr, KeyStrN + 1);
            key.copy(akey);
            mKey_ = key;
        }

        virtual void erase_()
        {
            mMap->eraseNode(this);
        }

        SafeString& key()
        {
            return mKey_;
        }

        Value& value()
        {
            return mValue;
        }

    private:
        Value mValue;
        StrTreeMap<KeyStrN, Value>* mMap;
        char mKeyStr[KeyStrN + 1];
    };

public:
    template <typename T> // T = {*}Delegate2<..., SafeString&, Value&>
    class ForEachConstContext
    {
    public:
        ForEachConstContext(const T& afun)
            : fun(afun)
        {
        }

        void call(TreeMapNode<SafeString>* n)
        {
            Node* node = static_cast<Node*>(n);
            fun(node->key(), node->value());
        }

    private:
        const T& fun;
    };

public:
    StrTreeMap()
        : mSize(0)
        , mNodeMax(0)
    {
    }

    void setBuffer(s32 node_max, void* buf)
    {
        if (node_max > 0)
        {
            if (buf != nullptr)
            {
                mNodeMax = node_max;
                mFreeList.init(buf, sizeof(Node), node_max);
            }
            else
            {
                // SEAD_ASSERT_MSG(false, "buf is null");
            }
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "node_max[%d] must be larger than zero", node_max);
        }
    }

    void allocBuffer(s32 node_max, Heap* heap, s32 alignment = 4)
    {
        // SEAD_ASSERT(mFreeList.work() == nullptr);
        if (node_max > 0)
        {
            void* buf = new (heap, alignment) u8[node_max * sizeof(Node)]; // NewArray<u8>(node_max * sizeof(Node), heap, alignment)
            setBuffer(node_max, buf);
        }
        else
        {
            // SEAD_ASSERT_MSG(false, "node_max[%d] must be larger than zero", node_max);
        }
    }

    void freeBuffer()
    {
        if (isBufferReady())
        {
            clear();
            delete[] static_cast<u8*>(mFreeList.work()); // DeleteArray<u8>(mFreeList.work())
            mNodeMax = 0;
            mFreeList.cleanup();
        }
    }

    bool isBufferReady() const
    {
        return mFreeList.work() != nullptr;
    }

    bool isEmpty() const
    {
        return mSize == 0;
    }

    bool isFull() const
    {
        return mSize >= mNodeMax;
    }

    s32 size() const
    {
        return mSize;
    }

    s32 maxSize() const
    {
        return mNodeMax;
    }

    Value* find(const SafeString& key) const
    {
        Node* node = static_cast<Node*>(TreeMapImpl<SafeString>::find(key));
        if (node != nullptr)
            return &node->value();

        return nullptr;
    }

    bool contains(const SafeString& key) const
    {
        return find(key) != nullptr;
    }

    void insert(const SafeString& key, const Value& value)
    {
        if (!isFull())
        {
            Node* node = new (mFreeList.get()) Node(key, value, this);
            mSize++;
            TreeMapImpl<SafeString>::insert(node);
        }
        else
        {
            Value* p_value = find(key);
            if (p_value != nullptr)
                new (p_value) Value(value); // ???
            else
            {
                // SEAD_ASSERT_MSG(false, "map is full.");
            }
        }
    }

    void clear()
    {
        TreeMapImpl<SafeString>::forEach(
            DelegateCreator<
                StrTreeMap<KeyStrN, Value>,
                TreeMapNode<SafeString>*
            >(
                this,
                &StrTreeMap<KeyStrN, Value>::eraseNodeForClear
            )
        );
        mSize = 0;
        mRoot = nullptr;
        mFreeList.init(mFreeList.work(), sizeof(Node), mNodeMax);
    }

    void eraseNode(Node* node)
    {
        mFreeList.put(node);
        mSize--;
    }

    void eraseNodeForClear(TreeMapNode<SafeString>* n)
    {
        static_cast<Node*>(n)->value(); // ... ?
    }

    template <typename T> // T = {*}Delegate2<..., SafeString&, Value&>
    void forEach(const T& fun) const
    {
        ForEachConstContext<T> context(fun);

        TreeMapImpl<SafeString>::forEach(
            DelegateCreator<
                ForEachConstContext<T>,
                TreeMapNode<SafeString>*
            >(
                &context,
                &ForEachConstContext<T>::call
            )
        );
    }

protected:
    FreeList mFreeList;
    s32 mSize;
    s32 mNodeMax;
};

template <s32 KeyStrN, typename Value, s32 N>
class FixedStrTreeMap : public StrTreeMap<KeyStrN, Value>
{
public:
    FixedStrTreeMap()
        : StrTreeMap<KeyStrN, Value>()
    {
        StrTreeMap<KeyStrN, Value>::setBuffer(N, mWork);
    }

protected:
    void setBuffer(s32 node_max, void* buf);
    void allocBuffer(s32 node_max, Heap* heap, s32 alignment = 4);
    void freeBuffer();

protected:
    u8 mWork[N*sizeof(StrTreeMap<KeyStrN, Value>::Node)];
};

} // namespace sead

#endif // SEAD_STR_TREE_MAP_H_
