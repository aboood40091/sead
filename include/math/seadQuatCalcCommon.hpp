#pragma once

#include <math/seadMathCalcCommon.hpp>
#include <math/seadQuat.h>
#include <math/seadQuatCalcCommon.h>

namespace sead {

template <typename T>
bool QuatCalcCommon<T>::makeVectorRotation(QuatCalcCommon<T>::Base& o, const QuatCalcCommon<T>::Vec3& from, const QuatCalcCommon<T>::Vec3& to)
{
    Vector3<T> cross;
    cross.setCross(*static_cast<const Vector3<T>*>(&from),     *static_cast<const Vector3<T>*>(&to));
    const T dot =   static_cast<const Vector3<T>*>(&from)->dot(*static_cast<const Vector3<T>*>(&to)) + 1;

    if (dot <= std::numeric_limits<T>::epsilon())
    {
        o = Quat<T>::unit;
        return false;
    }
    else
    {
        T v0 = 2 * dot;
        T v1 = MathCalcCommon<T>::rsqrt(v0) * v0; // v1 = rsqrt(v0) * v0 => v0 / sqrt(v0) => sqrt(v0) -> v1 = sqrt(v0)
        T v2 = 1 / v1;                            // v2 = 1 / v1 => 1 / sqrt(v0) => rsqrt(v0) -> v2 = rsqrt(v0), which they already calculated when calling MathCalcCommon<T>::rsqrt(v0).
                                                  // Therefore, they are calculating the reverse square root twice. Retarded implementation.
        static_cast<Quat<T>*>(&o)->set(
            cross.x * v2,
            cross.y * v2,
            cross.z * v2,
            v1 * 0.5f
        );
    }

    return true;
}

} // namespace sead
