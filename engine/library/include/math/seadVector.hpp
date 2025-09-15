#pragma once

#include <math/seadVectorCalcCommon.h>

namespace sead {

template <typename T>
inline
Vector2<T>::Vector2(T x_, T y_)
{
    Vector2CalcCommon<T>::set(*this, x_, y_);
}

template <typename T>
template <typename VectorType>
inline
Vector2<T>::Vector2(const VectorType& v)
{
    Vector2CalcCommon<T>::set(*this, static_cast<T>(v.x), static_cast<T>(v.y));
}

template <typename T>
inline Vector2<T>
Vector2<T>::operator*(T t) const
{
    Self o;
    Vector2CalcCommon<T>::multScalar(o, *this, t);
    return o;
}

template <typename T>
inline Vector2<T>
operator*(T t, const Vector2<T>& v)
{
    Vector2<T> o;
    Vector2CalcCommon<T>::multScalar(o, v, t);
    return o;
}

template <typename T>
inline Vector2<T>
Vector2<T>::operator/(T t) const
{
    Self o;
    Vector2CalcCommon<T>::divScalar(o, *this, t);
    return o;
}

template <typename T>
inline Vector2<T>
Vector2<T>::operator+(const Self& v) const
{

    Self o;
    Vector2CalcCommon<T>::add(o, *this, v);
    return o;
}

template <typename T>
inline Vector2<T>
Vector2<T>::operator-(const Self& v) const
{

    Self o;
    Vector2CalcCommon<T>::sub(o, *this, v);
    return o;
}

template <typename T>
inline Vector2<T>
Vector2<T>::operator-() const
{
    Self o;
    Vector2CalcCommon<T>::neg(o, *this);
    return o;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator*=(T t)
{
    Vector2CalcCommon<T>::multScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator/=(T t)
{
    Vector2CalcCommon<T>::divScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator+=(const Self& v)
{
    Vector2CalcCommon<T>::add(*this, *this, v);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator-=(const Self& v)
{
    Vector2CalcCommon<T>::sub(*this, *this, v);
    return *this;
}

template <typename T>
inline Vector2<T>&
Vector2<T>::operator=(const Self& v)
{
    x = v.x;
    y = v.y;
    return *this;
}

template <typename T>
template <typename VectorType>
inline Vector2<T>&
Vector2<T>::operator=(const VectorType& v)
{
    x = static_cast<T>(v.x);
    y = static_cast<T>(v.y);
    return *this;
}

template <typename T>
inline bool
Vector2<T>::operator==(const Self& v) const
{
    return Vector2CalcCommon<T>::isEqual(*this, v);
}

template <typename T>
inline bool
Vector2<T>::operator!=(const Self& v) const
{
    return !Vector2CalcCommon<T>::isEqual(*this, v);
}

template <typename T>
inline T
Vector2<T>::dot(const Self& t) const
{
    return Vector2CalcCommon<T>::dot(*this, t);
}

template <typename T>
inline T
Vector2<T>::squaredLength() const
{
    return Vector2CalcCommon<T>::squaredLength(*this);
}

template <typename T>
inline T
Vector2<T>::length() const
{
    return Vector2CalcCommon<T>::length(*this);
}

template <typename T>
inline T
Vector2<T>::squaredDistance(const Self& v) const
{
    return Vector2CalcCommon<T>::squaredDistance(*this, v);
}

template <typename T>
inline T
Vector2<T>::distance(const Self& v) const
{
    return Vector2CalcCommon<T>::distance(*this, v);
}

template <typename T>
inline void
Vector2<T>::add(const Self& a)
{
    Vector2CalcCommon<T>::add(*this, *this, a);
}

template <typename T>
inline void
Vector2<T>::div(const Self& a)
{
    Vector2CalcCommon<T>::div(*this, *this, a);
}

template <typename T>
inline void
Vector2<T>::multScalar(T t)
{
    Vector2CalcCommon<T>::multScalar(*this, *this, t);
}

template <typename T>
inline T
Vector2<T>::normalize()
{
    return Vector2CalcCommon<T>::normalize(*this);
}

template <typename T>
inline T
Vector2<T>::setNormalize(const Self& v)
{
    return Vector2CalcCommon<T>::setNormalize(*this, v);
}

template <typename T>
inline void
Vector2<T>::set(const Self& v)
{
    Vector2CalcCommon<T>::set(*this, v);
}

template <typename T>
inline void
Vector2<T>::set(T x_, T y_)
{
    Vector2CalcCommon<T>::set(*this, x_, y_);
}

template <typename T>
inline void
Vector2<T>::setSub(const Self& a, const Self& b)
{
    Vector2CalcCommon<T>::sub(*this, a, b);
}

template <typename T>
inline void
Vector2<T>::setLerp(const Self& a, const Self& b, f32 ratio)
{
    Vector2CalcCommon<T>::lerp(*this, a, b, ratio);
}

template <typename T>
inline
Vector3<T>::Vector3(T x_, T y_, T z_)
{
    Vector3CalcCommon<T>::set(*this, x_, y_, z_);
}

template <typename T>
inline
Vector3<T>::Vector3(const Vec2& vec2, T z_)
{
    Vector3CalcCommon<T>::set(*this, vec2.x, vec2.y, z_);
}

template <typename T>
inline Vector3<T>
Vector3<T>::operator*(T t) const
{
    Self o;
    Vector3CalcCommon<T>::multScalar(o, *this, t);
    return o;
}

template <typename T>
inline Vector3<T>
operator*(T t, const Vector3<T>& v)
{
    Vector3<T> o;
    Vector3CalcCommon<T>::multScalar(o, v, t);
    return o;
}

template <typename T>
inline Vector3<T>
Vector3<T>::operator/(T t) const
{
    Self o;
    Vector3CalcCommon<T>::divScalar(o, *this, t);
    return o;
}

template <typename T>
inline Vector3<T>
Vector3<T>::operator+(const Self& v) const
{

    Self o;
    Vector3CalcCommon<T>::add(o, *this, v);
    return o;
}

template <typename T>
inline Vector3<T>
Vector3<T>::operator-(const Self& v) const
{

    Self o;
    Vector3CalcCommon<T>::sub(o, *this, v);
    return o;
}

template <typename T>
inline Vector3<T>
Vector3<T>::operator-() const
{
    Self o;
    Vector3CalcCommon<T>::neg(o, *this);
    return o;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator*=(T t)
{
    Vector3CalcCommon<T>::multScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator/=(T t)
{
    Vector3CalcCommon<T>::divScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator+=(const Self& v)
{
    Vector3CalcCommon<T>::add(*this, *this, v);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator-=(const Self& v)
{
    Vector3CalcCommon<T>::sub(*this, *this, v);
    return *this;
}

template <typename T>
inline Vector3<T>&
Vector3<T>::operator=(const Self& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
}

template <typename T>
inline bool
Vector3<T>::operator==(const Self& v) const
{
    return Vector3CalcCommon<T>::isEqual(*this, v);
}

template <typename T>
inline bool
Vector3<T>::operator!=(const Self& v) const
{
    return !Vector3CalcCommon<T>::isEqual(*this, v);
}

template <typename T>
inline T
Vector3<T>::dot(const Self& t) const
{
    return Vector3CalcCommon<T>::dot(*this, t);
}

template <typename T>
inline T
Vector3<T>::squaredLength() const
{
    return Vector3CalcCommon<T>::squaredLength(*this);
}

template <typename T>
inline T
Vector3<T>::length() const
{
    return Vector3CalcCommon<T>::length(*this);
}

template <typename T>
inline T
Vector3<T>::squaredDistance(const Self& v) const
{
    return Vector3CalcCommon<T>::squaredDistance(*this, v);
}

template <typename T>
inline T
Vector3<T>::distance(const Self& v) const
{
    return Vector3CalcCommon<T>::distance(*this, v);
}

template <typename T>
inline void
Vector3<T>::add(const Self& a)
{
    Vector3CalcCommon<T>::add(*this, *this, a);
}

template <typename T>
inline void
Vector3<T>::multScalar(T t)
{
    Vector3CalcCommon<T>::multScalar(*this, *this, t);
}

template <typename T>
inline T
Vector3<T>::normalize()
{
    return Vector3CalcCommon<T>::normalize(*this);
}

template <typename T>
inline T
Vector3<T>::setNormalize(const Self& v)
{
    return Vector3CalcCommon<T>::setNormalize(*this, v);
}

template <typename T>
inline void
Vector3<T>::set(const Self& v)
{
    Vector3CalcCommon<T>::set(*this, v);
}

template <typename T>
inline void
Vector3<T>::set(T x_, T y_, T z_)
{
    Vector3CalcCommon<T>::set(*this, x_, y_, z_);
}

template <typename T>
inline void
Vector3<T>::set(const Vec2& vec2, T z_)
{
    Vector3CalcCommon<T>::set(*this, vec2.x, vec2.y, z_);
}

template <typename T>
inline void
Vector3<T>::setAdd(const Self& a, const Self& b)
{
    Vector3CalcCommon<T>::add(*this, a, b);
}

template <typename T>
inline void
Vector3<T>::setCross(const Self& a, const Self& b)
{
    Vector3CalcCommon<T>::cross(*this, a, b);
}

template <typename T>
inline void
Vector3<T>::setLerp(const Self& a, const Self& b, f32 ratio)
{
    Vector3CalcCommon<T>::lerp(*this, a, b, ratio);
}

template <typename T>
inline void
Vector3<T>::setMul(const Mtx34& m, const Self& v)
{
    Vector3CalcCommon<T>::mul(*this, m, v);
}

template <typename T>
inline void
Vector3<T>::setRotate(const Mtx33& m, const Self& v)
{
    Vector3CalcCommon<T>::rotate(*this, m, v);
}

template <typename T>
inline void
Vector3<T>::setRotate(const Mtx34& m, const Self& v)
{
    Vector3CalcCommon<T>::rotate(*this, m, v);
}

template <typename T>
inline void
Vector3<T>::setScaleAdd(T t, const Self& a, const Self& b)
{
    Vector3CalcCommon<T>::multScalarAdd(*this, t, a, b);
}

template <typename T>
inline
Vector4<T>::Vector4(T x_, T y_, T z_, T w_)
{
    Vector4CalcCommon<T>::set(*this, x_, y_, z_, w_);
}

template <typename T>
inline
Vector4<T>::Vector4(const Vec2& vec2, T z_, T w_)
{
    Vector4CalcCommon<T>::set(*this, vec2.x, vec2.y, z_, w_);
}

template <typename T>
inline
Vector4<T>::Vector4(const Vec3& vec3, T w_)
{
    Vector4CalcCommon<T>::set(*this, vec3.x, vec3.y, vec3.z, w_);
}

template <typename T>
inline Vector4<T>
Vector4<T>::operator*(T t) const
{
    Self o;
    Vector4CalcCommon<T>::multScalar(o, *this, t);
    return o;
}

template <typename T>
inline Vector4<T>
operator*(T t, const Vector4<T>& v)
{
    Vector4<T> o;
    Vector4CalcCommon<T>::multScalar(o, v, t);
    return o;
}

template <typename T>
inline Vector4<T>
Vector4<T>::operator/(T t) const
{
    Self o;
    Vector4CalcCommon<T>::divScalar(o, *this, t);
    return o;
}

template <typename T>
inline Vector4<T>
Vector4<T>::operator+(const Self& v) const
{

    Self o;
    Vector4CalcCommon<T>::add(o, *this, v);
    return o;
}

template <typename T>
inline Vector4<T>
Vector4<T>::operator-(const Self& v) const
{

    Self o;
    Vector4CalcCommon<T>::sub(o, *this, v);
    return o;
}

template <typename T>
inline Vector4<T>
Vector4<T>::operator-() const
{
    Self o;
    Vector4CalcCommon<T>::neg(o, *this);
    return o;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator*=(T t)
{
    Vector4CalcCommon<T>::multScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator/=(T t)
{
    Vector4CalcCommon<T>::divScalar(*this, *this, t);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator+=(const Self& v)
{
    Vector4CalcCommon<T>::add(*this, *this, v);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator-=(const Self& v)
{
    Vector4CalcCommon<T>::sub(*this, *this, v);
    return *this;
}

template <typename T>
inline Vector4<T>&
Vector4<T>::operator=(const Self& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
    return *this;
}

template <typename T>
inline bool
Vector4<T>::operator==(const Self& v) const
{
    return Vector4CalcCommon<T>::isEqual(*this, v);
}

template <typename T>
inline bool
Vector4<T>::operator!=(const Self& v) const
{
    return !Vector4CalcCommon<T>::isEqual(*this, v);
}

template <typename T>
inline T
Vector4<T>::dot(const Self& t) const
{
    return Vector4CalcCommon<T>::dot(*this, t);
}

template <typename T>
inline T
Vector4<T>::squaredLength() const
{
    return Vector4CalcCommon<T>::squaredLength(*this);
}

template <typename T>
inline T
Vector4<T>::length() const
{
    return Vector4CalcCommon<T>::length(*this);
}

template <typename T>
inline T
Vector4<T>::squaredDistance(const Self& v) const
{
    return Vector4CalcCommon<T>::squaredDistance(*this, v);
}

template <typename T>
inline T
Vector4<T>::distance(const Self& v) const
{
    return Vector4CalcCommon<T>::distance(*this, v);
}

template <typename T>
inline void
Vector4<T>::multScalar(T t)
{
    Vector4CalcCommon<T>::multScalar(*this, *this, t);
}

template <typename T>
inline T
Vector4<T>::normalize()
{
    return Vector4CalcCommon<T>::normalize(*this);
}

template <typename T>
inline T
Vector4<T>::setNormalize(const Self& v)
{
    return Vector4CalcCommon<T>::setNormalize(*this, v);
}

template <typename T>
inline void
Vector4<T>::set(const Self& v)
{
    Vector4CalcCommon<T>::set(*this, v);
}

template <typename T>
inline void
Vector4<T>::set(T x_, T y_, T z_, T w_)
{
    Vector4CalcCommon<T>::set(*this, x_, y_, z_, w_);
}

template <typename T>
inline void
Vector4<T>::set(const Vec2& vec2, T z_, T w_)
{
    Vector4CalcCommon<T>::set(*this, vec2.x, vec2.y, z_, w_);
}

template <typename T>
inline void
Vector4<T>::set(const Vec3& vec3, T w_)
{
    Vector4CalcCommon<T>::set(*this, vec3.x, vec3.y, vec3.z, w_);
}

template <typename T>
inline void
Vector4<T>::setLerp(const Self& a, const Self& b, f32 ratio)
{
    Vector4CalcCommon<T>::lerp(*this, a, b, ratio);
}

}  // namespace sead
