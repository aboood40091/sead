#pragma once

#include <container/seadBuffer.h>
#include <container/seadOffsetList.h>
#include <container/seadPtrArray.h>
#include <utility/aglNamedObj.h>

namespace agl { namespace utl {

class INamedObjIndex;

class INamedObjMgr : public sead::hostio::Node
{
public:
    enum GroupEventType
    {
    };

    class Group : public sead::hostio::Node
    {
    public:
        Group();
        virtual ~Group();

    private:
        INamedObjMgr* mpMgr;
        s32 mIndex;
        sead::FixedSafeString<32> mName;
        sead::FixedSafeString<256> _34;
    };
    static_assert(sizeof(Group) == 0x144);

public:
    INamedObjMgr();
    virtual ~INamedObjMgr();

    virtual const sead::SafeString& getNamedObjName(s32 index, s32 type) const
    {
        return mObj[index]->getObjName();
    }

    virtual s32 getNamedObjNum(s32 type) const
    {
        return mObj.size();
    }

    virtual void constructList();

    virtual void listenPropertyEventFromGroup(GroupEventType, Group*)
    {
    }

protected:
    sead::PtrArray<INamedObj> mObj;
    sead::Buffer<Group> mGroupBuffer;
    sead::PtrArray<Group> mGroupPtrArray;
    sead::OffsetList<INamedObjIndex> mObjIndex;
};
static_assert(sizeof(INamedObjMgr) == 0x34);

} }
