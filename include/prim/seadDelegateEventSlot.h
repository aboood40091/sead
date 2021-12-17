#ifndef SEAD_DELEGATE_EVENT_SLOT_H_
#define SEAD_DELEGATE_EVENT_SLOT_H_

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
//#include <prim/seadDelegate.h>

namespace sead {

template <typename T>
class IDelegate1;

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
    public:
        template <typename U>
        Slot(U*, void (U::*)(T)); /*
            : IDisposer()
            , mNode()
            , mDelegatePtr(NULL)
            , mConnectedToDelegateEvent(false)
        {
            mDelegatePtr = new (mDelegate) Delegate1<U, T>();
        }
        */

        virtual ~Slot()
        {
            release();
        }

        void release()
        {
            if (mConnectedToDelegateEvent)
            {
                mNode.erase();
                mConnectedToDelegateEvent = false;
            }
        }

        SlotListNode mNode;
        IDelegate1<T>* mDelegatePtr;
        struct { u8 data_[20]; } mDelegate;
        bool mConnectedToDelegateEvent;
    };

public:
    DelegateEvent()
        : mList()
    {
    }

    virtual ~DelegateEvent()
    {
        for (SlotList::iterator it = mList.begin(); it != mList.end(); ++it)
            (*it)->release();
    }

    void connect(Slot&);
    void disconnect(Slot&);
    DelegateEvent<T>& operator+=(Slot&);
    DelegateEvent<T>& operator-=(Slot&);
    void fire(T);
    s32 getSlotLength() const;

protected:
    SlotList mList;
};
#ifdef cafe
static_assert(sizeof(DelegateEvent<float>) == 0x10, "sead::DelegateEvent<T> size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_DELEGATE_EVENT_SLOT_H_
