#pragma once

#ifdef cafe
#include <math/cafe/seadMathCalcCommonCafe.h>
#endif // cafe

namespace sead {

template <>
inline s32
MathCalcCommon<s32>::roundUpPow2(s32 val, s32 base)
{
    //SEAD_ASSERT_MSG(val >= 0 && (base - 1u & base) == 0, "illegal param[val:%d, base:%d]", val, base);
    return val + base - 1 & (u32)-base;
}

template <>
inline s32
MathCalcCommon<u32>::roundUpPow2(u32 val, s32 base)
{
    //SEAD_ASSERT_MSG((base - 1u & base) == 0, "illegal param[base:%d]", base);
    return val + base - 1 & (u32)-base;
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
