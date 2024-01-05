#pragma once

#include <utility/aglParameter.h>

namespace agl { namespace utl {

class INamedObjMgr;
class INamedObjIndexCallback;

class INamedObjIndex : public utl::Parameter< sead::FixedSafeString<32> >
{
public:
    INamedObjIndex();
    virtual ~INamedObjIndex();

    virtual const sead::SafeString& getNamedObjName(s32) const = 0;
    virtual s32 getNamedObjNum() const = 0;

protected:
    sead::ListNode mListNode;
    INamedObjMgr* mpObjMgr;
    u32 _48;
    INamedObjIndexCallback* mpCallback;
};
static_assert(sizeof(INamedObjIndex) == 0x50);

class INamedObjIndexCallback
{
public:
    virtual void callbackSyncNameToIndex(INamedObjIndex* p_index) = 0;
    virtual void callbackSyncIndexToName(INamedObjIndex* p_index) = 0;
};
static_assert(sizeof(INamedObjIndexCallback) == 4);

} }