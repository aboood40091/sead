#pragma once

#include <container/seadOffsetList.h>
#include <prim/seadSafeString.h>
#include <utility/aglResParameter.h>

namespace agl { namespace utl {

class IParameterObj;

class IParameterList
{
public:
    IParameterList();

    void addObj(IParameterObj* child, const sead::SafeString& name);

    void applyResParameterList(ResParameterList list);

protected:
    virtual bool preWrite_() const { return true; }
    virtual void postWrite_() const { }
    virtual bool preRead_() { return true; }
    virtual void postRead_() { }
    virtual bool isApply_(ResParameterList list) const { return list.getParameterListNameHash() == mNameHash; }
    virtual void callbackNotAppliable_(IParameterObj*, ResParameter) { }

    void setParameterListName_(const sead::SafeString& name);

    void applyResParameterList_(ResParameterList list, bool lerp = false, f32 t = 1.0f);
    void applyResParameterListB_(ResParameterList list, f32 t);

protected:
    sead::OffsetList<IParameterList> mChildList;
    sead::OffsetList<IParameterObj> mChildObj;
    sead::FixedSafeString<64> mName;
    u32 mNameHash;
    u32 _70;
    sead::ListNode mListNode;

    friend class IParameterObj;
};
static_assert(sizeof(IParameterList) == 0x80, "agl::utl::IParameterList size mismatch");

} }
