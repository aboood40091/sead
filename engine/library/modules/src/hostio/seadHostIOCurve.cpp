#include <hostio/seadHostIOCurve.h>
#include <math/seadMatrix.h>

namespace sead { namespace hostio {

namespace {

// Reference for all of this seems to be the "Game Programming Gems 4" book.
// Here is one source that also uses it as reference:
// http://old.zaynar.co.uk/cppdoc/latest/projects/maths/NUSpline.h.html
// http://old.zaynar.co.uk/cppdoc/latest/projects/maths/NUSpline.cpp.html

static const Matrix44f hermite(
     2.0f, -2.0f,  1.0f,  1.0f,
    -3.0f,  3.0f, -2.0f, -1.0f,
     0.0f,  0.0f,  1.0f,  0.0f,
     1.0f,  0.0f,  0.0f,  0.0f
);

Vector4f GetPositionOnCubic(const Vector4f& startPos, const Vector4f& startVel, const Vector4f& endPos, const Vector4f& endVel, f32 time)
{
    Matrix44f m1;
    Matrix44f m2;
    m1.setRow(0, Vector4f(startPos.x, startPos.y, startPos.z, 1.0f));
    m1.setRow(1, Vector4f(endPos.x, endPos.y, endPos.z, 1.0f));
    m1.setRow(2, Vector4f(startVel.x, startVel.y, startVel.z, 1.0f));
    m1.setRow(3, Vector4f(endVel.x, endVel.y, endVel.z, 1.0f));
    m2.setMul(hermite, m1);
    Vector4f timeVector(
        time * time * time,
        time * time,
        time,
        1.0f
    );
    return Vector4f(
        timeVector.dot(m2.getCol(0)),
        timeVector.dot(m2.getCol(1)),
        timeVector.dot(m2.getCol(2)),
        timeVector.dot(m2.getCol(3))
    );
}

// Rounded Nonuniform Spline
struct RNS
{
    static const s32 cMaxCtrlPoint = 64;

    struct splineData
    {
        Vector4f position;
        Vector4f velocity;
        f32 distance;
    };
    static_assert(sizeof(splineData) == 0x24);

    splineData node[cMaxCtrlPoint];
    f32 maxDistance;
    s32 nodeCount;

    RNS()
        : nodeCount(0)
    {
    }

    void addNode(const Vector4f& pos);

    Vector4f getStartVelocity(s32 index);
    Vector4f getEndVelocity(s32 index);
    void buildSpline();

    Vector4f getPosition(f32 time);
};
static_assert(sizeof(RNS) == 0x908);

void RNS::addNode(const Vector4f& pos)
{
    if (nodeCount == 0)
        maxDistance = 0.0f;

    else
    {
        node[nodeCount - 1].distance = (node[nodeCount - 1].position - pos).length();
        maxDistance += node[nodeCount - 1].distance;
    }

    node[nodeCount++].position = pos;
}

Vector4f RNS::getStartVelocity(s32 index)
{
    Vector4f temp = 3.0f * (node[index + 1].position - node[index].position) / node[index].distance;
    return (temp - node[index + 1].velocity) * 0.5f;
}

Vector4f RNS::getEndVelocity(s32 index)
{
    Vector4f temp = 3.0f * (node[index].position - node[index - 1].position) / node[index - 1].distance;
    return (temp - node[index - 1].velocity) * 0.5f;
}

void RNS::buildSpline()
{
    for (s32 i = 1; i < nodeCount - 1; i++)
    {
        Vector4f v1 = node[i + 1].position - node[i].position;
        Vector4f v2 = node[i - 1].position - node[i].position;
        v1.normalize();
        v2.normalize();
        node[i].velocity = v1 - v2;
        node[i].velocity.normalize();
    }
    node[0].velocity = getStartVelocity(0);
    node[nodeCount - 1].velocity = getEndVelocity(nodeCount - 1);
}

Vector4f RNS::getPosition(f32 time)
{
    f32 distance = time * maxDistance;
    f32 currentDistance = 0.0f;
    s32 i = 0;
    while (currentDistance + node[i].distance < distance && i < nodeCount - 2)
    {
        currentDistance += node[i].distance;
        i++;
    }
    f32 t = (distance - currentDistance) / node[i].distance;
    Vector4f startVel = node[i].velocity * node[i].distance;
    Vector4f endVel = node[i+1].velocity * node[i].distance;
    return GetPositionOnCubic(
        node[i].position, startVel,
        node[i+1].position, endVel,
        t
    );
}

}

template <typename T>
inline T curveLinear_(f32 x, const CurveDataInfo* info, const T* buf)
{
    if (x < 0.0f)
        return buf[0];

    s32 num = info->numUse - 1;
    f32 ixD = x * num;
    s32 ix = (s32)(x * num);
    if (ix >= num)
        return buf[ix];

    f32 t = ixD - (f32)ix;
    return buf[ix + 0] + (buf[ix + 1] - buf[ix + 0]) * t;
}

template <typename T>
inline T curveHermit_(f32 x, const CurveDataInfo* info, const T* buf)
{
    if (info->numUse % 2 != 0)
        return 0;

    if (x < 0.0f)
        return buf[0];

    s32 numCtrlPoint = info->numUse / 2;
    f32 ixD = x * (numCtrlPoint - 1);
    s32 ix = (s32)(x * (numCtrlPoint - 1));
    f32 s = ixD - (f32)ix;
    if (ix >= (numCtrlPoint - 1))
        return buf[ix * 2];

    ix *= 2;

    // https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Unit_interval_[0,_1]
    T vt0 =  2*s*s*s - 3*s*s     + 1;
    T vt1 = -2*s*s*s + 3*s*s;
    T tt0 =    s*s*s - 2*s*s + s;
    T tt1 =    s*s*s -   s*s;
    return
        buf[ix + 0] * vt0 +
        buf[ix + 2] * vt1 +
        buf[ix + 1] * tt0 +
        buf[ix + 3] * tt1;
}

template <typename T>
inline T curveStep_(f32 x, const CurveDataInfo* info, const T* buf)
{
    s32 ix = (s32)(Mathf::clamp2(0.0f, x, 1.0f) * (info->numUse - 1));
    return buf[ix];
}

template <typename T>
inline T curveSin_(f32 x, const CurveDataInfo*, const T* buf)
{
    return MathCalcCommon<T>::sin(x * buf[0] * Mathf::pi2()) * buf[1];
}

template <typename T>
inline T curveCos_(f32 x, const CurveDataInfo*, const T* buf)
{
    return MathCalcCommon<T>::cos(x * buf[0] * Mathf::pi2()) * buf[1];
}

template <typename T>
inline T curveSinPow2_(f32 x, const CurveDataInfo*, const T* buf)
{
    T val = MathCalcCommon<T>::sin(x * buf[0] * Mathf::pi2());
    return val * val * buf[1];
}

template <typename T>
inline T curveLinear2D_(f32 x, const CurveDataInfo* info, const T* buf)
{
    s32 numData = info->numUse / 2;
    s32 lastBuf = (numData - 1) * 2;

    if (x <= buf[0])
        return buf[1];

    if (x >= buf[lastBuf + 0])
        return buf[lastBuf + 1];

    for (s32 i = 0; i < numData; i++)
    {
        s32 ix = i * 2;
        if (x < buf[ix + 2])
        {
            T rate = (x - buf[ix + 0]) / (buf[ix + 2] - buf[ix + 0]);
            return buf[ix + 1] + (buf[ix + 3] - buf[ix + 1]) * rate;
        }
    }

    return 0;
}

template <typename T>
inline T curveHermit2D_(f32 x, const CurveDataInfo* info, const T* buf)
{
    s32 numData = info->numUse / 3;
    s32 lastBuf = (numData - 1) * 3;

    if (x <= buf[0])
        return buf[1];

    if (x >= buf[lastBuf + 0])
        return buf[lastBuf + 1];

    for (s32 i = 0; i < numData; i++)
    {
        s32 ix = i * 3;
        if (x < buf[ix + 3])
        {
            T s = (x - buf[ix + 0]) / (buf[ix + 3] - buf[ix + 0]);

            // https://en.wikipedia.org/wiki/Cubic_Hermite_spline#Unit_interval_[0,_1]
            T vt0 =  2*s*s*s - 3*s*s     + 1;
            T vt1 = -2*s*s*s + 3*s*s;
            T tt0 =    s*s*s - 2*s*s + s;
            T tt1 =    s*s*s -   s*s;
            return
                buf[ix + 1] * vt0 +
                buf[ix + 4] * vt1 +
                buf[ix + 2] * tt0 +
                buf[ix + 5] * tt1;
        }
    }

    return 0;
}

template <typename T>
inline T curveStep2D_(f32 x, const CurveDataInfo* info, const T* buf)
{
    s32 numData = info->numUse / 2;
    s32 lastBuf = (numData - 1) * 2;

    if (x <= buf[0])
        return buf[1];

    if (x >= buf[lastBuf + 0])
        return buf[lastBuf + 1];

    for (s32 i = 0; i < numData; i++)
    {
        s32 ix = i * 2;
        if (x < buf[ix + 2])
            return buf[ix + 1];
    }

    return 0;
}

template <typename T>
inline T curveNonuniformSpline_(f32, const CurveDataInfo*, const T*)
{
  //SEAD_ASSERT_MSG(false, "You must call ICurve::interpolateToVec2f at this curve type.");
    return 0;
}

template <typename T>
inline Vector2<T> curveLinearVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveLinear_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveHermitVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveHermit_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveStepVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveStep_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveSinVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveSin_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveCosVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveCos_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveSinPow2Vec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveSinPow2_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveLinear2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveLinear2D_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveHermit2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveHermit2D_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveStep2DVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
    return Vector2<T>(t, curveStep2D_(t, info, buf));
}

template <typename T>
inline Vector2<T> curveNonuniformSplineVec2_(f32 t, const CurveDataInfo* info, const T* buf)
{
  //SEAD_ASSERT(info->numUse / 2 < RNS::cMaxCtrlPoint);

    RNS rns;
    Vector4f result;

    for (s32 i = 0; i < info->numUse / 2; i++)
        rns.addNode(Vector4f(buf[i * 2 + 0], buf[i * 2 + 1], 0.0f, 1.0f));

    rns.buildSpline();
    result = rns.getPosition(t);
    return Vector2<T>(result.x, result.y);
}

f32 (*sCurveFunctionTbl_f32[cNumCurveType])(f32, const CurveDataInfo*, const f32*) = {
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

f64 (*sCurveFunctionTbl_f64[cNumCurveType])(f32, const CurveDataInfo*, const f64*) = {
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

Vector2<f32> (*sCurveFunctionTbl_Vec2f[cNumCurveType])(f32, const CurveDataInfo*, const f32*) = {
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

Vector2<f64> (*sCurveFunctionTbl_Vec2d[cNumCurveType])(f32, const CurveDataInfo*, const f64*) = {
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

