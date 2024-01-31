#ifndef SEAD_METHODTREE_H_
#define SEAD_METHODTREE_H_

#include <container/seadTreeNode.h>
#include <heap/seadDisposer.h>
#include <prim/seadBitFlag.h>
//#include <prim/seadDelegate.h>
#include <prim/seadNamable.h>
#include <prim/seadRuntimeTypeInfo.h>
//#include <thread/seadCriticalSection.h>

namespace sead {

class CriticalSection;

template <typename T, typename U>
class IDelegate2;

class MethodTreeNode : public TTreeNode<MethodTreeNode*>, public INamable, public IDisposer
{
    SEAD_RTTI_BASE(MethodTreeNode)

public:
    enum PauseFlag
    {
        cPause_None = 0,
        cPause_Self = 1,
        cPause_Child = 2,
        cPause_Both = 3
    };

public:
    MethodTreeNode(CriticalSection* cs); /*
        : TTreeNode<MethodTreeNode*>()
        , INamable()
        , IDisposer()
        , mCriticalSection(cs)
        , mPauseFlag(0)
        , mPauseEventDelegate(nullptr)
        , mUserID(nullptr)
    {
        new (mDelegateHolder) Delegate<aaaaaaaaaaa>();
        mPauseFlag = BitFlag32(0);
    } */

    virtual ~MethodTreeNode()
    {
        detachAll();
    }

    MethodTreeNode* getChild() const
    {
        if (child() != nullptr)
            return child()->val();
        else
            return nullptr;
    }

    MethodTreeNode* getNext() const
    {
        if (next() != nullptr)
            return next()->val();
        else
            return nullptr;
    }

    void attachMutexRec_(CriticalSection* m) const;
    void detachAll();
    void lock_();
    void unlock_();

    void call();

    typedef IDelegate2<MethodTreeNode*, PauseFlag> PauseEventDelegate;

    struct { u32 data_[4]; } mDelegateHolder;
    mutable CriticalSection* mCriticalSection;
    u32 mPriority;
    BitFlag32 mPauseFlag;
    PauseEventDelegate* mPauseEventDelegate;
    void* mUserID;
};
#ifdef cafe
static_assert(sizeof(MethodTreeNode) == 0x54, "sead::MethodTreeNode size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_METHODTREE_H_
