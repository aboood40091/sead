#ifndef SEAD_VECTOR_H_
#define SEAD_VECTOR_H_

#ifdef cafe
#include <cafe.h>
#endif // cafe

#include <basis/seadTypes.h>
#include <math/seadVectorCalcCommon.h>

namespace sead {

template <typename T>
class Vector2 : public Policies<T>::Vec2Base
{
private:
    typedef Vector2<T> Self;

public:
    Vector2()
    {
    }

    Vector2(T x_, T y_)
    {
        x = x_;
        y = y_;
    }

    Vector2(const Self& v)
    {
        x = v.x;
        y = v.y;
    }

    Self& operator=(const Self& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }

    void set(T x_, T y_)
    {
        x = x_;
        y = y_;
    }

    static const Vector2 zero;
    static const Vector2 ex;
    static const Vector2 ey;
    static const Vector2 ones;
};

template <typename T>
class Vector3 : public Policies<T>::Vec3Base
{
private:
    typedef Vector3<T> Self;

public:
    Vector3()
    {
    }

    Vector3(T x_, T y_, T z_)
    {
        x = x_;
        y = y_;
        z = z_;
    }

    Vector3(const Self& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    Self operator*(T t) const
    {
        Self v = *this;
        v.multScalar(t);
        return v;
    }

    Self operator-(const Self& v) const;
    Self& operator+=(const Self& v);

    Self& operator=(const Self& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }

    T length() const;
    T dot(const Self& t) const;
    void setCross(const Self& a, const Self& b);
    void multScalar(T t);

    T normalize()
    {
        return Vector3CalcCommon<T>::normalize(*this);
    }

    void set(T x_, T y_, T z_)
    {
        Vector3CalcCommon<T>::set(*this, x_, y_, z_);
    }

    static const Vector3 zero;
    static const Vector3 ex;
    static const Vector3 ey;
    static const Vector3 ez;
    static const Vector3 ones;
};

#ifdef cafe

template <>
inline Vector3<f32>
Vector3<f32>::operator-(const Vector3<f32>& v) const
{
    // TODO: Implement using intrinsics
    Vector3<f32> o;
    ASM_VECSubtract((const Vec*)this, (const Vec*)&v, (Vec*)&o);
    return o;
}

template <>
inline Vector3<f32>&
Vector3<f32>::operator+=(const Vector3<f32>& v)
{
    // TODO: Implement using intrinsics
    ASM_VECAdd((const Vec*)this, (const Vec*)&v, (Vec*)this);
    return *this;
}

template <>
inline f32
Vector3<f32>::length() const
{
    return ASM_VECMag((const Vec*)this);
}

template <>
inline f32
Vector3<f32>::dot(const Vector3<f32>& t) const
{
     // TODO: Implement using intrinsics
    return ASM_VECDotProduct((const Vec*)this, (const Vec*)&t);
}

template <>
inline void
Vector3<f32>::setCross(const Vector3<f32>& a, const Vector3<f32>& b)
{
    ASM_VECCrossProduct((const Vec*)&a, (const Vec*)&b, (Vec*)this);
}

template <>
inline void
Vector3<f32>::multScalar(f32 t)
{
    // TODO: Implement using intrinsics
    ASM_VECScale((const Vec*)this, (Vec*)this, t);
}

#endif // cafe

template <typename T>
class Vector4 : public Policies<T>::Vec4Base
{
private:
    typedef Vector3<T> Self;

public:
    Vector4()
    {
    }

    Vector4(T x_, T y_, T z_, T w_)
    {
        x = x_;
        y = y_;
        z = z_;
        w = w_;
    }

    static const Vector4 zero;
    static const Vector4 ex;
    static const Vector4 ey;
    static const Vector4 ez;
    static const Vector4 ew;
    static const Vector4 ones;
};

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;
typedef Vector4<f32> Vector4f;

template <>
extern const Vector2<f32> Vector2<f32>::zero;

template <>
extern const Vector2<f32> Vector2<f32>::ex;

template <>
extern const Vector2<f32> Vector2<f32>::ey;

template <>
extern const Vector2<f32> Vector2<f32>::ones;

template <>
extern const Vector3<f32> Vector3<f32>::zero;

template <>
extern const Vector3<f32> Vector3<f32>::ex;

template <>
extern const Vector3<f32> Vector3<f32>::ey;

template <>
extern const Vector3<f32> Vector3<f32>::ez;

template <>
extern const Vector3<f32> Vector3<f32>::ones;

template <>
extern const Vector4<f32> Vector4<f32>::zero;

template <>
extern const Vector4<f32> Vector4<f32>::ex;

template <>
extern const Vector4<f32> Vector4<f32>::ey;

template <>
extern const Vector4<f32> Vector4<f32>::ez;

template <>
extern const Vector4<f32> Vector4<f32>::ew;

template <>
extern const Vector4<f32> Vector4<f32>::ones;

}  // namespace sead

#endif // #define SEAD_VECTOR_H_
