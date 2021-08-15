#pragma once

namespace sead {

template <typename T>
void Matrix34CalcCommon<T>::makeSRT(Matrix34CalcCommon<T>::Base& o, const Matrix34CalcCommon<T>::Vec3& s, const Matrix34CalcCommon<T>::Vec3& r, const Matrix34CalcCommon<T>::Vec3& t)
{
    T sinRX = MathCalcCommon<T>::sin(r.x);
    T sinRY = MathCalcCommon<T>::sin(r.y);
    T sinRZ = MathCalcCommon<T>::sin(r.z);

    T cosRX = MathCalcCommon<T>::cos(r.x);
    T cosRY = MathCalcCommon<T>::cos(r.y);
    T cosRZ = MathCalcCommon<T>::cos(r.z);

    o.m[0][0] = s.x * (cosRY * cosRZ);
    o.m[1][0] = s.x * (cosRY * sinRZ);
    o.m[2][0] = s.x * -sinRY;

    o.m[0][1] = s.y * (sinRX * sinRY * cosRZ - cosRX * sinRZ);
    o.m[1][1] = s.y * (sinRX * sinRY * sinRZ + cosRX * cosRZ);
    o.m[2][1] = s.y * (sinRX * cosRY);

    o.m[0][2] = s.z * (cosRX * cosRZ * sinRY + sinRX * sinRZ);
    o.m[1][2] = s.z * (cosRX * sinRZ * sinRY - sinRX * cosRZ);
    o.m[2][2] = s.z * (cosRX * cosRY);

    o.m[0][3] = t.x;
    o.m[1][3] = t.y;
    o.m[2][3] = t.z;
}

template <typename T>
void Matrix34CalcCommon<T>::makeST(Matrix34CalcCommon<T>::Base& o, const Matrix34CalcCommon<T>::Vec3& s, const Matrix34CalcCommon<T>::Vec3& t)
{
    o.m[0][0] = s.x;
    o.m[1][0] = 0;
    o.m[2][0] = 0;

    o.m[0][1] = 0;
    o.m[1][1] = s.y;
    o.m[2][1] = 0;

    o.m[0][2] = 0;
    o.m[1][2] = 0;
    o.m[2][2] = s.z;

    o.m[0][3] = t.x;
    o.m[1][3] = t.y;
    o.m[2][3] = t.z;
}

template <typename T>
inline void Matrix34CalcCommon<T>::makeS(Matrix34CalcCommon<T>::Base& o, const Matrix34CalcCommon<T>::Vec3& s)
{
    o.m[0][0] = s.x;
    o.m[1][0] = 0;
    o.m[2][0] = 0;

    o.m[0][1] = 0;
    o.m[1][1] = s.y;
    o.m[2][1] = 0;

    o.m[0][2] = 0;
    o.m[1][2] = 0;
    o.m[2][2] = s.z;

    o.m[0][3] = 0;
    o.m[1][3] = 0;
    o.m[2][3] = 0;
}

#ifdef cafe

template <>
inline void Matrix34CalcCommon<f32>::makeS(Matrix34CalcCommon<f32>::Base& o, const Matrix34CalcCommon<f32>::Vec3& s)
{
    ASM_MTXScale(o.m, s.x, s.y, s.z);
}

#endif // cafe

template <typename T>
void Matrix34CalcCommon<T>::makeQ(Matrix34CalcCommon<T>::Base& o, const Matrix34CalcCommon<T>::Quat& q)
{
    const T x = q.x;
    const T y = q.y;
    const T z = q.z;
    const T w = q.w;

    const T x_2 = 2 * x;
    const T y_2 = 2 * y;
    const T z_2 = 2 * z;
    const T w_2 = 2 * w;

    const T v0 = x_2 * x;
    const T v1 = x_2 * y;
    const T v2 = x_2 * z;

    const T v3 = y_2 * y;
    const T v4 = y_2 * z;

    const T v5 = z_2 * z;

    const T v6 = w_2 * x;
    const T v7 = w_2 * y;
    const T v8 = w_2 * z;

    const T v9  = 1 - v0;
    const T v10 = 1 - v3;

    o.m[0][0] = v10 - v5;
    o.m[0][1] = v1  - v8;
    o.m[0][2] = v2  + v7;
    o.m[0][3] = 0;

    o.m[1][0] = v1 + v8;
    o.m[1][1] = v9 - v5;
    o.m[1][2] = v4 - v6;
    o.m[1][3] = 0;

    o.m[2][0] = v2 - v7;
    o.m[2][1] = v4 + v6;
    o.m[2][2] = v9 - v3;
    o.m[2][3] = 0;
}

template <typename T>
void Matrix34CalcCommon<T>::setBase(Matrix34CalcCommon<T>::Base& n, s32 axis, const Matrix34CalcCommon<T>::Vec3& v)
{
    n.m[0][axis] = v.x;
    n.m[1][axis] = v.y;
    n.m[2][axis] = v.z;
}

template <typename T>
void Matrix34CalcCommon<T>::setTranslation(Matrix34CalcCommon<T>::Base& n, const Matrix34CalcCommon<T>::Vec3& t)
{
    setBase(n, 3, t);
}

template <typename T>
void Matrix44CalcCommon<T>::multiply(Matrix44CalcCommon<T>::Base& o, const Matrix44CalcCommon<T>::Base& a, const Matrix44CalcCommon<T>::Mtx34& b)
{
    T a_00 = a.m[0][0];
    T a_01 = a.m[0][1];
    T a_02 = a.m[0][2];
    T a_03 = a.m[0][3];

    T a_10 = a.m[1][0];
    T a_11 = a.m[1][1];
    T a_12 = a.m[1][2];
    T a_13 = a.m[1][3];

    T a_20 = a.m[2][0];
    T a_21 = a.m[2][1];
    T a_22 = a.m[2][2];
    T a_23 = a.m[2][3];

    T a_30 = a.m[2][0];
    T a_31 = a.m[2][1];
    T a_32 = a.m[2][2];
    T a_33 = a.m[2][3];

    T b_00 = b.m[0][0];
    T b_01 = b.m[0][1];
    T b_02 = b.m[0][2];
    T b_03 = b.m[0][3];

    T b_10 = b.m[1][0];
    T b_11 = b.m[1][1];
    T b_12 = b.m[1][2];
    T b_13 = b.m[1][3];

    T b_20 = b.m[2][0];
    T b_21 = b.m[2][1];
    T b_22 = b.m[2][2];
    T b_23 = b.m[2][3];

    o.m[0][0] = a_00 * b_00 + a_01 * b_10 + a_02 * b_20;
    o.m[0][1] = a_00 * b_01 + a_01 * b_11 + a_02 * b_21;
    o.m[0][2] = a_00 * b_02 + a_01 * b_12 + a_02 * b_22;
    o.m[0][3] = a_00 * b_03 + a_01 * b_13 + a_02 * b_23 + a_03;

    o.m[1][0] = a_10 * b_00 + a_11 * b_10 + a_12 * b_20;
    o.m[1][1] = a_10 * b_01 + a_11 * b_11 + a_12 * b_21;
    o.m[1][2] = a_10 * b_02 + a_11 * b_12 + a_12 * b_22;
    o.m[1][3] = a_10 * b_03 + a_11 * b_13 + a_12 * b_23 + a_13;

    o.m[2][0] = a_20 * b_00 + a_21 * b_10 + a_22 * b_20;
    o.m[2][1] = a_20 * b_01 + a_21 * b_11 + a_22 * b_21;
    o.m[2][2] = a_20 * b_02 + a_21 * b_12 + a_22 * b_22;
    o.m[2][3] = a_20 * b_03 + a_21 * b_13 + a_22 * b_23 + a_23;

    o.m[3][0] = a_30 * b_00 + a_31 * b_10 + a_32 * b_20;
    o.m[3][1] = a_30 * b_01 + a_31 * b_11 + a_32 * b_21;
    o.m[3][2] = a_30 * b_02 + a_31 * b_12 + a_32 * b_22;
    o.m[3][3] = a_30 * b_03 + a_31 * b_13 + a_32 * b_23 + a_33;
}

} // namespace sead
