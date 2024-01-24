#include <detail/aglRootNode.h>
#include <utility/aglNamedObjIndex.h>
#include <utility/aglNamedObjMgr.h>

namespace {

s32 compare(const agl::utl::INamedObj* a, const agl::utl::INamedObj* b);

}

namespace agl { namespace utl {

INamedObjMgr::INamedObjMgr()
{
    mObjIndex.initOffset(offsetof(INamedObjIndex, mListNode));
}

INamedObjMgr::~INamedObjMgr()
{
    mObj.freeBuffer();
    mGroupBuffer.freeBuffer();
    mGroupPtrArray.freeBuffer();
    mObjIndex.clear();
}

void INamedObjMgr::initialize(u32 obj_num, u32 group_num, sead::Heap* heap)
{
    mObj.allocBuffer(obj_num, heap);
    mGroupBuffer.allocBuffer(group_num, heap);
    mGroupPtrArray.allocBuffer(group_num, heap);

    for (
        sead::Buffer<Group>::iterator
            itr_group = mGroupBuffer.begin(),
            itr_group_end = mGroupBuffer.end()
        ;   itr_group != itr_group_end
        ; ++itr_group
    )
        itr_group->initialize(itr_group.getIndex(), this, heap);
}

void INamedObjMgr::pushBackNamedObj(INamedObj* p_obj)
{
  //SEAD_ASSERT(p_obj != nullptr);
    mObj.pushBack(p_obj);
}

void INamedObjMgr::constructList()
{
    mObj.heapSort(&compare);

    mGroupPtrArray.clear();
    for (
        sead::PtrArray<INamedObj>::constIterator
            itr_obj = mObj.constBegin(),
            itr_obj_end = mObj.constEnd()
        ;   itr_obj != itr_obj_end
        ; ++itr_obj
    )
    {
        bool exists = false;
        for (
            sead::PtrArray<Group>::constIterator
                itr_group = mGroupPtrArray.constBegin()
            ;   itr_group != mGroupPtrArray.constEnd()
            ; ++itr_group
        )
        {
            if (itr_group->getName() == itr_obj->getGroupName())
            {
                exists = true;
                break;
            }
        }
        if (exists)
            continue;
        
        mGroupBuffer[mGroupPtrArray.size()].reset(itr_obj->getGroupName());
        mGroupPtrArray.pushBack(&(mGroupBuffer[mGroupPtrArray.size()]));
    }
    mGroupPtrArray.heapSort(&Group::compare);

    for (
        sead::Buffer<Group>::constIterator
            itr_group = mGroupBuffer.constBegin(),
            itr_group_end = mGroupBuffer.constEnd()
        ;   itr_group != itr_group_end
        ; ++itr_group
    )
    {
    }
}

void INamedObjMgr::constructListByName(s32 group_index)
{
    for (
        sead::PtrArray<INamedObj>::constIterator
            itr_obj = mObj.constBegin(),
            itr_obj_end = mObj.constEnd()
        ;   itr_obj != itr_obj_end
        ; ++itr_obj
    )
    {
        if (group_index == -1 || mGroupBuffer[group_index].getName() == itr_obj->getGroupName())
        {
            if (itr_obj->isHostIOEnabled())
            {
            }
        }
    }
}

void INamedObjMgr::constructListByGroup(s32 group_index)
{
    s32 i_group = 0;
    for (
        sead::PtrArray<Group>::constIterator
            itr_group = mGroupPtrArray.constBegin()
        ;   itr_group != mGroupPtrArray.constEnd()
        ; ++itr_group
    )
    {
        if (group_index == -1 || i_group == group_index)
        {
            for (
                sead::PtrArray<INamedObj>::constIterator
                    itr_obj = mObj.constBegin(),
                    itr_obj_end = mObj.constEnd()
                ;   itr_obj != itr_obj_end
                ; ++itr_obj
            )
            {
                if (itr_group->getName() == itr_obj->getGroupName())
                {
                    if (itr_obj->isHostIOEnabled())
                    {
                    }
                }
            }
        }
        i_group++;
    }
}

void INamedObjMgr::updateList()
{
    for (
        sead::OffsetList<INamedObjIndex>::iterator
            itr_obj_index = mObjIndex.begin(),
            itr_obj_index_end = mObjIndex.end()
        ;   itr_obj_index != itr_obj_index_end
        ; ++itr_obj_index
    )
        itr_obj_index->syncNameToIndex();
}

INamedObjMgr::Group::Group()
    : mpMgr(nullptr)
    , mName("")
{
}

INamedObjMgr::Group::~Group()
{
}

void INamedObjMgr::Group::initialize(s32 index, INamedObjMgr* p_mgr, sead::Heap* heap)
{
    _34 = "";
    mpMgr = p_mgr;
    mIndex = index;
}

void INamedObjMgr::Group::reset(const sead::SafeString& name)
{
    mName = name;
    if (name != INamedObj::getDefaultGroupName())
        detail::RootNode::setNodeMeta(this, "Icon=FOLDER_GREEN");
    else
        detail::RootNode::setNodeMeta(this, "Icon=FOLDER_RED");
}

s32 INamedObjMgr::Group::compare(const Group* a, const Group* b)
{
    if (a->getName() < b->getName())
        return -1;

    return 1;
}

} }

namespace {

s32 compare(const agl::utl::INamedObj* a, const agl::utl::INamedObj* b)
{
    if (a->getObjType() < b->getObjType())
        return -1;

    if (b->getObjType() < a->getObjType())
        return 1;

    if (a->getObjName() < b->getObjName())
        return -1;

    return 1;
}

}
