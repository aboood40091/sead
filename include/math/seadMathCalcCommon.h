#ifndef SEAD_MATHCALCCOMMON_H_
#define SEAD_MATHCALCCOMMON_H_

#include <basis/seadTypes.h>

namespace sead {

template <typename T>
class MathCalcCommon
{
public:
    static s32 roundUpPow2(T x, s32 y);
    static s32 roundDownPow2(T x, s32 y);
    static T gcd(T x, T y);
    static T lcm(T x, T y);
    static T cos(T t);
    static T sin(T t);
    static T deg2rad(T deg);
    static T rsqrt(T x);
};

typedef MathCalcCommon<s32> Mathi;
typedef MathCalcCommon<u32> Mathu;
typedef MathCalcCommon<f32> Mathf;

} // namespace sead

#endif // SEAD_MATHCALCCOMMON_H_
