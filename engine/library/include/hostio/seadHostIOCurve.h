#ifndef SEAD_HOST_IO_CURVE_H_
#define SEAD_HOST_IO_CURVE_H_

#include <math/seadVector.h>

namespace sead { namespace hostio {

class ICurve
{
public:
    virtual f32 interpolateToF32(f32 t) const = 0;
    virtual Vector2f interpolateToVec2f(f32 t) const = 0;
};

enum CurveType
{
    cCurveLinear = 0,
    cCurveHermit,
    cCurveStep,
    cCurveSin,
    cCurveCos,
    cCurveSinPow2,
    cCurveLinear2D,
    cCurveHermit2D,
    cCurveStep2D,
    cCurveNonuniformSpline,
    cNumCurveType
};
static_assert(cNumCurveType == 10);

struct CurveDataInfo
{
    u8 curveType;
    u8 unitSize;
    u8 numBuf;
    u8 numUse;
};
static_assert(sizeof(CurveDataInfo) == 4);

template <typename T>
class Curve : public ICurve
{
public:
    Curve()
        : mBuf(nullptr)
    {
        mInfo.curveType = 0;
        mInfo.unitSize = sizeof(T);
        mInfo.numBuf = 0;
        mInfo.numUse = 0;
    }

    T* getBuf() const
    {
        return mBuf;
    }

    void setBuf(T* buf)
    {
        mBuf = buf;
    }

    CurveType getCurveType() const
    {
        return CurveType(mInfo.curveType);
    }

    void setCurveType(CurveType curveType)
    {
      //SEAD_ASSERT(mInfo.curveType < cNumCurveType);
        mInfo.curveType = curveType;
    }

    u32 getNumBuf() const
    {
        return mInfo.numBuf;
    }

    void setNumBuf(u32 numBuf)
    {
        mInfo.numBuf = numBuf;
    }

    u32 getNumUse() const
    {
        return mInfo.numUse;
    }

    void setNumUse(u32 numUse)
    {
      //SEAD_ASSERT(numUse <= 0xff);
        mInfo.numUse = numUse;
    }

    virtual f32 interpolateToF32(f32 t) const;
    virtual Vector2f interpolateToVec2f(f32 t) const;

private:
    T* mBuf;
    CurveDataInfo mInfo;
};
static_assert(sizeof(Curve<f32>) == 0xC);

template <typename T, u32 N>
struct CurveData
{
    u32 numUse;
    u32 curveType;
    T buf[N];
};
static_assert(sizeof(CurveData<f32, 30>) == 0x80);

} } // namespace sead::hostio

#ifdef __cplusplus

#include <hostio/seadHostIOCurve.hpp>

#endif // __cplusplus

#endif // SEAD_HOST_IO_CURVE_H_
