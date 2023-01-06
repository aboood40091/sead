#pragma once

#include <heap/seadDisposer.h>
#include <prim/seadDelegate.h>
#include <prim/seadNamable.h>

namespace agl { namespace lyr {

class RenderInfo;

class DrawMethod : public sead::IDisposer, public sead::INamable
{
private:
    enum DelegateType
    {
        cDelegateType_Dummy         = 0,
        cDelegateType_SeadDelegate1 = 1,
        cDelegateType_SeadDelegate  = 2
    };

    class Dummy
    {
    };

    template <typename T>
    struct MethodPtr1 { typedef void (T::*Type)(const RenderInfo&); };

    template <typename T>
    struct MethodPtr { typedef void (T::*Type)(); };

public:
    DrawMethod()
        : sead::IDisposer()
        , sead::INamable("")
        , mDelegateType(cDelegateType_Dummy)
        , mPriority(0)
    {
        new (mDelegateData) sead::Delegate<Dummy>();
    }

    virtual ~DrawMethod();

    template <typename T>
    void bind(T* obj, MethodPtr1<T>::Type method, const sead::SafeString& name)
    {
        new (mDelegateData) sead::Delegate1<T, const RenderInfo&>(obj, method);
        mDelegateType = cDelegateType_SeadDelegate1;
        setName(name);
    }

    template <typename T>
    void bind(T* obj, MethodPtr<T>::Type method, const sead::SafeString& name)
    {
        new (mDelegateData) sead::Delegate<T>(obj, method);
        mDelegateType = cDelegateType_SeadDelegate;
        setName(name);
    }

    void setPriority(u32 priority)
    {
        mPriority = priority;
    }

    u32 getPriority() const
    {
        return mPriority;
    }

    void invoke(const RenderInfo& render_info)
    {
        return reinterpret_cast<sead::Delegate1<Dummy, const RenderInfo&>*>(mDelegateData)->invoke(render_info);
    }

    static s32 compare(const DrawMethod* a, const DrawMethod* b);

private:
    DelegateType mDelegateType;
    u32 mPriority;
    u32 mDelegateData[8];   // Nintendo: "32 bytes... must be enough :)"
                            // (Note: they didn't change it for Switch, which results in misalignment)
};
static_assert(sizeof(DrawMethod) == 0x40, "agl::lyr::DrawMethod size mismatch");

} }
