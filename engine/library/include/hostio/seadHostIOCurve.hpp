#pragma once

namespace sead { namespace hostio {

template <typename T>
T curveLinear_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveHermit_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveStep_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveSin_(f32 t_, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveCos_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveSinPow2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveLinear2D_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveHermit2D_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveStep2D_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
T curveNonuniformSpline_(f32 t, const CurveDataInfo* info, const T* buf);

template <typename T>
Vector2<T> curveLinearVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveHermitVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveStepVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveSinVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveCosVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveSinPow2Vec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveLinear2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveHermit2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveStep2DVec2_(f32 t, const CurveDataInfo* info, const T* buf);
template <typename T>
Vector2<T> curveNonuniformSplineVec2_(f32 t, const CurveDataInfo* info, const T* buf);

static const s32 cNumCurveType = 10;

typedef f32 (*CurveFunctionType_f32)(f32, const CurveDataInfo*, const f32*);
typedef f64 (*CurveFunctionType_f64)(f32, const CurveDataInfo*, const f64*);
typedef Vector2<f32> (*CurveFunctionType_Vec2f)(f32, const CurveDataInfo*, const f32*);
typedef Vector2<f64> (*CurveFunctionType_Vec2d)(f32, const CurveDataInfo*, const f64*);

extern CurveFunctionType_f32 sCurveFunctionTbl_f32[cNumCurveType];
extern CurveFunctionType_f64 sCurveFunctionTbl_f64[cNumCurveType];
extern CurveFunctionType_Vec2f sCurveFunctionTbl_Vec2f[cNumCurveType];
extern CurveFunctionType_Vec2d sCurveFunctionTbl_Vec2d[cNumCurveType];

} } // namespace sead::hostio
