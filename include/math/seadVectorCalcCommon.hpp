#pragma once

#include <math/seadVectorCalcCommon.h>

namespace sead {

template <typename T>
T Vector3CalcCommon<T>::normalize(Vector3CalcCommon<T>::Base& v)
{
    T length = static_cast<Vector3<T>*>(&v)->length();
    if (length > 0)
    {
        T inv_length = 1 / length;
        v.x *= inv_length;
        v.y *= inv_length;
        v.z *= inv_length;
    }

    return length;
}

template <typename T>
inline void Vector3CalcCommon<T>::set(Vector3CalcCommon<T>::Base& v, T x, T y, T z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}

} // namespace sead
