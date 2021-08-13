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
    static bool makeVectorRotation(Base& o, const Vec3& from, const Vec3& to);
};

}  // namespace sead

#endif // SEAD_QUATCALCCOMMON_H_
