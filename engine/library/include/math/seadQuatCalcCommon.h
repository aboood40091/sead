#ifndef SEAD_QUATCALCCOMMON_H_
#define SEAD_QUATCALCCOMMON_H_

#include <math/seadMathPolicies.h>

namespace sead {

template <typename T>
class QuatCalcCommon
{
public:
    typedef typename Policies<T>::QuatBase Base;
    typedef typename Policies<T>::Vec3Base Vec3;

public:
    static void makeUnit(Base& q);
    static bool makeVectorRotation(Base& q, const Vec3& from, const Vec3& to);
    static void set(Base& q, T w, T x, T y, T z);
    static void slerpTo(Base& q, const Base& from, const Base& to, f32 t);
};

}  // namespace sead

#ifdef __cplusplus

#include <math/seadQuatCalcCommon.hpp>

#endif // __cplusplus

#endif // SEAD_QUATCALCCOMMON_H_
