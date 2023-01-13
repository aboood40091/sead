#ifndef SEAD_DELEGATE_H_
#define SEAD_DELEGATE_H_

#include <basis/seadNew.h>

namespace sead {

template <typename T, typename R>
class DelegateTraits
{
public:
    typedef R (T::*MethodPtr)();
};

template <typename T, typename A, typename R>
class DelegateTraits1
{
public:
    typedef R (T::*MethodPtr)(A);
};

class IDelegate
{
public:
    virtual void invoke() = 0;

    void operator() ()
    {
        return invoke();
    }

    virtual IDelegate* clone(Heap*) const = 0;
};

template <typename A>
class IDelegate1
{
public:
    virtual void invoke(A a) = 0;

    void operator() (A a)
    {
        return Invoke(a);
    }
};

template <typename T, typename METHODPTR, typename IDELEGATE>
class DelegateBase : public IDELEGATE
{
private:
    typedef METHODPTR MethodPtr;

protected:
    T* mObject;
    METHODPTR mMethod;

public:
    DelegateBase()
        : mObject(nullptr)
        , mMethod(nullptr)
    {
    }

    DelegateBase(T* o, METHODPTR m)
        : mObject(o)
        , mMethod(m)
    {
    }

    void bind(T* o, METHODPTR m)
    {
        mObject = o;
        mMethod = m;
    }

    void bind(T* o)
    {
        mObject = o;
    }

    void bind(METHODPTR m)
    {
        mMethod = m;
    }

    void unbind()
    {
        mObject = nullptr;
        mMethod = nullptr;
    }

    T* getObject() const
    {
        return mObject;
    }
};

template <typename T>
class Delegate : public DelegateBase< T, typename DelegateTraits<T, void>::MethodPtr, IDelegate >
{
public:
    typedef Delegate<T> self;
    typedef typename DelegateTraits<T, void>::MethodPtr MethodPtr;

public:
    Delegate()
        : DelegateBase< T, MethodPtr, IDelegate >()
    {
    }

    Delegate(T* o, MethodPtr m)
        : DelegateBase< T, MethodPtr, IDelegate >(o, m)
    {
    }

    virtual void invoke()
    {
        if (mObject && mMethod)
            (mObject->*mMethod)();
    }

    void invoke() const
    {
        if (mObject && mMethod)
            (mObject->*mMethod)();
    }

    void operator() () const
    {
        invoke();
    }

    virtual IDelegate* clone(Heap* heap) const
    {
        return new (heap) self(*this);
    }
};

template <typename T, typename A>
class Delegate1 : public DelegateBase< T, typename DelegateTraits1<T, A, void>::MethodPtr, IDelegate1<A> >
{
public:
    typedef typename DelegateTraits1<T, A, void>::MethodPtr MethodPtr;

public:
    Delegate1()
        : DelegateBase< T, MethodPtr, IDelegate1<A> >()
    {
    }

    Delegate1(T* o, MethodPtr m)
        : DelegateBase< T, MethodPtr, IDelegate1<A> >(o, m)
    {
    }

    virtual void invoke(A a)
    {
        if (mObject && mMethod)
            (mObject->*mMethod)(a);
    }

    void invoke(A a) const
    {
        if (mObject && mMethod)
            (mObject->*mMethod)(a);
    }

    void operator() (A a) const
    {
        invoke(a);
    }
};

} // namespace sead

#endif // SEAD_DELEGATE_H_
