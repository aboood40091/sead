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
    static const u32 cQuarterRoundIdx;
    static const u32 cHalfRoundIdx;

public:
    struct SinCosSample;
    struct AtanSample;
    struct ExpSample;
    struct LogSample;

public:
    static T piHalf() { return M_PI / 2; }
    static T pi() { return M_PI; }
    static T pi2() { return M_PI * 2; }
    static T zero() { return 0; }
    static T one() { return 1; }
    static T ln2();
    static T ln2Inv();
    static T neg(T);
    static T inv(T t);

    static inline T sign(T t)
    {
        if (t >= 0) return  1;
        else        return -1;
    }

    static T fitSign(T, T);
    static T square(T);
    static T sqrt(T x);
    static T rsqrt(T x);
    static T pow(T, T);
    static T powTable(T, T);
    static T sin(T t);
    static T cos(T t);
    static T tan(T);
    static T asin(T);
    static T acos(T);
    static T atan(T);
    static T atan2(T, T);
    static T sinIdx(u32 idx);
    static T cosIdx(u32);
    static T tanIdx(u32);
    static u32 asinIdx(T);
    static u32 acosIdx(T c);
    static u32 atanIdx(T);
    static u32 atan2Idx(T y, T x);
    static void sinCosIdx(T*, T*, u32);
    static T exp(T);
    static T log(T);
    static T log2(T);
    static T log10(T);
    static T expTable(T x);
    static T logTable(T x);
    static T minNumber();
    static T maxNumber();
    static T infinity();
    static T nan();
    static T epsilon();
    static bool equalsEpsilon(T a, T b, T eps = epsilon());

    static T abs(T t)
    {
        return std::abs(t);
    }

    static inline T max(T a, T b)
    {
        if (a < b)  return b;
        else        return a;
    }

    static inline T min(T a, T b)
    {
        if (a < b) return a;
        else       return b;
    }

    static T max3(T, T, T);
    static T min3(T, T, T);

    static inline T deg2rad(T a)
    {
        return static_cast<T>(a * (M_PI / 180.0));
    }

    static T rad2deg(T);
    static u32 deg2idx(T);
    static u32 rad2idx(T);
    static T idx2deg(u32);
    static T idx2rad(u32 a);
    static T roundAngle(T);
    static T angleDist(T, T);
    static T random();
    static T getRand(T);
    static T getRandRange(T, T);
    static T getRandSign();
    static s32 roundOff(T);
    static s32 floor(T);
    static s32 ceil(T);
    static s32 roundUpN(T val, s32 multNumber);
    static s32 roundUpPow2(T val, s32 base);
    static s32 roundDownN(T val, s32 multNumber);
    static s32 roundDownPow2(T val, s32 base);
    static T clampMax(T val, T max_);
    static T clampMin(T val, T min_);
    static T clamp2(T min_, T val, T max_);
    static T gcd(T m, T n);
    static T lcm(T m, T n);
    static bool isZero(T, T);
    static bool isNan(T);
    static bool isPow2(T);
    static bool isMultiplePow2(T, T);
    static bool isInfinity(T);
    static bool isIntersect1d(T, T, T, T);
    static bool chase(T*, T, T);
    static bool chaseAngle(T*, T, T);
    static bool chaseAngleIdx(u32*, u32, s64);

    static T lerp(T a, T b, f32 ratio);

protected:
    static u32 atanIdx_(T t);
    static T expLn2_(T x);
    static T log1_2_(T x);
    static void assertGreaterThanOrEqualToZero_(T);

public:
    static const SinCosSample cSinCosTbl[256 + 1];
    static const AtanSample     cAtanTbl[128 + 1];
    static const ExpSample       cExpTbl[ 32 + 1];
    static const LogSample       cLogTbl[256 + 1];
};

// For convenience
typedef MathCalcCommon<s32> Mathi;
typedef MathCalcCommon<u32> Mathu;
typedef MathCalcCommon<f32> Mathf;

} // namespace sead

#ifdef __cplusplus

#include <math/seadMathCalcCommon.hpp>

#endif // __cplusplus

#endif // SEAD_MATHCALCCOMMON_H_
