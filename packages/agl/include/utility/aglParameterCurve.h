#pragma once

#include <hostio/seadHostIOCurve.h>
#include <utility/aglParameter.h>

namespace agl { namespace utl { 

template <u32 N>
class ParameterCurve : public ParameterBase
{
    static const s32 cNumUseDefault = 9;
    static const s32 cNumBuf = 30;

public:
    ParameterCurve()
        : ParameterBase()
    {
        reset();
    }

    ParameterCurve(const sead::SafeString& name, const sead::SafeString& label, IParameterObj* p_obj)
        : ParameterBase(name, label, p_obj)
    {
        reset();
    }

    ParameterCurve(const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj)
        : ParameterBase(name, label, meta, p_obj)
    {
        reset();
    }

    void reset();

    f32 evaluateUnit(s32 curve_index, f32 t) const;

    virtual bool copy(const ParameterBase& src);
    virtual void copyUnsafe(const ParameterBase& src);

    virtual ParameterType getParameterType() const;
    virtual const void* ptr() const { return mCurveData; }
    virtual void* ptr() { return mCurveData; }
    virtual const void* typePtr() const { return mCurveData; }
    virtual void* typePtr() { return mCurveData; }
    virtual size_t size() const { return sizeof(mCurveData); }
    virtual ParameterBase* clone(sead::Heap* heap, IParameterObj* p_obj) const;

protected:
    virtual void postApplyResource_(const void* p_data, size_t data_size);

private:
    sead::hostio::Curve<f32> mCurve[N];
    sead::hostio::CurveData<f32, cNumBuf> mCurveData[N];
};

} }

#ifdef __cplusplus

#include <utility/aglParameterCurve.hpp>

#endif // __cplusplus
