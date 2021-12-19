#ifndef SEAD_DELEGATE_H_
#define SEAD_DELEGATE_H_

#include <basis/seadTypes.h>

namespace sead {

class Heap;

class IDelegate
{
public:
    virtual void invoke() = 0;
    virtual IDelegate* clone(Heap*) const = 0;
};

template <typename A>
class IDelegate1
{
public:
    virtual void invoke(A a) = 0;
};

template <typename T, typename PTMF, typename Base>
class DelegateBase : public Base
{
public:
    DelegateBase()
        : mObject(NULL)
        , mMethod(NULL)
    {
    }

    DelegateBase(T* o, PTMF m)
        : mObject(o)
        , mMethod(m)
    {
    }

    void bind(T* o, PTMF m)
    {
        mObject = o;
        mMethod = m;
    }

protected:
    T* mObject;
    PTMF mMethod;
};

template <typename T>
class Delegate : public DelegateBase< T, void (T::*)(), IDelegate >
{
public:
    typedef void (T::*MethodPtr)();
    typedef DelegateBase< T, MethodPtr, IDelegate > Base;

public:
    Delegate()
        : Base()
    {
    }

    Delegate(T* o, MethodPtr m)
        : Base(o, m)
    {
    }

    virtual void invoke()
    {
        if (mObject && mMethod)
            return (mObject->*(mMethod))();
    }

    virtual IDelegate* clone(Heap* heap) const
    {
        return new (heap) Delegate(*this);
    }
};

template <typename T, typename A>
class Delegate1 : public DelegateBase< T, void (T::*)(A), IDelegate1<A> >
{
public:
    typedef void (T::*MethodPtr)(A);
    typedef DelegateBase< T, MethodPtr, IDelegate1<A> > Base;

public:
    Delegate1()
        : Base()
    {
    }

    Delegate1(T* o, MethodPtr m)
        : Base(o, m)
    {
    }

    virtual void invoke(A a)
    {
        if (mObject && mMethod)
            return (mObject->*(mMethod))(a);
    }
};

} // namespace sead

#endif // SEAD_DELEGATE_H_
