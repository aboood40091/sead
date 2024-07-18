#ifndef SEAD_ATOMIC_H_
#define SEAD_ATOMIC_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename T>
class AtomicBase32
{
    static_assert(sizeof(T) == sizeof(u32));

public:
    AtomicBase32()
        : mValue()
    {
    }

    AtomicBase32(T val)
        : mValue(val)
    {
    }

    T getValue() const;
    void setValue(T val);

    void setValueNonAtomic(T val)
    {
        mValue = val;
    }

    T add(s32 val);
    T increment();
    T decrement();

    T orMask(T mask);
    T andMask(T mask);
    T xorMask(T mask);

    T swap(T val);
    bool compareAndSwap(T cmp_val, T new_val);
    bool compareAndSwapAndGetOriginal(T cmp_val, T new_val, T* orig_val);

    bool isBitOn(T bitnum) const
    {
        return getValue() & (1 << bitnum);
    }

    bool setBitOn(T bitnum);
    bool setBitOff(T bitnum);

protected:
    T mValue;
};

typedef AtomicBase32<u32> AtomicU32;

template <typename T>
class AtomicBase64
{
    static_assert(sizeof(T) == sizeof(u64));

public:
    AtomicBase64()
        : mValue()
    {
    }

    AtomicBase64(T val)
        : mValue(val)
    {
    }

    T getValue() const;
    void setValue(T val);

    void setValueNonAtomic(T val)
    {
        mValue = val;
    }

    T add(s64 val);
    T increment();
    T decrement();

    T orMask(T mask);
    T andMask(T mask);
    T xorMask(T mask);

    T swap(T val);
    bool compareAndSwap(T cmp_val, T new_val);
    bool compareAndSwapAndGetOriginal(T cmp_val, T new_val, T* orig_val);

    bool isBitOn(u32 bitnum) const
    {
        return getValue() & (1LL << bitnum);
    }

    bool setBitOn(u32 bitnum);
    bool setBitOff(u32 bitnum);

protected:
    T mValue;
};

typedef AtomicBase64<u64> AtomicU64;

template <typename T>
class AtomicPtr
{
    static_assert(sizeof(T) == sizeof(void*));

public:
    AtomicPtr()
        : mValue(nullptr)
    {
    }

    AtomicPtr(T val)
        : mValue(val)
    {
    }

    T getValue() const;
    void setValue(T val);

    void setValueNonAtomic(T val)
    {
        mValue = val;
    }

    T swap(T val);
    bool compareAndSwap(T cmp_val, T new_val);
    bool compareAndSwapAndGetOriginal(T cmp_val, T new_val, T* orig_val);

protected:
    T mValue;
};

} // namespace sead

#ifdef __cplusplus

#ifdef cafe
#include <thread/cafe/seadAtomicCafe.hpp>
#else
#error "Unknown platform"
#endif // cafe

#endif // __cplusplus

#endif // SEAD_ATOMIC_H_