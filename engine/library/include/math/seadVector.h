#ifndef SEAD_VECTOR_H_
#define SEAD_VECTOR_H_

#include <basis/seadTypes.h>
#include <math/seadMathPolicies.h>

namespace sead {

template <typename T>
class Vector2 : public Policies<T>::Vec2Base
{
private:
    typedef Vector2<T> Self;

public:
    typedef T ValueType;

public:
    Vector2()
    {
    }

    //Vector2(const Self& v)
    //{
    //    operator=(v);
    //}

    Vector2(T x_, T y_);

    template <typename VectorType>
    Vector2(const VectorType& v);

    Self operator*(T t) const;
    Self operator+(const Self& v) const;
    Self operator-(const Self& v) const;

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator=(const Self& v);

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const;

    T length() const;

    void add(const Self& a);
    void div(const Self& a);
    void multScalar(T t);
    T normalize();
    T setNormalize(const Self& v);
    void set(const Self& v);
    void set(T x_, T y_);

    void setLerp(const Self& a, const Self& b, f32 ratio);

    static const Vector2 zero;
    static const Vector2 ex;
    static const Vector2 ey;
    static const Vector2 ones;
};

template <typename T>
class Matrix33;

template <typename T>
class Matrix34;

template <typename T>
class Matrix44;

template <typename T>
class Vector3 : public Policies<T>::Vec3Base
{
private:
    typedef Vector3<T> Self;
    typedef Vector2<T> Vec2;
    typedef Matrix33<T> Mtx33;
    typedef Matrix34<T> Mtx34;
    typedef Matrix44<T> Mtx44;

public:
    typedef T ValueType;

public:
    Vector3()
    {
    }

    //Vector3(const Self& v)
    //{
    //    operator=(v);
    //}

    Vector3(T x_, T y_, T z_);
    Vector3(const Vec2& vec2, T z_);

    Self operator*(T t) const;
    Self operator+(const Self& v) const;
    Self operator-(const Self& v) const;

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator=(const Self& v);

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const;

    T dot(const Self& t) const;
    T length() const;

    void add(const Self& a);
    void multScalar(T t);
    T normalize();
    T setNormalize(const Self& v);
    void set(const Self& v);
    void set(T x_, T y_, T z_);
    void setAdd(const Self& a, const Self& b);
    void setCross(const Self& a, const Self& b);
    void setLerp(const Self& a, const Self& b, f32 ratio);
    void setMul(const Mtx34& m, const Self& v);
    void setRotate(const Mtx33& m, const Self& v);
    void setRotate(const Mtx34& m, const Self& v);
    void setMulAndDivByW(const Mtx44& m, const Self& v);
    void setScaleAdd(T t, const Self& a, const Self& b);

    static const Vector3 zero;
    static const Vector3 ex;
    static const Vector3 ey;
    static const Vector3 ez;
    static const Vector3 ones;
};

template <typename T>
class Vector4 : public Policies<T>::Vec4Base
{
private:
    typedef Vector4<T> Self;
    typedef Vector2<T> Vec2;
    typedef Vector3<T> Vec3;

public:
    typedef T ValueType;

public:
    Vector4()
    {
    }

    //Vector4(const Self& v)
    //{
    //    operator=(v);
    //}

    Vector4(T x_, T y_, T z_, T w_);
    Vector4(const Vec2& vec2, T z_, T w_);
    Vector4(const Vec3& vec3, T w_);

    Self operator*(T t) const;

    Self& operator+=(const Self& v);
    Self& operator=(const Self& v);

    bool operator==(const Self& v) const;
    bool operator!=(const Self& v) const;

    T length() const;

    void multScalar(T t);
    T normalize();
    T setNormalize(const Self& v);
    void set(const Self& v);
    void set(T x_, T y_, T z_, T w_);

    void setLerp(const Self& a, const Self& b, f32 ratio);

    static const Vector4 zero;
    static const Vector4 ex;
    static const Vector4 ey;
    static const Vector4 ez;
    static const Vector4 ew;
    static const Vector4 ones;
};

typedef Vector2<s32> Vector2i;
typedef Vector3<s32> Vector3i;
typedef Vector4<s32> Vector4i;

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;
typedef Vector4<f32> Vector4f;

typedef Vector2<u32> Vector2u;
typedef Vector3<u32> Vector3u;
typedef Vector4<u32> Vector4u;

#ifdef cafe
static_assert(sizeof(Vector2f) == 8, "sead::Vector2<T> size mismatch");
static_assert(sizeof(Vector3f) == 0xC, "sead::Vector3<T> size mismatch");
static_assert(sizeof(Vector4f) == 0x10, "sead::Vector4<T> size mismatch");
#endif // cafe

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

#ifdef __cplusplus

#include <math/seadVector.hpp>

#endif // __cplusplus

#endif // #define SEAD_VECTOR_H_
