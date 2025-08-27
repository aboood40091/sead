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
Vector2CalcCommon<T>::div(Base& o, const Base& a, const Base& b)
{
    o.x = a.x / b.x;
    o.y = a.y / b.y;
}

template <typename T>
inline bool
Vector2CalcCommon<T>::isEqual(const Base& a, const Base& b)
{
    return a.x == b.x &&
           a.y == b.y;
}

template <typename T>
inline T
Vector2CalcCommon<T>::dot(const Base& a, const Base& b)
{
    return a.x * b.x + a.y * b.y;
}

template <typename T>
inline T
Vector2CalcCommon<T>::squaredLength(const Base& v)
{
    return v.x * v.x + v.y * v.y;
}

template <typename T>
inline T
Vector2CalcCommon<T>::length(const Base& v)
{
    return MathCalcCommon<T>::sqrt(squaredLength(v));
}

template <typename T>
inline T
Vector2CalcCommon<T>::squaredDistance(const Base& v, const Base& t)
{
    T dx = v.x - t.x;
    T dy = v.y - t.y;
    return dx * dx + dy * dy;
}

template <typename T>
inline T
Vector2CalcCommon<T>::distance(const Base& v, const Base& t)
{
    return MathCalcCommon<T>::sqrt(squaredDistance(v, t));
}

template <typename T>
inline void
Vector2CalcCommon<T>::lerp(Base& o, const Base& a, const Base& b, f32 ratio)
{
    o.x = MathCalcCommon<T>::lerp(a.x, b.x, ratio);
    o.y = MathCalcCommon<T>::lerp(a.y, b.y, ratio);
}

template <typename T>
inline void
Vector2CalcCommon<T>::multScalar(Base& o, const Base& v, T t)
{
    o.x = v.x * t;
    o.y = v.y * t;
}

template <typename T>
inline void
Vector2CalcCommon<T>::divScalar(Base& o, const Base& v, T t)
{
    T inv_t = MathCalcCommon<T>::inv(t);
    multScalar(o, v, inv_t);
}

template <typename T>
inline void
Vector2CalcCommon<T>::neg(Base& o, const Base& v)
{
    o.x = MathCalcCommon<T>::neg(v.x);
    o.y = MathCalcCommon<T>::neg(v.y);
}

template <typename T>
T Vector2CalcCommon<T>::normalize(Base& v)
{
    const T len = length(v);
    if (len > 0)
    {
        const T inv_len = 1 / len;
        v.x *= inv_len;
        v.y *= inv_len;
    }

    return len;
}

template <typename T>
T Vector2CalcCommon<T>::setNormalize(Base& o, const Base& v)
{
    const T len = length(v);
    if (len > 0)
    {
        const T inv_len = 1 / len;
        o.x = v.x * inv_len;
        o.y = v.y * inv_len;
    }

    return len;
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
inline bool
Vector3CalcCommon<T>::isEqual(const Base& a, const Base& b)
{
    return a.x == b.x &&
           a.y == b.y &&
           a.z == b.z;
}

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

#ifdef cafe

template <>
inline f32
Vector3CalcCommon<f32>::squaredLength(const Base& v)
{
    return ASM_VECSquareMag((const Vec*)&v);
}

#endif // cafe

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
inline T
Vector3CalcCommon<T>::squaredDistance(const Base& v, const Base& t)
{
    T dx = v.x - t.x;
    T dy = v.y - t.y;
    T dz = v.z - t.z;
    return dx * dx + dy * dy + dz * dz;
}

#ifdef cafe

template <>
inline f32
Vector3CalcCommon<f32>::squaredDistance(const Base& v, const Base& t)
{
    return ASM_VECSquareDistance((const Vec*)&v, (const Vec*)&t);
}

#endif // cafe

template <typename T>
inline T
Vector3CalcCommon<T>::distance(const Base& v, const Base& t)
{
    return MathCalcCommon<T>::sqrt(squaredDistance(v, t));
}

#ifdef cafe

template <>
inline f32
Vector3CalcCommon<f32>::distance(const Base& v, const Base& t)
{
    return ASM_VECDistance((const Vec*)&v, (const Vec*)&t);
}

#endif // cafe

template <typename T>
inline void
Vector3CalcCommon<T>::lerp(Base& o, const Base& a, const Base& b, f32 ratio)
{
    o.x = MathCalcCommon<T>::lerp(a.x, b.x, ratio);
    o.y = MathCalcCommon<T>::lerp(a.y, b.y, ratio);
    o.z = MathCalcCommon<T>::lerp(a.z, b.z, ratio);
}

#ifdef cafe

template <>
inline void
Vector3CalcCommon<f32>::mul(Base& o, const Mtx34& m, const Base& v)
{
    ASM_MTXMultVec(const_cast<f32(*)[4]>(m.m), (const Vec*)&v, (Vec*)&o);
}

template <>
inline void
Vector3CalcCommon<f32>::rotate(Base& o, const Mtx34& m, const Base& v)
{
    ASM_MTXMultVecSR(const_cast<f32(*)[4]>(m.m), (const Vec*)&v, (Vec*)&o);
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
inline void
Vector3CalcCommon<T>::divScalar(Base& o, const Base& v, T t)
{
    T inv_t = MathCalcCommon<T>::inv(t);
    multScalar(o, v, inv_t);
}

template <typename T>
inline void
Vector3CalcCommon<T>::neg(Base& o, const Base& v)
{
    o.x = MathCalcCommon<T>::neg(v.x);
    o.y = MathCalcCommon<T>::neg(v.y);
    o.z = MathCalcCommon<T>::neg(v.z);
}

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
T Vector3CalcCommon<T>::setNormalize(Base& o, const Base& v)
{
    const T len = length(v);
    if (len > 0)
    {
        const T inv_len = 1 / len;
        o.x = v.x * inv_len;
        o.y = v.y * inv_len;
        o.z = v.z * inv_len;
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
Vector4CalcCommon<T>::add(Base& o, const Base& a, const Base& b)
{
    o.x = a.x + b.x;
    o.y = a.y + b.y;
    o.z = a.z + b.z;
    o.w = a.w + b.w;
}

template <typename T>
inline void
Vector4CalcCommon<T>::sub(Base& o, const Base& a, const Base& b)
{
    o.x = a.x - b.x;
    o.y = a.y - b.y;
    o.z = a.z - b.z;
    o.w = a.w - b.w;
}

template <typename T>
inline bool
Vector4CalcCommon<T>::isEqual(const Base& a, const Base& b)
{
    return a.x == b.x &&
           a.y == b.y &&
           a.z == b.z &&
           a.w == b.w;
}

template <typename T>
inline T
Vector4CalcCommon<T>::dot(const Base& a, const Base& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

template <typename T>
inline T
Vector4CalcCommon<T>::squaredLength(const Base& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w;
}

template <typename T>
inline T
Vector4CalcCommon<T>::length(const Base& v)
{
    return MathCalcCommon<T>::sqrt(squaredLength(v));
}

template <typename T>
inline T
Vector4CalcCommon<T>::squaredDistance(const Base& v, const Base& t)
{
    T dx = v.x - t.x;
    T dy = v.y - t.y;
    T dz = v.z - t.z;
    T dw = v.w - t.w;
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

template <typename T>
inline T
Vector4CalcCommon<T>::distance(const Base& v, const Base& t)
{
    return MathCalcCommon<T>::sqrt(squaredDistance(v, t));
}

template <typename T>
inline void
Vector4CalcCommon<T>::lerp(Base& o, const Base& a, const Base& b, f32 ratio)
{
    o.x = MathCalcCommon<T>::lerp(a.x, b.x, ratio);
    o.y = MathCalcCommon<T>::lerp(a.y, b.y, ratio);
    o.z = MathCalcCommon<T>::lerp(a.z, b.z, ratio);
    o.w = MathCalcCommon<T>::lerp(a.w, b.w, ratio);
}

template <typename T>
inline void
Vector4CalcCommon<T>::multScalar(Base& o, const Base& v, T t)
{
    o.x = v.x * t;
    o.y = v.y * t;
    o.z = v.z * t;
    o.w = v.w * t;
}

template <typename T>
inline void
Vector4CalcCommon<T>::divScalar(Base& o, const Base& v, T t)
{
    T inv_t = MathCalcCommon<T>::inv(t);
    multScalar(o, v, inv_t);
}

template <typename T>
inline void
Vector4CalcCommon<T>::neg(Base& o, const Base& v)
{
    o.x = MathCalcCommon<T>::neg(v.x);
    o.y = MathCalcCommon<T>::neg(v.y);
    o.z = MathCalcCommon<T>::neg(v.z);
    o.w = MathCalcCommon<T>::neg(v.w);
}

template <typename T>
T Vector4CalcCommon<T>::normalize(Base& v)
{
    const T len = length(v);
    if (len > 0)
    {
        const T inv_len = 1 / len;
        v.x *= inv_len;
        v.y *= inv_len;
        v.z *= inv_len;
        v.w *= inv_len;
    }

    return len;
}

template <typename T>
T Vector4CalcCommon<T>::setNormalize(Base& o, const Base& v)
{
    const T len = length(v);
    if (len > 0)
    {
        const T inv_len = 1 / len;
        o.x = v.x * inv_len;
        o.y = v.y * inv_len;
        o.z = v.z * inv_len;
        o.w = v.w * inv_len;
    }

    return len;
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
