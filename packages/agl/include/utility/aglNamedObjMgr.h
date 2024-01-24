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

        void initialize(s32 index, INamedObjMgr* p_mgr, sead::Heap* heap);
        void reset(const sead::SafeString& name);

        const sead::SafeString& getName() const
        {
            return mName;
        }

    public:
        static s32 compare(const Group* a, const Group* b);

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

    void initialize(u32 obj_num, u32 group_num, sead::Heap* heap);
    void pushBackNamedObj(INamedObj* p_obj);

    virtual const sead::SafeString& getNamedObjName(s32 index, s32 type) const
    {
        return mObj[index]->getObjName();
    }

    virtual s32 getNamedObjNum(s32 type) const
    {
        return mObj.size();
    }

    virtual const sead::SafeString& getSaveFilePath() const
    {
        return sead::SafeString::cEmptyString;
    }

    virtual void listenPropertyEventFromGroup(GroupEventType, Group*)
    {
    }

    bool isGroup(s32 group_index, const agl::utl::INamedObj& obj) const
    {
        return group_index == -1 || mGroupBuffer[group_index].getName() == obj.getGroupName();
    }

    void constructList();
    void constructListByName(s32 group_index);
    void constructListByGroup(s32 group_index);
    void updateList();

protected:
    sead::PtrArray<INamedObj> mObj;
    sead::Buffer<Group> mGroupBuffer;
    sead::PtrArray<Group> mGroupPtrArray;
    sead::OffsetList<INamedObjIndex> mObjIndex;

    friend class INamedObjIndex;
};
static_assert(sizeof(INamedObjMgr) == 0x34);

} }
