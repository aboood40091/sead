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
        Node(const SafeString& akey, const Value& avalue, StrTreeMap<KeyStrN, Value>* map);

        virtual void erase_();

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

    void setBuffer(s32 node_max, void* buf);
    void allocBuffer(s32 node_max, Heap* heap, s32 alignment = sizeof(void*));
    void freeBuffer();

    Value* find(const SafeString& key) const;
    void insert(const SafeString& key, const Value& value);
    void clear();
    void eraseNode(Node* node);
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
