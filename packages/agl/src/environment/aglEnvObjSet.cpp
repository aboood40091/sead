#include <detail/aglRootNode.h>
#include <environment/aglEnvObjMgr.h>
#include <environment/aglEnvObjSet.h>

namespace agl { namespace env {

EnvObjSet::EnvObjSet()
    : mpEnvObjMgr(nullptr)
{
    detail::RootNode::setNodeMeta(this, "Icon=CIRCLE_GREEN");
}

EnvObjSet::~EnvObjSet()
{
    mObjRefArrayBuffer.freeBuffer();
    bind(nullptr);
}

void EnvObjSet::bind(EnvObjMgr* p_env_mgr)
{
    mpEnvObjMgr = p_env_mgr;
    for (
        sead::Buffer<Ref>::iterator
            itr = mObjRefArrayBuffer.begin(),
            itr_end = mObjRefArrayBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
        itr->mEnvIndex.bind(p_env_mgr);
}

void EnvObjSet::allocBuffer(const AllocateArg& arg, sead::Heap* heap)
{
    EnvObjBuffer::allocBuffer(arg, heap);
    mObjRefArrayBuffer.allocBuffer(arg.getContainTotal());

    s32 count = 0;
    for (s32 type = 0; type < EnvObj::getTypeNum(); type++)
    {
        for (s32 i = 0; i < mTypeInfo[type].mMaxNum; i++)
        {
            EnvObj::Index& env_index = mObjRefArrayBuffer[count].mEnvIndex;
            env_index.setType(type);
            env_index.setCallback(this);
        }
    }
  //SEAD_ASSERT(count == getContainTotal());

    sead::FixedSafeString<32> name_value("EnvSet");
    mName.initialize(name_value, "name", "名前", &mSetting);

    for (
        sead::Buffer<Ref>::iterator
            itr = mObjRefArrayBuffer.begin(),
            itr_end = mObjRefArrayBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
    {
        Ref& ref = *itr;
        s32 index = itr.getIndex();

        s32 type = searchType(index);
      //SEAD_ASSERT(type != -1);
        {
            sead::FixedSafeString<32> value(EnvObj::getTypeName(type));
            ref.mType.initialize(value, "type", "タイプ", &ref);
        }
        {
            sead::FixedSafeString<32> value(sead::SafeString::cEmptyString);
            ref.mEnvIndex.initialize(value, "name", "名前", &ref);
        }
        mObjRefArrayParameterList.addObj(
            &ref,
            sead::FormatFixedSafeString<1024>(
                "%d",
                index
            )
        );
    }

    addObj(&mSetting, "setting");
    addList(&mObjRefArrayParameterList, "env_obj_ref_array");
}

bool EnvObjSet::preRead_()
{
    for (
        sead::Buffer<Ref>::iterator
            itr = mObjRefArrayBuffer.begin(),
            itr_end = mObjRefArrayBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
    {
        itr->mEnvIndex.setIndex(-1);
        itr->mEnvIndex.setValue(sead::SafeString::cEmptyString);
    }
    return true;
}

void EnvObjSet::postRead_()
{
    for (
        sead::Buffer<Ref>::iterator
            itr = mObjRefArrayBuffer.begin(),
            itr_end = mObjRefArrayBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
        itr->mEnvIndex.syncNameToIndex();
}

void EnvObjSet::callbackSyncNameToIndex(utl::INamedObjIndex* p_index)
{
  //SEAD_ASSERT(p_index != nullptr);
    EnvObj::Index* p_env_index = static_cast<EnvObj::Index*>(p_index);
    for (
        sead::Buffer<Ref>::iterator
            itr = mObjRefArrayBuffer.begin(),
            itr_end = mObjRefArrayBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
    {
        if (p_env_index == &itr->mEnvIndex)
        {
            s32 index = itr.getIndex();
            if (p_env_index->getIndex() >= 0)
            {
                EnvObj* p_obj = mpEnvObjMgr->getEnvObj(p_env_index->getType(), p_env_index->getIndex());
                mEnvObjPtrBuffer[index] = p_obj;
            }
            else
            {
                mEnvObjPtrBuffer[index] = nullptr;
            }
            break;
        }
    }
}

void EnvObjSet::callbackSyncIndexToName(utl::INamedObjIndex* p_index)
{
  //SEAD_ASSERT(p_index != nullptr);
    EnvObj::Index* p_env_index = static_cast<EnvObj::Index*>(p_index);
    for (
        sead::Buffer<Ref>::iterator
            itr = mObjRefArrayBuffer.begin(),
            itr_end = mObjRefArrayBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
    {
        if (p_env_index == &itr->mEnvIndex)
        {
            s32 index = itr.getIndex();
            if (p_env_index->getIndex() >= 0)
            {
                EnvObj* p_obj = mpEnvObjMgr->getEnvObj(p_env_index->getType(), p_env_index->getIndex());
                mEnvObjPtrBuffer[index] = p_obj;
            }
            else
            {
                mEnvObjPtrBuffer[index] = nullptr;
            }
            break;
        }
    }
}

bool EnvObjSet::pushBack(EnvObj* p_obj)
{
    if (p_obj != nullptr)
    {
        for (s32 i = 0; i < mTypeInfo[p_obj->getTypeID()].mMaxNum; i++)
        {
            s32 index = mTypeInfo[p_obj->getTypeID()].mStartIndex + i;
            EnvObj::Index& env_index = mObjRefArrayBuffer[index].mEnvIndex;
            if (env_index.getIndex() < 0 && env_index.getIndex() != -2)
            {
                mEnvObjPtrBuffer[index] = p_obj;

                env_index.setValue(p_obj->getEnvObjName());
                env_index.syncNameToIndex();
              //SEAD_ASSERT(env_index.getIndex() == p_obj->getIndex());

                return true;
            }
        }
    }
    return false;
}

bool EnvObjSet::Ref::isApply_(utl::ResParameterObj obj) const
{
    if (mEnvIndex.getValue() != sead::SafeString::cEmptyString)
        return false;

    utl::ResParameter type_param = obj.searchResParameter(utl::ParameterBase::calcHash("type"));
    if (mType.getValue() == static_cast<const char*>(type_param.getValue()))
        return true;

    return false;
}

} }
