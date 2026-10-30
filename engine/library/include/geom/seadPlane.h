#ifndef SEAD_PLANE_H_
#define SEAD_PLANE_H_

#include <math/seadVector.h>

namespace sead {

template <typename VectorType>
class PlaneBase
{
public:
    typedef typename VectorType::ValueType T;

public:
    PlaneBase()
        : mNormal(VectorType::ex)
        , mDistance(0.0f)
    {
    }

    PlaneBase(const VectorType& normal, T distance)
        : mNormal(normal)
        , mDistance(distance)
    {
    }

    const VectorType& getNormal() const { return mNormal; }
    T getDistance() const { return mDistance; }

    void setNormal(const VectorType& normal)
    {
        mNormal = normal;
    }

    void setDistance(T distance)
    {
        mDistance = distance;
    }

    void set(const VectorType& normal, T distance)
    {
        mNormal = normal;
        mDistance = distance;
    }

protected:
    VectorType mNormal;
    T mDistance;
};

template <typename T>
class Plane3 : public PlaneBase<Vector3<T> >
{
public:
    Plane3()
        : PlaneBase<Vector3<T> >()
    {
    }

    Plane3(const Vector3<T>& normal, T distance)
        : PlaneBase<Vector3<T> >(normal, distance)
    {
    }
};

typedef Plane3<f32> Plane3f;

#ifdef cafe
static_assert(sizeof(Plane3f) == 0x10, "sead::Plane3<T> size mismatch");
#endif // cafe

}  // namespace sead

#endif // #define SEAD_PLANE_H_
