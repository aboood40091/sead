#pragma once

#ifdef cafe

#include <cafe.h>
#include <ppc_ps.h>

inline f32x2& tof32x2(f32& x)
{
    return *reinterpret_cast<f32x2*>(&x);
}

inline const f32x2& tof32x2(const f32& x)
{
    return *reinterpret_cast<const f32x2*>(&x);
}

#endif // cafe

#include <math/seadMathCalcCommon.h>

namespace sead {

template <typename T>
inline void
Vector2CalcCommon<T>::add(Base& o, const Base& a, const Base& b)
{
    o.x = a.x + b.x;
    o.y = a.y + b.y;
}

template <typename T>
inline void
Vector2CalcCommon<T>::sub(Base& o, const Base& a, const Base& b)
{
    o.x = a.x - b.x;
    o.y = a.y - b.y;
}

template <typename T>
inline void
Vector2CalcCommon<T>::set(Base& o, const Base& v)
{
    o.x = v.x;
    o.y = v.y;
}

template <typename T>
inline void
Vector2CalcCommon<T>::set(Base& v, T x, T y)
{
    v.x = x;
    v.y = y;
}

template <typename T>
inline void
Vector3CalcCommon<T>::add(Base& o, const Base& a, const Base& b)
{
    o.x = a.x + b.x;
    o.y = a.y + b.y;
    o.z = a.z + b.z;
}

#ifdef cafe

template <>
inline void
Vector3CalcCommon<f32>::add(Base& o, const Base& a, const Base& b)
{
    tof32x2(o.x) = __PS_ADD(tof32x2(a.x), tof32x2(b.x));
    o.z = a.z + b.z;
}

#endif // cafe

template <typename T>
inline void
Vector3CalcCommon<T>::sub(Base& o, const Base& a, const Base& b)
{
    o.x = a.x - b.x;
    o.y = a.y - b.y;
    o.z = a.z - b.z;
}

#ifdef cafe

template <>
inline void
Vector3CalcCommon<f32>::sub(Base& o, const Base& a, const Base& b)
{
    tof32x2(o.x) = __PS_SUB(tof32x2(a.x), tof32x2(b.x));
    o.z = a.z - b.z;
}

#endif // cafe

template <typename T>
inline void
Vector3CalcCommon<T>::cross(Base& o, const Base& a, const Base& b)
{
    Base v;

    v.x = a.y * b.z - a.z * b.y;
    v.y = a.z * b.x - a.x * b.z;
    v.z = a.x * b.y - a.y * b.x;

    o.x = v.x;
    o.y = v.y;
    o.z = v.z;
}

#ifdef cafe

template <>
inline void
Vector3CalcCommon<f32>::cross(Base& o, const Base& a, const Base& b)
{
    ASM_VECCrossProduct((const Vec*)&a, (const Vec*)&b, (Vec*)&o);
}

#endif // cafe

template <typename T>
inline T
Vector3CalcCommon<T>::dot(const Base& a, const Base& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

#ifdef cafe

template <>
inline f32
Vector3CalcCommon<f32>::dot(const Base& a, const Base& b)
{
    f32x2 f0;
    f0 = __PS_MUL(tof32x2(a.x), tof32x2(b.x));
    f0 = __PS_SUM0(f0, f0, f0);

    return f0[0] + a.z * b.z;
}

#endif // cafe

template <typename T>
inline T
Vector3CalcCommon<T>::squaredLength(const Base& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

template <typename T>
inline T
Vector3CalcCommon<T>::length(const Base& v)
{
    return MathCalcCommon<T>::sqrt(squaredLength(v));
}

#ifdef cafe

template <>
inline f32
Vector3CalcCommon<f32>::length(const Base& v)
{
    return ASM_VECMag((const Vec*)&v);
}

#endif // cafe

template <typename T>
inline void
Vector3CalcCommon<T>::multScalar(Base& o, const Base& v, T t)
{
    o.x = v.x * t;
    o.y = v.y * t;
    o.z = v.z * t;
}

#ifdef cafe

template <>
inline void
Vector3CalcCommon<f32>::multScalar(Base& o, const Base& v, f32 t)
{
    tof32x2(o.x) = __PS_MULS0F(tof32x2(v.x), t);
    o.z = v.z * t;
}

#endif // cafe

template <typename T>
inline void
Vector3CalcCommon<T>::multScalarAdd(Base& o, T t, const Base& a, const Base& b)
{
    o.x = a.x * t + b.x;
    o.y = a.y * t + b.y;
    o.z = a.z * t + b.z;
}

#ifdef cafe

template <>
inline void
Vector3CalcCommon<f32>::multScalarAdd(Base& o, f32 t, const Base& a, const Base& b)
{
    tof32x2(o.x) = __PS_MADDS0F(tof32x2(a.x), t, tof32x2(b.x));
    o.z = a.z * t + b.z;
}

#endif // cafe

template <typename T>
T Vector3CalcCommon<T>::normalize(Base& v)
{
    const T len = length(v);
    if (len > 0)
    {
        const T inv_len = 1 / len;
        v.x *= inv_len;
        v.y *= inv_len;
        v.z *= inv_len;
    }

    return len;
}

template <typename T>
inline void
Vector3CalcCommon<T>::set(Base& o, const Base& v)
{
    o.x = v.x;
    o.y = v.y;
    o.z = v.z;
}

template <typename T>
inline void
Vector3CalcCommon<T>::set(Base& v, T x, T y, T z)
{
    v.x = x;
    v.y = y;
    v.z = z;
}

template <typename T>
inline void
Vector4CalcCommon<T>::set(Base& o, const Base& v)
{
    o.x = v.x;
    o.y = v.y;
    o.z = v.z;
    o.w = v.w;
}

template <typename T>
inline void
Vector4CalcCommon<T>::set(Base& v, T x, T y, T z, T w)
{
    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;
}

} // namespace sead
