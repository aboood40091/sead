#pragma once

#include <hostio/seadHostIOCurve.h>
#include <utility/aglParameter.h>

namespace agl { namespace utl { 

template <u32 N>
class ParameterCurve : public ParameterBase
{
public:
    ParameterCurve()
        : ParameterBase()
    {
        reset();
    }

    void reset();

    virtual bool copy(const ParameterBase& parameter);
    virtual void copyUnsafe(const ParameterBase& parameter);

    virtual ParameterType getParameterType() const;
    virtual const void* ptr() const { return mCurveData; }
    virtual void* ptr() { return mCurveData; }
    virtual const void* typePtr() const { return mCurveData; }
    virtual void* typePtr() { return mCurveData; }
    virtual size_t size() const { return sizeof(mCurveData); }
    virtual ParameterBase* clone(sead::Heap* heap, IParameterObj* p_obj) const;

protected:
    virtual void postApplyResource_(const void*, size_t) { }

private:
    sead::hostio::Curve<f32> mCurve[N];
    sead::hostio::CurveData<f32, 30> mCurveData[N];
};

} }

#ifdef __cplusplus

#include <utility/aglParameterCurve.hpp>

#endif // __cplusplus
