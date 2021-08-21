#ifndef SEAD_MATHCALCCOMMON_H_
#define SEAD_MATHCALCCOMMON_H_

#include <basis/seadTypes.h>

#define _USE_MATH_DEFINES
#ifdef __cplusplus
#include <cmath>
#else
#include <math.h>
#endif // __cplusplus

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif // M_PI

namespace sead {

template <typename T>
class MathCalcCommon
{
public:
    static T roundUpPow2(T x, s32 y);
    static T roundDownPow2(T x, s32 y);
    static T gcd(T x, T y);
    static T lcm(T x, T y);
    static T cos(T t);
    static T sin(T t);
    static T sqrt(T x);
    static T rsqrt(T x);

    static inline T deg2rad(T a)
    {
        return static_cast<T>(a * (M_PI / 180.0));
    }
};

typedef MathCalcCommon<s32> Mathi;
typedef MathCalcCommon<u32> Mathu;
typedef MathCalcCommon<f32> Mathf;

} // namespace sead

#ifdef __cplusplus

#include <math/seadMathCalcCommon.hpp>

#endif // __cplusplus

#endif // SEAD_MATHCALCCOMMON_H_
