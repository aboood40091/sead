#include <hostio/seadHostIOCurve.h>

namespace sead { namespace hostio {

CurveFunctionType_f32 sCurveFunctionTbl_f32[cNumCurveType] = {
    &curveLinear_<f32>,
    &curveHermit_<f32>,
    &curveStep_<f32>,
    &curveSin_<f32>,
    &curveCos_<f32>,
    &curveSinPow2_<f32>,
    &curveLinear2D_<f32>,
    &curveHermit2D_<f32>,
    &curveStep2D_<f32>,
    &curveNonuniformSpline_<f32>,
};

CurveFunctionType_f64 sCurveFunctionTbl_f64[cNumCurveType] = {
    &curveLinear_<f64>,
    &curveHermit_<f64>,
    &curveStep_<f64>,
    &curveSin_<f64>,
    &curveCos_<f64>,
    &curveSinPow2_<f64>,
    &curveLinear2D_<f64>,
    &curveHermit2D_<f64>,
    &curveStep2D_<f64>,
    &curveNonuniformSpline_<f64>,
};

CurveFunctionType_Vec2f sCurveFunctionTbl_Vec2f[cNumCurveType] = {
    &curveLinearVec2_<f32>,
    &curveHermitVec2_<f32>,
    &curveStepVec2_<f32>,
    &curveSinVec2_<f32>,
    &curveCosVec2_<f32>,
    &curveSinPow2Vec2_<f32>,
    &curveLinear2DVec2_<f32>,
    &curveHermit2DVec2_<f32>,
    &curveStep2DVec2_<f32>,
    &curveNonuniformSplineVec2_<f32>,
};

CurveFunctionType_Vec2d sCurveFunctionTbl_Vec2d[cNumCurveType] = {
    &curveLinearVec2_<f64>,
    &curveHermitVec2_<f64>,
    &curveStepVec2_<f64>,
    &curveSinVec2_<f64>,
    &curveCosVec2_<f64>,
    &curveSinPow2Vec2_<f64>,
    &curveLinear2DVec2_<f64>,
    &curveHermit2DVec2_<f64>,
    &curveStep2DVec2_<f64>,
    &curveNonuniformSplineVec2_<f64>,
};

} } // namespace sead::hostio

