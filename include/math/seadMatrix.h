#ifndef SEAD_MATRIX34_H_
#define SEAD_MATRIX34_H_

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace sead {

template <typename T>
class Matrix34 : public Policies<T>::Mtx34Base
{
private:
    typedef Matrix34<T> Self;
    typedef Quat<T> Quat;
    typedef Vector3<T> Vec3;

public:
    Matrix34()
    {
    }

    Matrix34(
        T _00, T _01, T _02, T _03,
        T _10, T _11, T _12, T _13,
        T _20, T _21, T _22, T _23
    )
    {
        m[0][0] = _00;
        m[0][1] = _01;
        m[0][2] = _02;
        m[0][3] = _03;

        m[1][0] = _10;
        m[1][1] = _11;
        m[1][2] = _12;
        m[1][3] = _13;

        m[2][0] = _20;
        m[2][1] = _21;
        m[2][2] = _22;
        m[2][3] = _23;
    }

    Self& operator=(const Self& n);
    void setMul(const Self& a, const Self& b);

    void makeSRT(const Vec3& s, const Vec3& r, const Vec3& t)
    {
        Matrix34CalcCommon<T>::makeSRT(*this, s, r, t);
    }

    void makeST(const Vec3& s, const Vec3& t)
    {
        Matrix34CalcCommon<T>::makeST(*this, s, t);
    }

    void makeS(T x, T y, T z)
    {
        Vec3 s(x, y, z);
        Matrix34CalcCommon<T>::makeS(*this, s);
    }

    void fromQuat(const Quat& q)
    {
        Matrix34CalcCommon<T>::makeQ(*this, q);
    }

    void setTranslation(const Vec3& t)
    {
        Matrix34CalcCommon<T>::setTranslation(*this, t);
    }

    static const Matrix34 zero;
    static const Matrix34 ident;
};

#ifdef cafe

template <>
inline Matrix34<f32>&
Matrix34<f32>::operator=(const Matrix34<f32>& n)
{
    ASM_MTXCopy(const_cast<f32(*)[4]>(n.m), this->m);
    return *this;
}

template <>
inline void
Matrix34<f32>::setMul(const Matrix34<f32>& a, const Matrix34<f32>& b)
{
    ASM_MTXConcat(const_cast<f32(*)[4]>(a.m), const_cast<f32(*)[4]>(b.m), this->m);
}

#endif // cafe

template <typename T>
class Matrix44 : public Policies<T>::Mtx44Base
{
private:
    typedef Matrix44<T> Self;

public:
    Matrix44()
    {
    }

    Matrix44(
        T _00, T _01, T _02, T _03,
        T _10, T _11, T _12, T _13,
        T _20, T _21, T _22, T _23,
        T _30, T _31, T _32, T _33
    )
    {
        m[0][0] = _00;
        m[0][1] = _01;
        m[0][2] = _02;
        m[0][3] = _03;

        m[1][0] = _10;
        m[1][1] = _11;
        m[1][2] = _12;
        m[1][3] = _13;

        m[2][0] = _20;
        m[2][1] = _21;
        m[2][2] = _22;
        m[2][3] = _23;

        m[3][0] = _30;
        m[3][1] = _31;
        m[3][2] = _32;
        m[3][3] = _33;
    }

    Self& operator=(const Self& n);

    static const Matrix44 zero;
    static const Matrix44 ident;
};

#ifdef cafe

template <>
inline Matrix44<f32>&
Matrix44<f32>::operator=(const Matrix44<f32>& n)
{
    ASM_MTX44Copy(const_cast<f32(*)[4]>(n.m), this->m);
    return *this;
}

#endif // cafe

template <>
extern const Matrix34<f32> Matrix34<f32>::zero;

template <>
extern const Matrix34<f32> Matrix34<f32>::ident;

template <>
extern const Matrix44<f32> Matrix44<f32>::zero;

template <>
extern const Matrix44<f32> Matrix44<f32>::ident;

typedef Matrix34<f32> Matrix34f;
typedef Matrix44<f32> Matrix44f;

}  // namespace sead

#endif // SEAD_MATRIX34_H_
