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

    void addList(IParameterList* child, const sead::SafeString& name);
    void addObj(IParameterObj* child, const sead::SafeString& name);

    void applyResParameterList(ResParameterList list);

    bool isApply(ResParameterList list) const { return list.getParameterListNameHash() == mNameHash; }

protected:
    virtual bool preWrite_() const { return true; }
    virtual void postWrite_() const { }
    virtual bool preRead_() { return true; }
    virtual void postRead_() { }
    virtual bool isApply_(ResParameterList list) const { return isApply(list); }
    virtual void callbackNotAppliable_(IParameterObj*, ResParameter) { }

    void setParameterListName_(const sead::SafeString& name);

    void applyResParameterList_(ResParameterList list, bool lerp = false, f32 t = 1.0f);
    void applyResParameterListB_(ResParameterList list, f32 t);

protected:
    sead::OffsetList<IParameterList> mChildList;
    sead::OffsetList<IParameterObj> mChildObj;
    sead::FixedSafeString<64> mName;
    u32 mNameHash;
    sead::ListNode mListNode;
    IParameterList* mpParent;

    friend class IParameterObj;
};
static_assert(sizeof(IParameterList) == 0x80, "agl::utl::IParameterList size mismatch");

} }
