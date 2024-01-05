#pragma once

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace agl { namespace utl {

template <typename T>
inline ParameterBase* Parameter<T>::clone(sead::Heap* heap, IParameterObj* p_obj) const
{
    return new (heap, 4) Parameter<T>(getValue(), getParameterName(), getLabel(), getMeta(), p_obj);
}

template <typename T>
inline void Parameter<T>::setValue(const T& value)
{
    mValue = value;
}

template <>
inline void Parameter<sead::Vector2f>::setValue(const sead::Vector2f& value)
{
    mValue.set(value);
}

template <>
inline void Parameter<sead::Vector3f>::setValue(const sead::Vector3f& value)
{
    mValue.set(value);
}

template <>
inline void Parameter<sead::Vector4f>::setValue(const sead::Vector4f& value)
{
    mValue.set(value);
}

static_assert(sizeof(Parameter<bool>) == 0x14);
static_assert(sizeof(Parameter<s32>) == 0x14);
static_assert(sizeof(Parameter<f32>) == 0x14);
static_assert(sizeof(Parameter<sead::Vector2f>) == 0x18);
static_assert(sizeof(Parameter<sead::Vector3f>) == 0x1C);
static_assert(sizeof(Parameter<sead::Vector4f>) == 0x20);
static_assert(sizeof(Parameter<sead::Color4f>) == 0x20);
static_assert(sizeof(Parameter< sead::FixedSafeString<32> >) == 0x3C);

template <typename T>
void ParameterBase::copyLerp_(const ParameterBase& parameter_a, const ParameterBase& parameter_b, f32 t)
{
    static_cast<T*>(ptr())->setLerp(*static_cast<const T*>(parameter_a.ptr()), *static_cast<const T*>(parameter_b.ptr()), t);
}

template <>
void ParameterBase::copyLerp_<f32>(const ParameterBase& parameter_a, const ParameterBase& parameter_b, f32 t)
{
    *static_cast<f32*>(ptr()) = sead::Mathf::lerp(*static_cast<const f32*>(parameter_a.ptr()), *static_cast<const f32*>(parameter_b.ptr()), t);
}

template <>
inline ParameterBase::ParameterType
Parameter<bool>::getParameterType() const
{
    return cType_bool;
}

template <>
inline ParameterBase::ParameterType
Parameter<s32>::getParameterType() const
{
    return cType_int;
}

template <>
inline ParameterBase::ParameterType
Parameter<f32>::getParameterType() const
{
    return cType_f32;
}

template <>
inline ParameterBase::ParameterType
Parameter<sead::Vector2f>::getParameterType() const
{
    return cType_vec2;
}

template <>
inline ParameterBase::ParameterType
Parameter<sead::Vector3f>::getParameterType() const
{
    return cType_vec3;
}

template <>
inline ParameterBase::ParameterType
Parameter<sead::Vector4f>::getParameterType() const
{
    return cType_vec4;
}

template <>
inline ParameterBase::ParameterType
Parameter<sead::Color4f>::getParameterType() const
{
    return cType_color;
}

template <>
inline ParameterBase::ParameterType
Parameter< sead::FixedSafeString<32> >::getParameterType() const
{
    return cType_string32;
}

} }
