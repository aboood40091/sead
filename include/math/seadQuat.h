#ifndef SEAD_QUAT_H_
#define SEAD_QUAT_H_

#include <math/seadVector.h>

namespace sead {

template <typename T>
class Quat : public Policies<T>::QuatBase
{
private:
    typedef Quat<T> Self;
    typedef Vector3<T> Vec3;

public:
    Quat()
    {
    }

    Quat(T x_, T y_, T z_, T w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }

    void set(T x_, T y_, T z_, T w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }

    bool makeVectorRotation(const Vec3& from, const Vec3& to)
    {
        return QuatCalcCommon<T>::makeVectorRotation(*this, from, to);
    }

    static const Quat unit;
};

typedef Quat<f32> Quatf;

template <>
extern const Quat<f32> Quat<f32>::unit;

}  // namespace sead

#endif // #define SEAD_QUAT_H_
