#ifndef SEAD_STR_TREE_MAP_H_
#define SEAD_STR_TREE_MAP_H_

#include <container/seadTreeMapImpl.h>
#include <container/seadFreeList.h>
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
            mMap->mFreeList.free(this);
            mMap->mSize--;
        }

    private:
        Value mValue;
        StrTreeMap<KeyStrN, Value>* mMap;
        char mKeyStr[KeyStrN + 1];

        friend class StrTreeMap<KeyStrN, Value>;
    };

public:
    StrTreeMap()
        : mSize(0)
        , mNodeMax(0)
    {
    }

    void setBuffer(s32 node_max, void* buf)
    {
        mNodeMax = node_max;
        mFreeList.init(buf, sizeof(Node), node_max);
    }

    void allocBuffer(s32 node_max, Heap* heap, s32 alignment = sizeof(void*))
    {
        // SEAD_ASSERT(mFreeList.work() == nullptr);
        if (node_max <= 0)
        {
            // SEAD_ASSERT_MSG(false, "node_max[%d] must be larger than zero", node_max);
            // AllocFailAssert(heap, node_max * sizeof(Node), alignment);
        }

        void* buf = new (heap, alignment) u8[node_max * sizeof(Node)];
        if (buf)
            setBuffer(node_max, buf);
    }

    void freeBuffer()
    {
        u8* buf = static_cast<u8*>(mFreeList.work());
        if (buf != nullptr)
        {
            delete[] buf;
            mNodeMax = 0;
            mFreeList.cleanup();
        }
    }

    Value* find(const SafeString& key) const
    {
        Node* node = static_cast<Node*>(TreeMapImpl<SafeString>::find(key));
        return node ? &node->mValue : nullptr;
    }

    void insert(const SafeString& key, const Value& value)
    {
        if (mSize < mNodeMax)
        {
            Node* node = new (mFreeList.get()) Node(key, value, this);
            mSize++;
            TreeMapImpl<SafeString>::insert(node);
        }
        else
        {
            Value* p_value = find(key);
            if (p_value != nullptr)
                *p_value = value;
            else
            {
                // SEAD_ASSERT_MSG(false, "map is full.");
            }
        }
    }

    void clear();

    void eraseNode(Node* node)
    {
        node->erase_();
    }

    void eraseNodeForClear(TreeMapNode<SafeString>* n);

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
    void allocBuffer(s32 node_max, Heap* heap, s32 alignment = sizeof(void*));
    void freeBuffer();

protected:
    u8 mWork[N*sizeof(StrTreeMap<KeyStrN, Value>::Node)];
};

} // namespace sead

#endif // SEAD_STR_TREE_MAP_H_
