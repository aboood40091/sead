#ifndef SEAD_MATHCALCCOMMON_H_
#define SEAD_MATHCALCCOMMON_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename T>
class MathCalcCommon
{
public:
    static const u32 cQuarterRoundIdx = 0x40000000; //  90 degrees, PI/2 radians
    static const u32 cHalfRoundIdx    = 0x80000000; // 180 degrees, PI   radians

public:
    struct SinCosSample
    {
        T sin_val;
        T sin_delta;
        T cos_val;
        T cos_delta;
    };

    struct AtanSample
    {
        u32 atan_val;
        T atan_delta;
    };

    struct ExpSample
    {
        T exp_val;
        T exp_delta;
    };

    struct LogSample
    {
        T log_val;
        T log_delta;
    };

public:
    static T piHalf() { return 1.570796326794896619231; }
    static T pi()     { return 3.141592653589793238462; }
    static T pi2()    { return 6.283185307179586476924; }
    static T zero()   { return 0; }
    static T one()    { return 1; }
    static T ln2()    { return 0.69314718055994530942 ; }
    static T ln2Inv() { return 1.442695040888963407   ; }

    static T neg(T t);
    static T inv(T t);

    static T sign(T t)
    {
        if (t >= 0) return  1;
        else        return -1;
    }

    static T fitSign(T x, T y)
    {
        // Unsure
        return abs(x) * sign(y);
    }

    static T square(T t)
    {
        // Good for temp values
        return t * t;
    }

    static T sqrt(T t);
    static T rsqrt(T t);

    static T pow(T x, T y);
    static T powTable(T, T);

    static T sin(T t);
    static T cos(T t);
    static T tan(T t);

    static T asin(T s);
    static T acos(T c);
    static T atan(T t);
    static T atan2(T y, T x);

    static T sinIdx(u32 idx);
    static T cosIdx(u32 idx);
    static T tanIdx(u32 idx);

    static u32 asinIdx(T s);
    static u32 acosIdx(T c);
    static u32 atanIdx(T t);
    static u32 atan2Idx(T y, T x);

    static void sinCosIdx(T* p_sin, T* p_cos, u32 idx);

    static T exp(T t);
    static T log(T t);
    static T log2(T t);
    static T log10(T t);

    static T expTable(T x);
    static T logTable(T x);

    static T minNumber();
    static T maxNumber();
    static T infinity();
    static T nan();
    static T epsilon();

    static bool equalsEpsilon(T a, T b, T eps = epsilon())
    {
        return abs(a - b) <= eps;
    }

    static T abs(T t)
    {
        if (x > 0) return  x;
        else       return -x;
    }

    static T max(T a, T b)
    {
        if (a < b) return b;
        else       return a;
    }

    static T min(T a, T b)
    {
        if (a < b) return a;
        else       return b;
    }

    static T max3(T a, T b, T c)
    {
        return max(max(a, b), c);
    }

    static T min3(T a, T b, T c)
    {
        return min(min(a, b), c);
    }

    static T deg2rad(T a)
    {
        return a * pi() / 180;
    }

    static T rad2deg(T a)
    {
        return a * 180 / pi();
    }

    static u32 deg2idx(T a)
    {
        return a * cHalfRoundIdx / 180;
    }

    static u32 rad2idx(T a)
    {
        return a * cHalfRoundIdx / pi();
    }

    static T idx2deg(u32 a)
    {
        return a * (180.0f / cHalfRoundIdx);
    }

    static T idx2rad(u32 a)
    {
        return a * pi() / cHalfRoundIdx;
    }

    static T roundAngle(T);
    static T angleDist(T, T);

    static T random();
    static T getRand(T);
    static T getRandRange(T, T);
    static T getRandSign();

    static s32 roundOff(T val);
    static s32 floor(T val);
    static s32 ceil(T val);
    static s32 roundUpN(T val, s32 multNumber);
    static s32 roundUpPow2(T val, s32 base);
    static s32 roundDownN(T val, s32 multNumber);
    static s32 roundDownPow2(T val, s32 base);

    static T clampMax(T val, T max_)
    {
        return min(val, max_);
    }

    static T clampMin(T val, T min_)
    {
        return max(val, min_);
    }

    static T clamp2(T min_, T val, T max_)
    {
        if (val < min_) return min_;
        if (max_ < val) return max_;

        return val;
    }

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

template <>
extern s32 MathCalcCommon<s32>::gcd(s32 m, s32 n);

template <>
extern s32 MathCalcCommon<s32>::lcm(s32 m, s32 n);

template <>
extern u32 MathCalcCommon<f32>::atanIdx_(f32 t);

template <>
extern f32 MathCalcCommon<f32>::expTable(f32 x);

template <>
extern f32 MathCalcCommon<f32>::logTable(f32 x);

template <>
extern const MathCalcCommon<f32>::SinCosSample MathCalcCommon<f32>::cSinCosTbl[];

template <>
extern const MathCalcCommon<f32>::AtanSample   MathCalcCommon<f32>::  cAtanTbl[];

template <>
extern const MathCalcCommon<f32>::ExpSample    MathCalcCommon<f32>::   cExpTbl[];

template <>
extern const MathCalcCommon<f32>::LogSample    MathCalcCommon<f32>::   cLogTbl[];

// For convenience
typedef MathCalcCommon<s32> Mathi;
typedef MathCalcCommon<u32> Mathu;
typedef MathCalcCommon<f32> Mathf;

} // namespace sead

#ifdef __cplusplus

#include <math/seadMathCalcCommon.hpp>

#endif // __cplusplus

#endif // SEAD_MATHCALCCOMMON_H_
