#pragma once

#include <container/seadListImpl.h>
#include <prim/seadSafeString.h>

namespace sead {

class Heap;

}

namespace agl { namespace utl {

class IParameterObj;

class ParameterBase
{
public:
    enum ParameterType
    {
        cType_bool      = 0,    // bool
        cType_f32       = 1,    // f32
        cType_int       = 2,    // s32
        cType_vec2      = 3,    // sead::Vector2f
        cType_vec3      = 4,    // sead::Vector3f
        cType_vec4      = 5,    // sead::Vector4f
        cType_color     = 6,    // sead::Color4f
        cType_string32  = 7,    // sead::FixedSafeString<32>

        // Unused in NSMBU
        cType_string64  = 8,    // sead::FixedSafeString<64>
        cType_curve1    = 9,    // I'll tell you when I find out
        cType_curve2    = 10,   // ...
        cType_curve3    = 11,   // ...
        cType_curve4    = 12    // ...
    };

public:
    ParameterBase();
    virtual ~ParameterBase() { }

    virtual bool copy(const ParameterBase& parameter);
    virtual void copyUnsafe(const ParameterBase& parameter);
    virtual bool copyLerp(const ParameterBase& parameter_a, const ParameterBase& parameter_b, f32 t);

private:
    template <typename T>
    void copyLerp_(const ParameterBase& parameter_a, const ParameterBase& parameter_b, f32 t);

public:
    virtual ParameterType getParameterType() const = 0;
    virtual const void* ptr() const = 0;
    virtual void* ptr() = 0;
    virtual const void* typePtr() const = 0;
    virtual void* typePtr() = 0;
    virtual size_t size() const = 0;
    virtual void clone(sead::Heap*, IParameterObj*) const = 0;

    virtual void postApplyResource_(const void*, size_t) { }

    void initializeListNode(const sead::SafeString& name, const sead::SafeString& label, const sead::SafeString& meta, IParameterObj* p_obj);

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
    Parameter(const T& value, const sead::SafeString& name, const sead::SafeString& label, IParameterObj* p_obj)
        : ParameterBase()
    {
        initializeListNode(name, label, "", p_obj);
        mValue = value;
    }

    ParameterType getParameterType() const;
    const void* ptr() const { return &mValue; }
    void* ptr() { return &mValue; }
    const void* typePtr() const { return &mValue; }
    void* typePtr() { return &mValue; }
    virtual size_t size() const { return sizeof(mValue); }
    void clone(sead::Heap*, IParameterObj*) const { /* Deleted in NSMBU */ }

    void setValue(const T& value) { mValue = value; }
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
