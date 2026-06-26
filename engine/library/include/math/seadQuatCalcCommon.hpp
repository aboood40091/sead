#pragma once

#ifdef cafe
#include <cafe.h>
#endif // cafe

#include <math/seadMathCalcCommon.h>
#include <math/seadQuat.h>

namespace sead {

template <typename T>
inline void
QuatCalcCommon<T>::makeUnit(Base& q)
{
    q = Quat<T>::unit;
}

template <typename T>
bool QuatCalcCommon<T>::makeVectorRotation(Base& q, const Vec3& from, const Vec3& to)
{
    Vec3 cross;
    Vector3CalcCommon<T>::cross(cross, from, to);
    const T dot = Vector3CalcCommon<T>::dot(from, to) + 1;

    if (dot <= MathCalcCommon<T>::epsilon())
    {
        makeUnit(q);
        return false;
    }
    else
    {
        T v1 = MathCalcCommon<T>::sqrt(2 * dot);
        T v2 = 1 / v1;
        set(q, v1 * 0.5f,
               cross.x * v2, cross.y * v2, cross.z * v2);
    }

    return true;
}

template <typename T>
inline void
QuatCalcCommon<T>::set(Base& q, T w, T x, T y, T z)
{
    q.w = w;
    q.x = x;
    q.y = y;
    q.z = z;
}

#ifdef cafe

template <>
inline void
QuatCalcCommon<f32>::slerpTo(Base& q, const Base& from, const Base& to, f32 t)
{
    C_QUATSlerp(reinterpret_cast<const Quaternion*>(&from.x), reinterpret_cast<const Quaternion*>(&to.x), reinterpret_cast<Quaternion*>(&q.x), t);
}

#endif // cafe

} // namespace sead
