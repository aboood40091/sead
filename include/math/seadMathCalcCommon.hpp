#pragma once

#ifdef cafe
#include <math/cafe/seadMathCalcCommonCafe.h>
#endif // cafe

namespace sead {

template <typename T>
inline T
MathCalcCommon<T>::roundUpPow2(T x, s32 y)
{
    return x + y - 1 & (u32)-y;
}

template <typename T>
inline T
MathCalcCommon<T>::cos(T t)
{
    return std::cos(t);
}

template <typename T>
inline T
MathCalcCommon<T>::sin(T t)
{
    return std::sin(t);
}

template <typename T>
inline T
MathCalcCommon<T>::sqrt(T x)
{
#ifdef cafe
    return rsqrt(x) * x;
#else
    return std::sqrt(x);
#endif // cafe
}

template <typename T>
inline T
MathCalcCommon<T>::rsqrt(T x)
{
#ifdef cafe
    return MathCafe<T>::rsqrt(x);
#else
    return 1 / std::sqrt(x);
#endif // cafe
}

} // namespace sead
