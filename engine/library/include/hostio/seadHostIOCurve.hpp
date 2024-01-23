#pragma once

namespace sead { namespace hostio {

extern f32 (*sCurveFunctionTbl_f32[cNumCurveType])(f32, const CurveDataInfo*, const f32*);
extern f64 (*sCurveFunctionTbl_f64[cNumCurveType])(f32, const CurveDataInfo*, const f64*);
extern Vector2<f32> (*sCurveFunctionTbl_Vec2f[cNumCurveType])(f32, const CurveDataInfo*, const f32*);
extern Vector2<f64> (*sCurveFunctionTbl_Vec2d[cNumCurveType])(f32, const CurveDataInfo*, const f64*);

template <>
inline f32
Curve<f32>::interpolateToF32(f32 t) const
{
    return (*(sCurveFunctionTbl_f32[mInfo.curveType]))(t, &mInfo, mBuf);
}

template <>
inline Vector2f
Curve<f32>::interpolateToVec2f(f32 t) const
{
    return (*(sCurveFunctionTbl_Vec2f[mInfo.curveType]))(t, &mInfo, mBuf);
}

} } // namespace sead::hostio
