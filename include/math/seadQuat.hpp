#pragma once

#include <math/seadQuatCalcCommon.h>

namespace sead {

template <typename T>
inline
Quat<T>::Quat(T w_, T x_, T y_, T z_)
{
    QuatCalcCommon<T>::set(*this, w_, x_, y_, z_);
}

template <typename T>
inline void
Quat<T>::makeUnit()
{
    QuatCalcCommon<T>::makeUnit(*this);
}

template <typename T>
inline bool
Quat<T>::makeVectorRotation(const Vec3& from, const Vec3& to)
{
    return QuatCalcCommon<T>::makeVectorRotation(*this, from, to);
}

template <typename T>
inline void
Quat<T>::set(T w_, T x_, T y_, T z_)
{
    QuatCalcCommon<T>::set(*this, w_, x_, y_, z_);
}

}  // namespace sead
