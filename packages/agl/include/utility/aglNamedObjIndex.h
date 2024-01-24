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

    virtual const sead::SafeString& getNamedObjName(s32 index) const = 0;
    virtual s32 getNamedObjNum() const = 0;

    void bind(INamedObjMgr* p_mgr);

    s32 getIndex() const
    {
        return mIndex;
    }

    void setIndex(s32 index)
    {
        mIndex = index;
    }

    void setCallback(INamedObjIndexCallback* p_callback)
    {
        mpCallback = p_callback;
    }

    void syncNameToIndex();

protected:
    sead::ListNode mListNode;
    INamedObjMgr* mpObjMgr;
    s32 mIndex;
    INamedObjIndexCallback* mpCallback;

    friend class INamedObjMgr;
};
static_assert(sizeof(INamedObjIndex) == 0x50);

class INamedObjIndexCallback
{
public:
    virtual void callbackSyncNameToIndex(INamedObjIndex* p_index) { }
    virtual void callbackSyncIndexToName(INamedObjIndex* p_index) { }
};
static_assert(sizeof(INamedObjIndexCallback) == 4);

} }