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

    Quat(T w_, T x_, T y_, T z_);

    void makeUnit();
    bool makeVectorRotation(const Vec3& from, const Vec3& to);
    void set(T w_, T x_, T y_, T z_);

    static const Quat unit;
};

typedef Quat<f32> Quatf;

#ifdef cafe
static_assert(sizeof(Quatf) == 0x10, "sead::Quat<T> size mismatch");
#endif // cafe

template <>
extern const Quat<f32> Quat<f32>::unit;

}  // namespace sead

#ifdef __cplusplus

#include <math/seadQuat.hpp>

#endif // __cplusplus

#endif // #define SEAD_QUAT_H_
