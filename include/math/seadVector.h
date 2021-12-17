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
    Vector2()
    {
    }

    //Vector2(const Self& v)
    //{
    //    operator=(v);
    //}

    Vector2(T x_, T y_);

    Self operator+(const Self& v) const;
    Self operator-(const Self& v) const;

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator=(const Self& v);

    void set(const Self& v);
    void set(T x_, T y_);

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

    //Vector3(const Self& v)
    //{
    //    operator=(v);
    //}

    Vector3(T x_, T y_, T z_);

    Self operator*(T t) const;
    Self operator+(const Self& v) const;
    Self operator-(const Self& v) const;

    Self& operator+=(const Self& v);
    Self& operator-=(const Self& v);
    Self& operator=(const Self& v);

    T dot(const Self& t) const;
    T length() const;

    void add(const Self& a);
    void multScalar(T t);
    T normalize();
    void set(const Self& v);
    void set(T x_, T y_, T z_);
    void setCross(const Self& a, const Self& b);
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

public:
    Vector4()
    {
    }

    //Vector4(const Self& v)
    //{
    //    operator=(v);
    //}

    Vector4(T x_, T y_, T z_, T w_);

    Self& operator=(const Self& v);

    void set(const Self& v);
    void set(T x_, T y_, T z_, T w_);

    static const Vector4 zero;
    static const Vector4 ex;
    static const Vector4 ey;
    static const Vector4 ez;
    static const Vector4 ew;
    static const Vector4 ones;
};

typedef Vector2<s32> Vector2i;

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;
typedef Vector4<f32> Vector4f;

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
