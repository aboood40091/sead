#pragma once

#include <math/seadMathCalcCommon.h>

#define _USE_MATH_DEFINES
#include <cmath>

#ifdef M_PI
#define F_PI f32(M_PI)
#else
//#define F_PI std::acos(-1.0f) <-- Does not match
#define F_PI 3.1415927410125732f
#endif // M_PI

namespace sead {

template <typename T>
inline s32
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

template <>
inline f32
MathCalcCommon<f32>::deg2rad(f32 deg)
{
    return deg * (F_PI / 180.0f);
}

} // namespace sead

#ifdef cafe

#include <math/cafe/seadMathCalcCommonCafe.hpp>

namespace sead {

template <typename T>
inline T
MathCalcCommon<T>::rsqrt(T x)
{
    return MathCafe<T>::rsqrt(x);
}

} // namespace sead

#endif // cafe
