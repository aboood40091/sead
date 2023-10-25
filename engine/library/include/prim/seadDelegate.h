#ifndef SEAD_DELEGATE_H_
#define SEAD_DELEGATE_H_

#include <basis/seadNew.h>

namespace sead {

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

template <typename T, typename R>
class DelegateTraits
{
public:
    typedef R (T::*MethodPtr)();
};

template <typename T, typename R>
class DelegateTraitsConst
{
public:
    typedef R (T::*MethodPtr)() const;
};

template <typename R>
class DelegateTraits<void, R>
{
public:
    typedef R (*MethodPtr)();
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

template <>
class Delegate<void> : public DelegateBase< void, typename DelegateTraits<void, void>::MethodPtr, IDelegate >
{
public:
    typedef Delegate<void> self;
    typedef typename DelegateTraits<void, void>::MethodPtr MethodPtr;

public:
    Delegate()
        : DelegateBase< void, MethodPtr, IDelegate >()
    {
    }

    Delegate(MethodPtr m)
        : DelegateBase< void, MethodPtr, IDelegate >(nullptr, m)
    {
    }

    virtual void invoke()
    {
        if (mMethod)
            (mMethod)();
    }

    void invoke() const
    {
        if (mMethod)
            (mMethod)();
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

template <typename T>
class DelegateConst : public DelegateBase< const T, typename DelegateTraitsConst<T, void>::MethodPtr, IDelegate >
{
public:
    typedef DelegateConst<T> self;
    typedef typename DelegateTraitsConst<T, void>::MethodPtr MethodPtr;

public:
    DelegateConst()
        : DelegateBase< const T, MethodPtr, IDelegate >()
    {
    }

    DelegateConst(const T* o, MethodPtr m)
        : DelegateBase< const T, MethodPtr, IDelegate >(o, m)
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

class StaticDelegate : public Delegate<void>
{
public:
    StaticDelegate()
        : Delegate<void>()
    {
    }

    StaticDelegate(typename Delegate<void>::MethodPtr m)
        : Delegate<void>(m)
    {
    }
};

template <typename T>
inline Delegate<T>
DelegateCreator(T* obj, void (T::*m)())
{
    return Delegate<T>(obj, m);
}

inline Delegate<void>
FunctionDelegateCreator(void (*m)())
{
    return Delegate<void>(m);
}

template <typename T, typename A, typename R>
class DelegateTraits1
{
public:
    typedef R (T::*MethodPtr)(A);
};

template <typename T, typename A, typename R>
class DelegateTraits1Const
{
public:
    typedef R (T::*MethodPtr)(A) const;
};

template <typename A, typename R>
class DelegateTraits1<void, A, R>
{
public:
    typedef R (*MethodPtr)(A);
};

template <typename A>
class IDelegate1
{
public:
    virtual void invoke(A a) = 0;

    void operator() (A a)
    {
        return invoke(a);
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

template <typename A>
class Delegate1<void, A> : public DelegateBase< void, typename DelegateTraits1<void, A, void>::MethodPtr, IDelegate1<A> >
{
public:
    typedef typename DelegateTraits1<void, A, void>::MethodPtr MethodPtr;

public:
    Delegate1()
        : DelegateBase< void, MethodPtr, IDelegate1<A> >()
    {
    }

    Delegate1(MethodPtr m)
        : DelegateBase< void, MethodPtr, IDelegate1<A> >(nullptr, m)
    {
    }

    virtual void invoke(A a)
    {
        if (mMethod)
            (mMethod)(a);
    }

    void invoke(A a) const
    {
        if (mMethod)
            (mMethod)(a);
    }

    void operator() (A a) const
    {
        invoke(a);
    }
};

template <typename T, typename A>
class Delegate1Const : public DelegateBase< const T, typename DelegateTraits1Const<T, A, void>::MethodPtr, IDelegate1<A> >
{
public:
    typedef typename DelegateTraits1Const<T, A, void>::MethodPtr MethodPtr;

public:
    Delegate1Const()
        : DelegateBase< const T, MethodPtr, IDelegate1<A> >()
    {
    }

    Delegate1Const(const T* o, MethodPtr m)
        : DelegateBase< const T, MethodPtr, IDelegate1<A> >(o, m)
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

template<typename A>
class StaticDelegate1 : public Delegate1<void, A>
{
public:
    StaticDelegate1()
        : Delegate1<void, A>()
    {
    }

    StaticDelegate1(typename Delegate1<void, A>::MethodPtr method)
        : Delegate1<void, A>(method)
    {
    }
};

template <typename T, typename A>
inline Delegate1<T, A>
DelegateCreator(T* obj, void (T::*m)(A))
{
    return Delegate1<T, A>(obj, m);
}

template <typename A>
inline Delegate1<void, A>
FunctionDelegateCreator(void (*m)(A))
{
    return Delegate1<void, A>(m);
}

template <typename T, typename A1, typename A2, typename R>
class DelegateTraits2
{
public:
    typedef R (T::*MethodPtr)(A1, A2);
};

template <typename T, typename A1, typename A2, typename R>
class DelegateTraits2Const
{
public:
    typedef R (T::*MethodPtr)(A1, A2) const;
};

template <typename A1, typename A2, typename R>
class DelegateTraits2<void, A1, A2, R>
{
public:
    typedef R (*MethodPtr)(A1, A2);
};

template <typename A1, typename A2>
class IDelegate2
{
public:
    virtual void invoke(A1 a1, A2 a2) = 0;

    void operator() (A1 a1, A2 a2)
    {
        return invoke(a1, a2);
    }
};

template <typename T, typename A1, typename A2>
class Delegate2 : public DelegateBase< T, typename DelegateTraits2<T, A1, A2, void>::MethodPtr, IDelegate2<A1, A2> >
{
public:
    typedef typename DelegateTraits2<T, A1, A2, void>::MethodPtr MethodPtr;

public:
    Delegate2()
        : DelegateBase< T, MethodPtr, IDelegate2<A1, A2> >()
    {
    }

    Delegate2(T* o, MethodPtr m)
        : DelegateBase< T, MethodPtr, IDelegate2<A1, A2> >(o, m)
    {
    }

    virtual void invoke(A1 a1, A2 a2)
    {
        if (mObject && mMethod)
            (mObject->*mMethod)(a1, a2);
    }

    void invoke(A1 a1, A2 a2) const
    {
        if (mObject && mMethod)
            (mObject->*mMethod)(a1, a2);
    }

    void operator() (A1 a1, A2 a2) const
    {
        invoke(a1, a2);
    }
};

template <typename A1, typename A2>
class Delegate2<void, A1, A2> : public DelegateBase< void, typename DelegateTraits2<void, A1, A2, void>::MethodPtr, IDelegate2<A1, A2> >
{
public:
    typedef typename DelegateTraits2<void, A1, A2, void>::MethodPtr MethodPtr;

public:
    Delegate2()
        : DelegateBase< void, MethodPtr, IDelegate2<A1, A2> >()
    {
    }

    Delegate2(MethodPtr m)
        : DelegateBase< void, MethodPtr, IDelegate2<A1, A2> >(nullptr, m)
    {
    }

    virtual void invoke(A1 a1, A2 a2)
    {
        if (mMethod)
            (mMethod)(a1, a2);
    }

    void invoke(A1 a1, A2 a2) const
    {
        if (mMethod)
            (mMethod)(a1, a2);
    }

    void operator() (A1 a1, A2 a2) const
    {
        invoke(a1, a2);
    }
};

template <typename T, typename A1, typename A2>
class Delegate2Const : public DelegateBase< const T, typename DelegateTraits2Const<T, A1, A2, void>::MethodPtr, IDelegate2<A1, A2> >
{
public:
    typedef typename DelegateTraits2Const<T, A1, A2, void>::MethodPtr MethodPtr;

public:
    Delegate2Const()
        : DelegateBase< const T, MethodPtr, IDelegate2<A1, A2> >()
    {
    }

    Delegate2Const(const T* o, MethodPtr m)
        : DelegateBase< const T, MethodPtr, IDelegate2<A1, A2> >(o, m)
    {
    }

    virtual void invoke(A1 a1, A2 a2)
    {
        if (mObject && mMethod)
            (mObject->*mMethod)(a1, a2);
    }

    void invoke(A1 a1, A2 a2) const
    {
        if (mObject && mMethod)
            (mObject->*mMethod)(a1, a2);
    }

    void operator() (A1 a1, A2 a2) const
    {
        invoke(a1, a2);
    }
};

template<typename A1, typename A2>
class StaticDelegate2 : public Delegate2<void, A1, A2>
{
public:
    StaticDelegate2()
        : Delegate2<void, A1, A2>()
    {
    }

    StaticDelegate2(typename Delegate2<void, A1, A2>::MethodPtr method)
        : Delegate2<void, A1, A2>(method)
    {
    }
};

template <typename T, typename A1, typename A2>
inline Delegate2<T, A1, A2>
DelegateCreator(T* obj, void (T::*m)(A1, A2))
{
    return Delegate2<T, A1, A2>(obj, m);
}

template <typename A1, typename A2>
inline Delegate2<void, A1, A2>
FunctionDelegateCreator(void (*m)(A1, A2))
{
    return Delegate2<void, A1, A2>(m);
}

} // namespace sead

#endif // SEAD_DELEGATE_H_
