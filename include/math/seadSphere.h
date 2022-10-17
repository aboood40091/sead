#ifndef SEAD_SPHERE_H_
#define SEAD_SPHERE_H_

#include <math/seadVector.h>

namespace sead {

template <typename VectorType>
class Sphere
{
public:
    typedef typename VectorType::ValueType T;

public:
    Sphere()
        : mCenter(VectorType::zero)
        , mRadius(0.0f)
    {
    }

    Sphere(const VectorType& center, T radius)
    {
        mCenter.set(center);
        mRadius = radius;
    }

    const VectorType& getCenter() const { return mCenter; }
    T getRadius() const { return mRadius; }

    void setCenter(const VectorType& center)
    {
        mCenter.set(center);
    }

    void setRadius(T radius)
    {
        mRadius = radius;
    }

private:
    VectorType mCenter;
    T mRadius;
};

typedef Sphere<Vector2f> Sphere2f;
typedef Sphere<Vector3f> Sphere3f;

#ifdef cafe
static_assert(sizeof(Sphere2f) == 0xC, "sead::Sphere<T> size mismatch");
static_assert(sizeof(Sphere3f) == 0x10, "sead::Sphere<T> size mismatch");
#endif // cafe

}  // namespace sead

#endif // #define SEAD_SPHERE_H_
