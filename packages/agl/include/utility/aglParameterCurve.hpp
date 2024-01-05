#pragma once

namespace agl { namespace utl {

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

} }
