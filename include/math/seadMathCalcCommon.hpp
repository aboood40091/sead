#pragma once

#ifdef cafe
#include <math/cafe/seadMathCalcCommonCafe.h>
#endif // cafe

#include <cmath>
#include <limits>

namespace sead {

template <typename T>
inline T
MathCalcCommon<T>::sqrt(T t)
{
    return std::sqrt(t);
}

template <typename T>
inline T
MathCalcCommon<T>::rsqrt(T t)
{
    return 1 / std::sqrt(t);
}

#ifdef cafe

template <>
inline f32
MathCalcCommon<f32>::sqrt(f32 t)
{
    return MathCafe<f32>::rsqrt(t) * t;
}

template <>
inline f32
MathCalcCommon<f32>::rsqrt(f32 t)
{
    return MathCafe<f32>::rsqrt(t);
}

#endif // cafe

template <typename T>
inline T
MathCalcCommon<T>::pow(T x, T y)
{
    return std::pow(x, y);
}

template <typename T>
inline T
MathCalcCommon<T>::sin(T t)
{
    return std::sin(t);
}

template <typename T>
inline T
MathCalcCommon<T>::cos(T t)
{
    return std::cos(t);
}

template <typename T>
inline T
MathCalcCommon<T>::tan(T t)
{
    return std::tan(t);
}

template <typename T>
inline T
MathCalcCommon<T>::asin(T s)
{
    return std::asin(s);
}

template <typename T>
inline T
MathCalcCommon<T>::acos(T c)
{
    return std::acos(c);
}

template <typename T>
inline T
MathCalcCommon<T>::atan(T t)
{
    return std::atan(t);
}

template <typename T>
inline T
MathCalcCommon<T>::atan2(T y, T x)
{
    return std::atan2(y, x);
}

template <>
inline f32
MathCalcCommon<f32>::sinIdx(u32 idx)
{
    u32 index = (idx >> 24) & 0xff;
    u32 rest = idx & 0xffffff;

    return cSinCosTbl[index].sin_val + cSinCosTbl[index].sin_delta * rest / 0x1000000;
}

template <>
inline f32
MathCalcCommon<f32>::cosIdx(u32 idx)
{
    u32 index = (idx >> 24) & 0xff;
    u32 rest = idx & 0xffffff;

    return cSinCosTbl[index].cos_val + cSinCosTbl[index].cos_delta * rest / 0x1000000;
}

template <>
inline f32
MathCalcCommon<f32>::tanIdx(u32 idx)
{
    u32 index = (idx >> 24) & 0xff;
    f32 rest = static_cast<f32>(idx & 0xffffff) / 0x1000000;
    const SinCosSample& sample = cSinCosTbl[index];

    return (sample.sin_val + sample.sin_delta * rest) / (sample.cos_val + sample.cos_delta * rest);
}

template <>
inline u32
MathCalcCommon<f32>::asinIdx(f32 s)
{
    //SEAD_ASSERT_MSG(s <= 1 && s >= -1, "s(%f) is not in [-1, 1].", s);

    const f32 rsqrt_2 = 0.7071067690849304f; // rsqrt(2)

    if (s >= 0)
    {
        if (s > rsqrt_2)
            return 0x40000000 - atanIdx_(sqrt(1 - s * s) / s);

        else
            return atanIdx_(s / sqrt(1 - s * s));
    }
    else
    {
        if (s < -rsqrt_2)
            return 0xC0000000 + atanIdx_(-sqrt(1 - s * s) / s);

        else
            return -atanIdx_(-s / sqrt(1 - s * s));
    }
}

template <>
inline u32
MathCalcCommon<f32>::acosIdx(f32 c)
{
    //SEAD_ASSERT_MSG(c <= 1 && c >= -1, "c(%f) is not in [-1, 1].", c);

    const f32 rsqrt_2 = 0.7071067690849304f; // rsqrt(2)

    if (c >= 0)
    {
        if (c > rsqrt_2)
            return atanIdx_(sqrt(1 - c * c) / c);

        else
            return 0x40000000 - atanIdx_(c / sqrt(1 - c * c));
    }
    else
    {
        if (c < -rsqrt_2)
            return 0x80000000 - atanIdx_(-sqrt(1 - c * c) / c);

        else
            return 0x40000000 + atanIdx_(-c / sqrt(1 - c * c));
    }
}

template <>
inline u32
MathCalcCommon<f32>::atanIdx(f32 t)
{
    if (t >= 0)
    {
        if (t > 1)
            return 0x40000000 - atanIdx_(1 / t);

        else
            return atanIdx_(t);
    }
    else
    {
        if (t < -1)
            return 0xC0000000 + atanIdx_(-1 / t);

        else
            return -atanIdx_(-t);
    }
}

template <>
inline u32
MathCalcCommon<f32>::atan2Idx(f32 y, f32 x)
{
    if (x == 0 && y == 0)
        return 0;

    if (x >= 0)
    {
        if (y >= 0)
        {
            if (x >= y)
                return atanIdx_(y / x);

            else
                return 0x40000000 - atanIdx_(x / y);
        }
        else
        {
            if (x >= -y)
                return -atanIdx_(-y / x);

            else
                return 0xC0000000 + atanIdx_(x / -y);
        }
    }
    else
    {
        if (y >= 0)
        {
            if (-x >= y)
                return 0x80000000 - atanIdx_(y / -x);

            else
                return 0x40000000 + atanIdx_(-x / y);
        }
        else
        {
            if (x <= y)
                return 0x80000000 + atanIdx_(y / x);

            else
                return 0xC0000000 - atanIdx_(x / y);
        }
    }
}

template <>
inline void
MathCalcCommon<f32>::sinCosIdx(f32* pSin, f32* pCos, u32 idx)
{
    u32 index = (idx >> 24) & 0xff;
    f32 rest = static_cast<f32>(idx & 0xffffff) / 0x1000000;
    const SinCosSample& sample = cSinCosTbl[index];

    /*if (pSin != NULL)*/ *pSin = sample.sin_val + sample.sin_delta * rest;
    /*if (pCos != NULL)*/ *pCos = sample.cos_val + sample.cos_delta * rest;
}

template <typename T>
inline T
MathCalcCommon<T>::exp(T t)
{
    return std::exp(t);
}

template <typename T>
inline T
MathCalcCommon<T>::log(T t)
{
    return std::log(t);
}

template <typename T>
inline T
MathCalcCommon<T>::log2(T t)
{
    return std::log2(t);
}

template <typename T>
inline T
MathCalcCommon<T>::log10(T t)
{
    return std::log10(t);
}

template <typename T>
inline T
MathCalcCommon<T>::minNumber()
{
    return std::numeric_limits<T>::min();
}

template <typename T>
inline T
MathCalcCommon<T>::maxNumber()
{
    return std::numeric_limits<T>::max();
}

template <>
inline float
MathCalcCommon<float>::minNumber()
{
    return -std::numeric_limits<float>::max();
}

template <>
inline float
MathCalcCommon<float>::maxNumber()
{
    return std::numeric_limits<float>::max();
}

template <>
inline double
MathCalcCommon<double>::minNumber()
{
    return -std::numeric_limits<double>::max();
}

template <>
inline double
MathCalcCommon<double>::maxNumber()
{
    return std::numeric_limits<double>::max();
}

template <>
inline long double
MathCalcCommon<long double>::minNumber()
{
    return -std::numeric_limits<long double>::max();
}

template <>
inline long double
MathCalcCommon<long double>::maxNumber()
{
    return std::numeric_limits<long double>::max();
}

template <typename T>
inline T
MathCalcCommon<T>::infinity()
{
    return std::numeric_limits<T>::infinity();
}

template <>
inline f32
MathCalcCommon<f32>::nan()
{
    static const u32 float_nan_binary = 0x7FFFFFFF;

    union { const u32* ui; f32* f; } bit_cast = { .ui = &float_nan_binary };
    return *bit_cast.f;
}

template <>
inline f64
MathCalcCommon<f64>::nan()
{
    static const u64 double_nan_binary = 0x7FFFFFFFFFFFFFFF;

    union { const u64* ui; f64* f; } bit_cast = { .ui = &double_nan_binary };
    return *bit_cast.f;
}

template <typename T>
inline T
MathCalcCommon<T>::epsilon()
{
    return std::numeric_limits<T>::epsilon();
}

template <>
inline s32
MathCalcCommon<s32>::abs(s32 t)
{
    return (t ^ t >> 31) - (t >> 31);
}

template <>
inline u32
MathCalcCommon<u32>::abs(u32 t)
{
    return t;
}

#ifdef cafe

template <>
inline f32
MathCalcCommon<f32>::abs(f32 t)
{
    return std::fabs(t);
}

template <>
inline f64
MathCalcCommon<f64>::abs(f64 t)
{
    return std::fabs(t);
}

#endif // cafe

template <typename T>
inline s32
MathCalcCommon<T>::roundOff(T val)
{
    return std::floor(val + 0.5f);
}

template <>
inline s32
MathCalcCommon<s32>::roundOff(s32 val)
{
    return val;
}

//template <>
//inline s32
//MathCalcCommon<u32>::roundOff(u32 val)
//{
//    return val;
//}

template <typename T>
inline s32
MathCalcCommon<T>::floor(T val)
{
    return std::floor(val);
}

template <>
inline s32
MathCalcCommon<s32>::floor(s32 val)
{
    return val;
}

//template <>
//inline s32
//MathCalcCommon<u32>::floor(u32 val)
//{
//    return val;
//}

template <typename T>
inline s32
MathCalcCommon<T>::ceil(T val)
{
    return std::ceil(val);
}

template <>
inline s32
MathCalcCommon<s32>::ceil(s32 val)
{
    return val;
}

//template <>
//inline s32
//MathCalcCommon<u32>::ceil(u32 val)
//{
//    return val;
//}

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

} // namespace sead
