#pragma once

#include <container/seadOffsetList.h>
#include <prim/seadSafeString.h>
#include <utility/aglResParameter.h>

namespace agl { namespace utl {

class IParameterList;
class ParameterBase;

class IParameterObj
{
public:
    IParameterObj();

    void pushBackListNode(ParameterBase* p_node);

    void applyResParameterObj_(ResParameterObj obj, bool lerp, f32 t, IParameterList* p_list);

protected:
    virtual bool preWrite_() const { return true; }
    virtual void postWrite_() const { }
    virtual bool preRead_() { return true; }
    virtual void postRead_() { }
    virtual bool isApply_(ResParameterObj obj) const { return obj.getParameterObjNameHash() == mNameHash; }

protected:
    sead::OffsetList<ParameterBase> mChildParameter;
    sead::FixedSafeString<64> mName;
    u32 mNameHash;
    u32 mChildHash;
    sead::ListNode mListNode;

    friend class IParameterList;
};
static_assert(sizeof(IParameterObj) == 0x70, "agl::utl::IParameterObj size mismatch");

} }
