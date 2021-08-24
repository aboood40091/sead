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
    Vector2CalcCommon<T>::set(*this, v);
    return *this;
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
inline
Vector3<T>::Vector3(T x_, T y_, T z_)
{
    Vector3CalcCommon<T>::set(*this, x_, y_, z_);
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
    Vector3CalcCommon<T>::set(*this, v);
    return *this;
}

template <typename T>
inline T
Vector3<T>::dot(const Self& t) const
{
    return Vector3CalcCommon<T>::dot(*this, t);
}

template <typename T>
inline T
Vector3<T>::length() const
{
    return Vector3CalcCommon<T>::length(*this);
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
Vector3<T>::setCross(const Self& a, const Self& b)
{
    Vector3CalcCommon<T>::cross(*this, a, b);
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
inline Vector4<T>&
Vector4<T>::operator=(const Self& v)
{
    Vector4CalcCommon<T>::set(*this, v);
    return *this;
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

}  // namespace sead
