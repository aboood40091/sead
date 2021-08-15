#ifndef SEAD_MATRIXCALCCOMMON_H_
#define SEAD_MATRIXCALCCOMMON_H_

#include <math/seadMathPolicies.h>

namespace sead {

template <typename T>
class Matrix34CalcCommon
{
public:
    typedef typename Policies<T>::Mtx34Base Base;
    typedef typename Policies<T>::QuatBase Quat;
    typedef typename Policies<T>::Vec3Base Vec3;

public:
    static void makeSRT(Base& o, const Vec3& s, const Vec3& r, const Vec3& t);
    static void makeST(Base& o, const Vec3& s, const Vec3& t);
    static void makeS(Base& o, const Vec3& s);
    static void makeQ(Base& o, const Quat& q);
    static void setBase(Base& n, s32 axis, const Vec3& v);
    static void setTranslation(Base& n, const Vec3& t);
};

template <typename T>
class Matrix44CalcCommon
{
public:
    typedef typename Policies<T>::Mtx44Base Base;
    typedef typename Policies<T>::Mtx34Base Mtx34;

public:
    static void multiply(Base& o, const Base& a, const Mtx34& b);
};

}  // namespace sead

#ifdef __cplusplus

#include <math/seadMatrixCalcCommon.hpp>

#endif // __cplusplus

#endif // SEAD_MATRIXCALCCOMMON_H_
