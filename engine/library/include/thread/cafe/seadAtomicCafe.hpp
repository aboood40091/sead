#pragma once

#include <cafe/os/OSAtomic.h>

namespace sead {

template <typename T>
inline T
AtomicBase32<T>::getValue() const
{
    // I hope this is correct
    return *reinterpret_cast<const volatile T*>(&mValue);
}

template <typename T>
inline void
AtomicBase32<T>::setValue(T val)
{
    // I hope this is correct
    *reinterpret_cast<volatile T*>(&mValue) = val;
}

template <typename T>
inline T
AtomicBase32<T>::add(s32 val)
{
    return OSAddAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), val);
}

template <typename T>
inline T
AtomicBase32<T>::increment()
{
    return OSIncAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue));
}

template <typename T>
inline T
AtomicBase32<T>::decrement()
{
    return OSDecAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue));
}

template <typename T>
inline T
AtomicBase32<T>::orMask(T mask)
{
    return OSOrAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), mask);
}

template <typename T>
inline T
AtomicBase32<T>::andMask(T mask)
{
    return OSAndAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), mask);
}

template <typename T>
inline T
AtomicBase32<T>::xorMask(T mask)
{
    return OSXorAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), mask);
}

template <typename T>
inline T
AtomicBase32<T>::swap(T val)
{
    return OSSwapAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), val);
}

template <typename T>
inline bool
AtomicBase32<T>::compareAndSwap(T cmp_val, T new_val)
{
    return OSCompareAndSwapAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), cmp_val, new_val);
}

template <typename T>
inline bool
AtomicBase32<T>::compareAndSwapAndGetOriginal(T cmp_val, T new_val, T* orig_val)
{
    return OSCompareAndSwapAtomicEx(reinterpret_cast<volatile OSAtomicVar*>(&mValue), cmp_val, new_val, reinterpret_cast<u32*>(orig_val));
}

template <typename T>
inline bool
AtomicBase32<T>::setBitOn(T bitnum)
{
    return OSTestAndSetAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), bitnum);
}

template <typename T>
inline bool
AtomicBase32<T>::setBitOff(T bitnum)
{
    return OSTestAndClearAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), bitnum);
}

template <typename T>
inline T
AtomicBase64<T>::getValue() const
{
    return OSGetAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue));
}

template <typename T>
inline void
AtomicBase64<T>::setValue(T val)
{
    OSSetAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), val);
}

template <typename T>
inline T
AtomicBase64<T>::add(s64 val)
{
    return OSAddAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), val);
}

template <typename T>
inline T
AtomicBase64<T>::increment()
{
    return OSIncAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue));
}

template <typename T>
inline T
AtomicBase64<T>::decrement()
{
    return OSDecAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue));
}

template <typename T>
inline T
AtomicBase64<T>::orMask(T mask)
{
    return OSOrAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), mask);
}

template <typename T>
inline T
AtomicBase64<T>::andMask(T mask)
{
    return OSAndAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), mask);
}

template <typename T>
inline T
AtomicBase64<T>::xorMask(T mask)
{
    return OSXorAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), mask);
}

template <typename T>
inline T
AtomicBase64<T>::swap(T val)
{
    return OSSwapAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), val);
}

template <typename T>
inline bool
AtomicBase64<T>::compareAndSwap(T cmp_val, T new_val)
{
    return OSCompareAndSwapAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), cmp_val, new_val);
}

template <typename T>
inline bool
AtomicBase64<T>::compareAndSwapAndGetOriginal(T cmp_val, T new_val, T* orig_val)
{
    return OSCompareAndSwapAtomicEx64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), cmp_val, new_val, reinterpret_cast<u64*>(orig_val));
}

template <typename T>
inline bool
AtomicBase64<T>::setBitOn(u32 bitnum)
{
    return OSTestAndSetAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), bitnum);
}

template <typename T>
inline bool
AtomicBase64<T>::setBitOff(u32 bitnum)
{
    return OSTestAndClearAtomic64(reinterpret_cast<volatile OSAtomicVar64*>(&mValue), bitnum);
}

template <typename T>
inline T
AtomicPtr<T>::getValue() const
{
    // I hope this is correct
    return *reinterpret_cast<const volatile T*>(&mValue);
}

template <typename T>
inline void
AtomicPtr<T>::setValue(T val)
{
    // I hope this is correct
    *reinterpret_cast<volatile T*>(&mValue) = val;
}

template <typename T>
inline T
AtomicPtr<T>::swap(T val)
{
    return (T)OSSwapAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), val);
}

template <typename T>
inline bool
AtomicPtr<T>::compareAndSwap(T cmp_val, T new_val)
{
    return OSCompareAndSwapAtomic(reinterpret_cast<volatile OSAtomicVar*>(&mValue), (u32)cmp_val, (u32)new_val);
}

template <typename T>
inline bool
AtomicPtr<T>::compareAndSwapAndGetOriginal(T cmp_val, T new_val, T* orig_val)
{
    return OSCompareAndSwapAtomicEx(reinterpret_cast<volatile OSAtomicVar*>(&mValue), (u32)cmp_val, (u32)new_val, (u32*)orig_val);
}

} // namespace sead
