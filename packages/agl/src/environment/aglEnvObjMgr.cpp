#include <detail/aglRootNode.h>
#include <environment/aglEnvObjMgr.h>
#include <gfx/seadGraphics.h>
#include <utility/aglDevTools.h>

namespace agl { namespace env {

EnvObjMgr::EnvObjMgr()
    : utl::IParameterIO("aglenv")
    , mFlag(0x200)
    , _24c(1.0f)
    , mConstructListType(2)
    , mConstructListGroupIndex(-1)
    , mApplyResourceGroupIndex(-1)
    , _45c(0)
    , _460(0)
{
}

EnvObjMgr::~EnvObjMgr()
{
    for (sead::Buffer<EnvObj*>::iterator itr_obj = mEnvObjPtrBuffer.begin(), itr_obj_end = mEnvObjPtrBuffer.end(); itr_obj != itr_obj_end; ++itr_obj)
    {
      //SEAD_ASSERT(*itr_obj != nullptr);
        delete *itr_obj;
    }

    mEnvObjPtrArray.freeBuffer();
    mTypeNode.freeBuffer();

    for (sead::Buffer<ViewInfo>::iterator itr = mViewInfo.begin(), itr_end = mViewInfo.end(); itr != itr_end; ++itr)
        ; // lol
}

void EnvObjMgr::initialize(const InitArg& arg, sead::Heap* heap)
{
    allocBuffer(arg, heap);
    mEnvObjSet.allocBuffer(arg, heap);
    mEnvObjSet.bind(this);
  //SEAD_ASSERT(0 < getContainTotal());
    mEnvObjPtrArray.allocBuffer(getContainTotal(), heap);
    mTypeNode.allocBuffer(EnvObj::getTypeNum(), heap);
    mViewInfo.allocBuffer(arg.getViewMax(), heap);
    for (
        sead::Buffer<TypeNode>::iterator
            itr_type_node = mTypeNode.begin(),
            itr_type_node_end = mTypeNode.end()
        ;   itr_type_node != itr_type_node_end
        ; ++itr_type_node
    )
    {
        TypeNode& type_node = *itr_type_node;
        s32 type = itr_type_node.getIndex();
        type_node.initialize(type, this, heap);
        addList(&type_node, EnvObj::getTypeName(type));
        for (u32 i = 0; i < mTypeInfo[type].mMaxNum; i++)
        {
            EnvObj* p_obj = (*EnvObj::getTypeCreateFunc(type))(heap);
            p_obj->initialize_(i, arg.getViewMax(), this, heap);
            mEnvObjPtrBuffer[mTypeInfo[type].mStartIndex + i] = p_obj;
            bool result = mEnvObjSet.pushBack(p_obj);
          //SEAD_ASSERT(result);
            type_node.addObj(
                p_obj,
                sead::FormatFixedSafeString<1024>(
                    "%s%d",
                    EnvObj::getTypeName(p_obj->getTypeID()).cstr(),
                    i
                )
            );
        }
    }

    for (
        sead::Buffer<ViewInfo>::iterator
            itr_view_info = mViewInfo.begin(),
            itr_view_info_end = mViewInfo.end()
        ;   itr_view_info != itr_view_info_end
        ; ++itr_view_info
    )
    {
        ViewInfo& view_info = *itr_view_info;
        view_info.view_mtx.makeIdentity();
        view_info.proj_mtx.makeIdentity();
        view_info.near = 0.01f;
        view_info.far = 10000.0f;
        view_info._78 = 0;
        view_info._7c.makeAllZero();
        view_info.p_render_buffer = nullptr;
        view_info._84 = 0.7853982f;
    }

    for (
        sead::UnsafeArray<u32, 4>::iterator
            itr_unk = _42c.begin(),
            itr_unk_end = _42c.end()
        ;   itr_unk != itr_unk_end
        ; ++itr_unk
    )
        *itr_unk = 0;

    utl::INamedObjMgr::initialize(getContainTotal(), arg.getGroupMax(), heap);
    for (
        sead::Buffer<EnvObj*>::iterator
            itr_p_obj = mEnvObjPtrBuffer.begin(),
            itr_p_obj_end = mEnvObjPtrBuffer.end()
        ;   itr_p_obj != itr_p_obj_end
        ; ++itr_p_obj
    )
        pushBackNamedObj(*itr_p_obj);

    detail::RootNode::setNodeMeta(this, "Icon = LIGHT");

    mFlag.set(1 << 0 |
              1 << 1);
}

void EnvObjMgr::update()
{
    if (mFlag.isOnBit(0) && mFlag.isOnBit(9))
    {
        mEnvObjPtrArray.clear();
        for (
            sead::Buffer<EnvObj*>::constIterator
                itr_p_obj = mEnvObjPtrBuffer.constBegin(),
                itr_p_obj_end = mEnvObjPtrBuffer.constEnd()
            ;   itr_p_obj != itr_p_obj_end
            ; ++itr_p_obj
        )
        {
            EnvObj* const p_obj = *itr_p_obj;
            if (p_obj->isEnable())
                mEnvObjPtrArray.pushBack(p_obj);
        }

        mFlag.resetBit(0);
    }

    for (
        sead::PtrArray<EnvObj>::iterator
            itr_obj = mEnvObjPtrArray.begin(),
            itr_obj_end = mEnvObjPtrArray.end()
        ;   itr_obj != itr_obj_end
        ; ++itr_obj
    )
        itr_obj->update_();

    if (mFlag.isOnBit(1) && mFlag.isOnBit(9))
    {
        constructList();
        updateList();

        mFlag.resetBit(1);
    }
}

void EnvObjMgr::updateView(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index)
{
    ViewInfo& view_info = mViewInfo[view_index];

    view_info.view_mtx = view_mtx;
    view_info.proj_mtx = proj_mtx;

    view_info._78 = 0;

    f32 v = (1.0f - proj_mtx(2, 2)) / (proj_mtx(2, 2) + 1.0f);
    view_info.far = proj_mtx(2, 3) * 0.5f * (v + 1.0f);
    view_info.near = -view_info.far / v;

    for (
        sead::PtrArray<EnvObj>::iterator
            itr_obj = mEnvObjPtrArray.begin(),
            itr_obj_end = mEnvObjPtrArray.end()
        ;   itr_obj != itr_obj_end
        ; ++itr_obj
    )
        itr_obj->updateView(view_mtx, proj_mtx, view_index);
}

void EnvObjMgr::constructList()
{
    utl::INamedObjMgr::constructList();

    for (
        sead::Buffer<EnvObj*>::iterator
            itr_p_obj = mEnvObjPtrBuffer.begin(),
            itr_p_obj_end = mEnvObjPtrBuffer.end()
        ;   itr_p_obj != itr_p_obj_end
        ; ++itr_p_obj
    )
    {
    }

    sead::Buffer<TypeNode>::iterator
        itr_type_node_begin = mTypeNode.begin(),
        itr_type_node_end = mTypeNode.end();

    for (
        sead::Buffer<TypeNode>::iterator
            itr_type_node = itr_type_node_begin
        ;   itr_type_node != itr_type_node_end
        ; ++itr_type_node
    )
    {
    }

    switch (mConstructListType)
    {
    default:
        break;
    case 1:
        for (
            sead::Buffer<TypeNode>::iterator
                itr_type_node = itr_type_node_begin
            ;   itr_type_node != itr_type_node_end
            ; ++itr_type_node
        )
        {
            s32 type = itr_type_node.getIndex();
            mEnvObjSet.sort(type);

            if (mTypeInfo[type].mMaxNum == 0)
                continue;

            for (
                sead::Buffer<EnvObj*>::iterator
                    itr_obj = mEnvObjSet.mEnvObjPtrBuffer.toIterator(mEnvObjSet.mTypeInfo[type].mStartIndex),
                    itr_obj_end = mEnvObjSet.mEnvObjPtrBuffer.toIterator(mEnvObjSet.mTypeInfo[type].mStartIndex + mEnvObjSet.mTypeInfo[type].mMaxNum)
                ;   itr_obj != itr_obj_end
                ; ++itr_obj
            )
            {
                EnvObj* p_obj = *itr_obj;
                if (p_obj != nullptr && mConstructListGroupIndex != -1 && isGroup(mConstructListGroupIndex, *p_obj))
                {
                }
            }
        }
        break;
    case 2:
        constructListByGroup(mConstructListGroupIndex);
        break;
    case 0:
        constructListByName(mConstructListGroupIndex);
        break;
    }
}

void EnvObjMgr::listenPropertyEventFromGroup(GroupEventType, Group*)
{
    // Deleted from NSMBU
}

bool EnvObjMgr::saveImpl_(const sead::SafeString&, u32, s32) const
{
    // Deleted from NSMBU
    // Uses sead::XmlDocument to write this parameter to an XML file
    return false;
}

void EnvObjMgr::applyResource_(utl::ResParameterArchive arc_a, utl::ResParameterArchive arc_b, f32 t)
{
    // This function is nowhere near matching. Honestly, the disassembly was pretty cursed, most
    // likely because of inlined iterators, but oh well. I'll just assume this is good enough.

    sead::Graphics::instance()->lockDrawContext();
    {
      //SEAD_ASSERT(arc_a.isValid());
        utl::ResParameterList list_a = arc_a.getResParameterList();
        if (list_a.getResParameterListNum() > 0)
        {
            utl::ResParameterList envobj_list_a_first = list_a.getResParameterList(0);
            if (list_a.getResParameterListNum() > 0)
            {
                utl::ResParameterList envobj_list_a_last = list_a.getResParameterList(list_a.getResParameterListNum() - 1);
                for (
                    utl::ResParameterList::listConstIterator
                        itr_envobj_list_a = utl::ResParameterList::listConstIterator(0, envobj_list_a_first.ptr()),
                        itr_envobj_list_a_end = list_a.listConstEnd()
                    ;   itr_envobj_list_a != itr_envobj_list_a_end
                    ; ++itr_envobj_list_a
                )
                {
                    utl::ResParameterList envobj_list_a = &(*itr_envobj_list_a);
                    s32 env_type = 0;
                    {
                        bool found = false;
                        for (
                            sead::OffsetList<utl::IParameterList>::iterator
                                itr_child_list = mChildList.begin(),
                                itr_child_list_end = mChildList.end()
                            ;   itr_child_list != itr_child_list_end
                            ; ++itr_child_list, env_type++
                        )
                        {
                            if (itr_child_list->isApply(envobj_list_a))
                            {
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                            continue;
                    }

                    s32 obj_index = -1;

                    if (envobj_list_a.getResParameterObjNum() > 0)
                    {
                        utl::ResParameterObj subobj_a_first = envobj_list_a.getResParameterObj(0);
                        if (envobj_list_a.getResParameterObjNum() > 0)
                        {
                            utl::ResParameterObj subobj_a_last = envobj_list_a.getResParameterObj(envobj_list_a.getResParameterObjNum() - 1);
                            for (
                                utl::ResParameterList::objConstIterator
                                    itr_subobj_a = utl::ResParameterList::objConstIterator(0, subobj_a_first.ptr()),
                                    itr_subobj_a_end = envobj_list_a.objConstEnd()
                                ;   itr_subobj_a != itr_subobj_a_end
                                ; ++itr_subobj_a
                            )
                            {
                                utl::ResParameterObj subobj_a = &(*itr_subobj_a);

                                u32 group_hash = utl::ParameterBase::calcHash("group");
                                utl::ResParameter group_param_a;
                                {
                                    bool found = false;
                                    for (
                                        utl::ResParameterObj::constIterator
                                            itr_param = subobj_a.constBegin(),
                                            itr_param_end = subobj_a.constEnd()
                                        ;   itr_param != itr_param_end
                                        ; ++itr_param
                                    )
                                    {
                                        utl::ResParameter param = &(*itr_param);
                                        if (group_hash == param.getParameterNameHash())
                                        {
                                            found = true;
                                            group_param_a = param;
                                            break;
                                        }
                                    }
                                    if (!found)
                                        group_param_a = nullptr;
                                }

                                u32 name_hash = utl::ParameterBase::calcHash("name");
                                utl::ResParameter name_param_a;
                                {
                                    bool found = false;
                                    for (
                                        utl::ResParameterObj::constIterator
                                            itr_param = subobj_a.constBegin(),
                                            itr_param_end = subobj_a.constEnd()
                                        ;   itr_param != itr_param_end
                                        ; ++itr_param
                                    )
                                    {
                                        utl::ResParameter param = &(*itr_param);
                                        if (name_hash == param.getParameterNameHash())
                                        {
                                            found = true;
                                            name_param_a = param;
                                            break;
                                        }
                                    }
                                    if (!found)
                                        name_param_a = nullptr;
                                }

                                if (!group_param_a.isValid() || !name_param_a.isValid())
                                    continue;

                                sead::SafeString subobj_a_name = static_cast<const char*>(name_param_a.getValue());
                                sead::SafeString subobj_a_group = static_cast<const char*>(group_param_a.getValue());

                                if (mApplyResourceGroupIndex != -1 && mGroupBuffer[mApplyResourceGroupIndex].getName() != subobj_a_group)
                                    continue;

                                EnvObj* p_obj = nullptr;
                                for (
                                    sead::Buffer<EnvObj*>::iterator
                                        itr_obj_a = mEnvObjPtrBuffer.toIterator(mTypeInfo[env_type].mStartIndex),
                                        itr_obj_a_end = mEnvObjPtrBuffer.toIterator(mTypeInfo[env_type].mStartIndex + mTypeInfo[env_type].mMaxNum)
                                    ;   itr_obj_a != itr_obj_a_end
                                    ; ++itr_obj_a
                                )
                                {
                                    if (p_obj == nullptr)
                                    {
                                        s32 index = itr_obj_a.getIndex();
                                        if (obj_index < index)
                                        {

                                            if ((*itr_obj_a)->getGroupName() == utl::INamedObj::getDefaultGroupName())
                                            {
                                                p_obj = *itr_obj_a;
                                                obj_index = index;
                                            }

                                            if ((*itr_obj_a)->getEnvObjName() == subobj_a_name)
                                            {
                                                p_obj = *itr_obj_a;
                                                break;
                                            }
                                        }
                                    }
                                }

                                if (p_obj == nullptr || !p_obj->isEnableHostIO())
                                    continue;

                                if (arc_a.ptr() != arc_b.ptr())
                                {
                                    utl::ResParameterList envobj_list_b = nullptr;

                                    utl::ResParameterList list_b = arc_b.getResParameterList();
                                    if (list_b.getResParameterListNum() > 0)
                                    {
                                        utl::ResParameterList envobj_list_b_first = list_b.getResParameterList(0);
                                        if (list_b.getResParameterListNum() > 0)
                                        {
                                            utl::ResParameterList envobj_list_b_last = list_b.getResParameterList(list_b.getResParameterListNum() - 1);
                                            for (
                                                utl::ResParameterList::listConstIterator
                                                    itr_envobj_list_b = utl::ResParameterList::listConstIterator(0, envobj_list_b_first.ptr()),
                                                    itr_envobj_list_b_end = list_b.listConstEnd()
                                                ;   itr_envobj_list_b != itr_envobj_list_b_end
                                                ; ++itr_envobj_list_b
                                            )
                                            {
                                                if (envobj_list_a.getParameterListNameHash() == itr_envobj_list_b->mNameHash)
                                                {
                                                    envobj_list_b = &(*itr_envobj_list_b);
                                                    break;
                                                }
                                            }
                                        }
                                    }

                                    if (envobj_list_b.getResParameterObjNum() > 0)
                                    {
                                        utl::ResParameterObj subobj_b_first = envobj_list_b.getResParameterObj(0);
                                        if (envobj_list_b.getResParameterObjNum() > 0)
                                        {
                                            utl::ResParameterObj subobj_b_last = envobj_list_b.getResParameterObj(envobj_list_b.getResParameterObjNum() - 1);
                                            for (
                                                utl::ResParameterList::objConstIterator
                                                    itr_subobj_b = utl::ResParameterList::objConstIterator(0, subobj_b_first.ptr()),
                                                    itr_subobj_b_end = envobj_list_b.objConstEnd()
                                                ;   itr_subobj_b != itr_subobj_b_end
                                                ; ++itr_subobj_b
                                            )
                                            {
                                                utl::ResParameterObj subobj_b = &(*itr_subobj_b);

                                                u32 name_hash = utl::ParameterBase::calcHash("name");
                                                utl::ResParameter name_param_b;
                                                {
                                                    bool found = false;
                                                    for (
                                                        utl::ResParameterObj::constIterator
                                                            itr_param = subobj_b.constBegin(),
                                                            itr_param_end = subobj_b.constEnd()
                                                        ;   itr_param != itr_param_end
                                                        ; ++itr_param
                                                    )
                                                    {
                                                        utl::ResParameter param = &(*itr_param);
                                                        if (name_hash == param.getParameterNameHash())
                                                        {
                                                            found = true;
                                                            name_param_b = param;
                                                            break;
                                                        }
                                                    }
                                                    if (!found)
                                                        name_param_b = nullptr;
                                                }

                                                sead::SafeString subobj_b_name = static_cast<const char*>(name_param_b.getValue());
                                                if (subobj_a_name == subobj_b_name)
                                                {
                                                    p_obj->applyResParameterObj(subobj_a, subobj_b, t);
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                else
                                {
                                    p_obj->applyResParameterObj(subobj_a);
                                }
                            }
                        }
                    }
                }
            }
        }

        mFlag.setBit(0);
    }
    sead::Graphics::instance()->unlockDrawContext();
}

ShaderMode EnvObjMgr::drawFog_(s32 view_index, const EnvObj& obj, f32 start, f32 end, const sead::Vector3f& direction, const sead::Color4f& color, ShaderMode mode) const
{
    const ViewInfo& view_info = mViewInfo[view_index];
    if (view_info.p_render_buffer == nullptr)
        return mode;

    const f32 depth_values[2] = {
        start,
        end
    };

    const sead::Color4f depth_colors[2] = {
        sead::Color4f::cBlack,
        sead::Color4f::cWhite
    };

    return utl::DevTools::drawDepthGradation(*view_info.p_render_buffer, 2, depth_values, depth_colors, view_info.near, view_info.far, mode);
}

EnvObjMgr::InitArg::InitArg()
    : mGroupMax(256)
    , mViewMax(1)
{
}

EnvObjMgr::TypeNode::TypeNode()
    : mType(0)
    , mpMgr(nullptr)
{
    detail::RootNode::setNodeMeta(this, "Icon=FOLDER_BLUE");
}

void EnvObjMgr::TypeNode::initialize(s32 type, EnvObjMgr* p_mgr, sead::Heap* heap)
{
    mType = type;
    mpMgr = p_mgr;
}

} }
