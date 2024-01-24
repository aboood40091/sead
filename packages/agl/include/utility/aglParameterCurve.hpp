#pragma once

#include <prim/seadMemUtil.h>

namespace agl { namespace utl {

template <u32 N>
inline ParameterBase*
ParameterCurve<N>::clone(sead::Heap* heap, IParameterObj* p_obj) const
{
    ParameterCurve<N>* p = new (heap, 4) ParameterCurve<N>(getParameterName(), getLabel(), p_obj);
    sead::MemUtil::copy(p->mCurveData, mCurveData, sizeof(mCurveData));
    return p;
}

typedef ParameterCurve<1> ParameterCurve1f;
typedef ParameterCurve<2> ParameterCurve2f;
typedef ParameterCurve<3> ParameterCurve3f;
typedef ParameterCurve<4> ParameterCurve4f;

static_assert(sizeof(ParameterCurve1f) == 0x9C);

template <>
inline ParameterBase::ParameterType
ParameterCurve<1>::getParameterType() const
{
    return cType_curve1;
}

template <>
inline ParameterBase::ParameterType
ParameterCurve<2>::getParameterType() const
{
    return cType_curve2;
}

template <>
inline ParameterBase::ParameterType
ParameterCurve<3>::getParameterType() const
{
    return cType_curve3;
}

template <>
inline ParameterBase::ParameterType
ParameterCurve<4>::getParameterType() const
{
    return cType_curve4;
}

template <u32 N>
void
ParameterCurve<N>::reset()
{
    static const f32 s_initialize[cNumUseDefault] = {
        0.0f, 0.0f, 0.5f,
        0.5f, 0.5f, 0.5f,
        1.0f, 1.0f, 0.5f
    };

    for (u32 i = 0; i < N; i++)
    {
        sead::MemUtil::copy(mCurveData[i].buf, s_initialize, sizeof(s_initialize));
        for (s32 j = cNumUseDefault; j < cNumBuf; j++)
            mCurveData[i].buf[j] = 1.0f;

        mCurveData[i].curveType = sead::hostio::cCurveHermit2D;
        mCurveData[i].numUse = cNumUseDefault;

        mCurve[i].setNumBuf(cNumBuf);
        mCurve[i].setCurveType(sead::hostio::cCurveHermit2D);
        mCurve[i].setNumUse(cNumUseDefault);
        mCurve[i].setBuf(mCurveData[i].buf);
    }
}

template <u32 N>
f32
ParameterCurve<N>::evaluateUnit(s32 curve_index, f32 t) const
{
    return mCurve[curve_index].interpolateToF32(t);
}

template <u32 N>
bool
ParameterCurve<N>::copy(const ParameterBase& src)
{
    if (getParameterType() != src.getParameterType() ||
        getParameterName() != src.getParameterName())
    {
        return false;
    }

    copyUnsafe(src);
    return true;
}

template <u32 N>
void
ParameterCurve<N>::copyUnsafe(const ParameterBase& src)
{
    if (getParameterType() != src.getParameterType())
    {
      //SEAD_ASSERT_MSG(false, "getParameterType() == src.getParameterType()");
        return;
    }

    sead::MemUtil::copy(ptr(), src.ptr(), size());
    mCurve->setCurveType(static_cast<const ParameterCurve<N>&>(src).mCurve->getCurveType());
    mCurve->setNumUse(static_cast<const ParameterCurve<N>&>(src).mCurve->getNumUse());
}

template <u32 N>
void
ParameterCurve<N>::postApplyResource_(const void* p_data, size_t data_size)
{
    if (data_size != size())
    {
        for (u32 i = 0; i < N; i++)
        {
            mCurve[i].setNumBuf(cNumBuf);
            mCurve[i].setBuf(mCurveData[i].buf);
        }
    }
    else
    {
        for (u32 i = 0; i < N; i++)
        {
            mCurve[i].setCurveType(sead::hostio::CurveType(mCurveData[i].curveType));
            mCurve[i].setNumBuf(cNumBuf);
            mCurve[i].setBuf(mCurveData[i].buf);
            mCurve[i].setNumUse(mCurveData[i].numUse);
        }
    }
}

} }
