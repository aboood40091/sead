#ifndef SEAD_VECTORCALCCOMMON_H_
#define SEAD_VECTORCALCCOMMON_H_

#include <math/seadMathPolicies.h>

namespace sead {

template <typename T>
class Vector3CalcCommon
{
public:
    typedef typename Policies<T>::Vec3Base Base;

public:
    static T normalize(Base& v);
    static void set(Base& v, T x, T y, T z);
};

}  // namespace sead

#endif // SEAD_VECTORCALCCOMMON_H_
