#ifndef SEAD_DELEGATE_EVENT_SLOT_H_
#define SEAD_DELEGATE_EVENT_SLOT_H_

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <prim/seadDelegate.h>

namespace sead {

template <typename T>
class DelegateEvent
{
public:
    class Slot;

private:
    typedef TList<Slot*> SlotList;
    typedef TListNode<Slot*> SlotListNode;

public:
    class Slot : public IDisposer
    {
    protected:
        SlotListNode mNode;
        IDelegate1<T>* mDelegatePtr;
        struct { u8 data_[20]; } mDelegate;
        bool mConnectedToDelegateEvent;

        friend class DelegateEvent;

    public:
        explicit Slot()
            : IDisposer()
            , m_Node()
            , mDelegatePtr(nullptr)
            , mConnectedToDelegateEvent(false)
        {
        }

        virtual ~Slot()
        {
            release();
        }

        template <typename U>
        explicit Slot(U* u, void (U::*fun)(T))
            : IDisposer()
            , mNode()
            , mDelegatePtr(nullptr)
            , mConnectedToDelegateEvent(false)
        {
            mDelegatePtr = new (&mDelegate) Delegate1<U, T>(u, fun);
        }

        template <typename U>
        void bind(U* u, void (U::*fun)(T))
        {
            mDelegatePtr = new (&mDelegate) Delegate1<U, T>(u, fun);
        }

        void release()
        {
            if (mConnectedToDelegateEvent)
            {
                mNode.erase();
                mConnectedToDelegateEvent = false;
            }
        }

        bool isConnectedToDelegateEvent() const
        {
            return mConnectedToDelegateEvent;
        }

    protected:
        void invoke_(T e)
        {
            if (mDelegatePtr)
                mDelegatePtr->invoke(e);
        }
    };

public:
    explicit DelegateEvent()
        : mList()
    {
    }

    virtual ~DelegateEvent()
    {
        for (typename SlotList::iterator it = mList.begin(); it != mList.end(); )
        {
            Slot* s = *it;
            ++it;
            s->release();
        }
    }

    void connect(Slot& slot)
    {
        if (slot.mConnectedToDelegateEvent)
            slot.release();

        mList.pushBack(slot);
        slot.mConnectedToDelegateEvent = true;
    }

    void disconnect(Slot& slot)
    {
        if (!slot.mConnectedToDelegateEvent)
            return;

        slot.release();
    }

    DelegateEvent<T>& operator+=(Slot& slot)
    {
        connect(slot);
        return *this;
    }

    DelegateEvent<T>& operator-=(Slot& slot)
    {
        disconnect(slot);
        return *this;
    }

    void fire(T e)
    {
        for (typename SlotList::iterator it = mList.begin(); it != mList.end(); ++it)
            (*it)->invoke_(e);
    }

    s32 getSlotLength() const
    {
        return mList.size();
    }

protected:
    SlotList mList;
};
#ifdef cafe
static_assert(sizeof(DelegateEvent<float>) == 0x10, "sead::DelegateEvent<T> size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_DELEGATE_EVENT_SLOT_H_
