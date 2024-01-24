#pragma once

#include <container/seadListImpl.h>
#include <prim/seadSafeString.h>

namespace sead {

class Heap;
class XmlElement;

}

namespace agl { namespace utl {

class IParameterObj;
class ResParameter;

class ParameterBase
{
public:
    enum ParameterType
    {
        cType_bool      = 0,    // agl::utl::Parameter< bool >
        cType_f32       = 1,    // agl::utl::Parameter< f32 >
        cType_int       = 2,    // agl::utl::Parameter< s32 >
        cType_vec2      = 3,    // agl::utl::Parameter< sead::Vector2f >
        cType_vec3      = 4,    // agl::utl::Parameter< sead::Vector3f >
        cType_vec4      = 5,    // agl::utl::Parameter< sead::Vector4f >
        cType_color     = 6,    // agl::utl::Parameter< sead::Color4f >
        cType_string32  = 7,    // agl::utl::Parameter< sead::FixedSafeString<32> >
        cType_string64  = 8,    // agl::utl::Parameter< sead::FixedSafeString<64> > (Unused in NSMBU)
        cType_curve1    = 9,    // agl::utl::ParameterCurve< 1 >
        cType_curve2    = 10,   // agl::utl::ParameterCurve< 2 >                    (Unused in NSMBU)
        cType_curve3    = 11,   // agl::utl::ParameterCurve< 3 >                    (Unused in NSMBU)
        cType_curve4    = 12    // agl::utl::ParameterCurve< 4 >                    (Unused in NSMBU)
    };

public:
    ParameterBase();
    ParameterBase(const sead::SafeString& name, const sead::SafeString& label, IParameterObj* p_obj);
    ParameterBase(const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj);

    bool makeZero();

    void applyResource(ResParameter res);
    void applyResource(ResParameter res, f32 t);

    virtual bool copy(const ParameterBase& src);
    virtual void copyUnsafe(const ParameterBase& src);
    bool copyLerp(const ParameterBase& src_a, const ParameterBase& src_b, f32 t);

private:
    template <typename T>
    void copyLerp_(const ParameterBase& src_a, const ParameterBase& src_b, f32 t);

public:
    virtual void writeToXML(sead::XmlElement*, sead::Heap*) const;
    virtual s32 readFromXML(const sead::XmlElement&);

public:
    sead::SafeString getParameterName() const
    {
        // SEAD_ASSERT_MSG(false, "Can\'t use this string.");
        return sead::SafeString::cEmptyString;
    }

    sead::SafeString getLabel() const
    {
        // SEAD_ASSERT_MSG(false, "Can\'t use this string.");
        return sead::SafeString::cEmptyString;
    }

    sead::SafeString getMeta() const
    {
        // SEAD_ASSERT_MSG(false, "Can\'t use this string.");
        return sead::SafeString::cEmptyString;
    }

    virtual ParameterType getParameterType() const = 0;
    virtual const void* ptr() const = 0;
    virtual void* ptr() = 0;
    virtual const void* typePtr() const = 0;
    virtual void* typePtr() = 0;
    virtual size_t size() const = 0;
    virtual ParameterBase* clone(sead::Heap* heap, IParameterObj* p_obj) const = 0;

protected:
    virtual void postApplyResource_(const void*, size_t) { }

public:
    void initializeListNode(const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj);

    u32 getNameHash() const { return mHash; }
    static u32 calcHash(const sead::SafeString& s);

protected:
    u32 mHash;
    sead::ListNode mListNode;

    friend class IParameterObj;
};
static_assert(sizeof(ParameterBase) == 0x10, "agl::utl::ParameterBase size mismatch");

template <typename T>
class Parameter : public ParameterBase
{
public:
    Parameter()
        : ParameterBase()
    {
    }

    Parameter(const T& value, const sead::SafeString& name, const sead::SafeString& label, IParameterObj* p_obj)
        : ParameterBase()
    {
        initialize(value, name, label, p_obj);
    }

    Parameter(const T& value, const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj)
        : ParameterBase()
    {
        initialize(value, name, label, meta, p_obj);
    }

    virtual ParameterType getParameterType() const;
    virtual const void* ptr() const { return &mValue; }
    virtual void* ptr() { return &mValue; }
    virtual const void* typePtr() const { return &mValue; }
    virtual void* typePtr() { return &mValue; }
    virtual size_t size() const { return sizeof(mValue); }
    virtual ParameterBase* clone(sead::Heap* heap, IParameterObj* p_obj) const;

    void initialize(const T& value, const sead::SafeString& name, const sead::SafeString& label, IParameterObj* p_obj)
    {
        initializeListNode(name, label, "", p_obj);
        setValue(value);
    }

    void initialize(const T& value, const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj)
    {
        initializeListNode(name, label, meta, p_obj);
        setValue(value);
    }

    template <typename U>
    void setValue(const U& value);

    const T& getValue() const { return mValue; }

    T& operator*() { return mValue; }
    const T& operator*() const { return mValue; }

    T* operator->() { return &mValue; }
    const T* operator->() const { return &mValue; }

private:
    T mValue;
};

} }

#ifdef __cplusplus

#include <utility/aglParameter.hpp>

#endif // __cplusplus
