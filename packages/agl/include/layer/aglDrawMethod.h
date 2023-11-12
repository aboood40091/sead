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
        cDelegateType_Dummy     = 0,
        cDelegateType_Const     = 1,
        cDelegateType_NonConst  = 2
    };

    class Dummy
    {
    };

public:
    DrawMethod()
        : sead::IDisposer()
        , sead::INamable("")
        , mDelegateType(cDelegateType_Dummy)
        , mPriority(0)
    {
        new (&mDelegateHolder) sead::Delegate<Dummy>();
    }

    virtual ~DrawMethod();

    template <typename T>
    void bind(T* obj, typename sead::Delegate1Const<T, const RenderInfo&>::MethodPtr method, const sead::SafeString& name)
    {
        new (&mDelegateHolder) sead::Delegate1Const<T, const RenderInfo&>(obj, method);
        mDelegateType = cDelegateType_Const;
        setName(name);
    }

    template <typename T>
    void bind(T* obj, typename sead::Delegate1<T, const RenderInfo&>::MethodPtr method, const sead::SafeString& name)
    {
        new (&mDelegateHolder) sead::Delegate1<T, const RenderInfo&>(obj, method);
        mDelegateType = cDelegateType_NonConst;
        setName(name);
    }

    void setPriority(s32 priority)
    {
        mPriority = priority;
    }

    s32 getPriority() const
    {
        return mPriority;
    }

    void invoke(const RenderInfo& render_info)
    {
        return reinterpret_cast<sead::Delegate1<Dummy, const RenderInfo&>*>(&mDelegateHolder)->invoke(render_info);
    }

    void operator() (const RenderInfo& render_info)
    {
        return invoke(render_info);
    }

    static s32 compare(const DrawMethod* a, const DrawMethod* b);

private:
    DelegateType mDelegateType;
    s32 mPriority;
    struct
    {
        u32 data_[8];       // Nintendo: "32 bytes... must be enough :)"
                            // (Note: they didn't change it for Switch, which results in misalignment)
    } mDelegateHolder;
};
static_assert(sizeof(DrawMethod) == 0x40, "agl::lyr::DrawMethod size mismatch");

} }
