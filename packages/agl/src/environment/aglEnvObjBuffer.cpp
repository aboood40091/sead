#include <environment/aglEnvObjBuffer.h>

namespace {

s32 compare(agl::env::EnvObj* const* a, agl::env::EnvObj* const* b);

}

namespace agl { namespace env {

EnvObjBuffer::EnvObjBuffer()
    : _0(0)
    , _4(0)
{
}

EnvObjBuffer::~EnvObjBuffer()
{
    EnvObjBuffer::freeBuffer();
}

void EnvObjBuffer::allocBuffer(const AllocateArg& arg, sead::Heap* heap)
{
  //SEAD_ASSERT( 0 < arg.getContainTotal() );
  //SEAD_ASSERT( 0 < EnvObj::getTypeNum() );
    mEnvObjPtrBuffer.allocBuffer(arg.getContainTotal(), heap);
    mTypeInfo.allocBuffer(EnvObj::getTypeNum(), heap);

    s32 size_count = 0;
    for (
        sead::Buffer<TypeInfo>::iterator
            itr = mTypeInfo.begin(),
            itr_end = mTypeInfo.end()
        ;   itr != itr_end
        ; ++itr
    )
    {
      //SEAD_ASSERT( size_count <= 0xffff );
      //SEAD_ASSERT( arg.getContainMax( itr.getIndex() ) <= 0xffff );
        itr->mStartIndex = size_count;
        itr->mMaxNum = arg.getContainMax(itr.getIndex());
        size_count += itr->mMaxNum;
    }

    for (
        sead::Buffer<EnvObj*>::iterator
            itr = mEnvObjPtrBuffer.begin(),
            itr_end = mEnvObjPtrBuffer.end()
        ;   itr != itr_end
        ; ++itr
    )
        *itr = nullptr;
}

void EnvObjBuffer::freeBuffer()
{
    mEnvObjPtrBuffer.freeBuffer();
    mTypeInfo.freeBuffer();
}

void EnvObjBuffer::sort(s32 type)
{
    if (mTypeInfo[type].mMaxNum == 0)
        return;

    mEnvObjPtrBuffer.heapSort(
        &compare,
        mTypeInfo[type].mStartIndex,
        mTypeInfo[type].mStartIndex + mTypeInfo[type].mMaxNum - 1
    );
}

s32 EnvObjBuffer::searchBufferIndex(s32 type, const sead::SafeString& name) const
{
    for (
        sead::Buffer<EnvObj*>::constIterator
            itr = mEnvObjPtrBuffer.toIterator(mTypeInfo[type].mStartIndex),
            itr_end = mEnvObjPtrBuffer.toIterator(mTypeInfo[type].mStartIndex + mTypeInfo[type].mMaxNum)
        ;   itr != itr_end
        ; ++itr
    )
    {
        EnvObj* p_obj = *itr;
        if (p_obj && p_obj->getEnvObjName() == name)
            return itr.getIndex();
    }

    return -1;
}

s32 EnvObjBuffer::searchTypeIndex(s32 type, const sead::SafeString& name) const
{
    s32 buffer_index = searchBufferIndex(type, name);
    if (buffer_index >= 0)
        return buffer_index - mTypeInfo[type].mStartIndex;

    return -1;
}

s32 EnvObjBuffer::searchType(s32 buffer_index) const
{
    for (s32 type = 0, type_num = EnvObj::getTypeNum(); type < type_num; type++)
        if (mTypeInfo[type].mStartIndex <= buffer_index && buffer_index < mTypeInfo[type].mStartIndex + mTypeInfo[type].mMaxNum)
            return type;

    return -1;
}

EnvObjBuffer::AllocateArg::AllocateArg()
    : mTotal(0)
{
    mTypeMax.fill(0);
}

void EnvObjBuffer::AllocateArg::setContainMax(s32 type, s32 max)
{
    mTotal -= mTypeMax[type];
    mTypeMax[type] = max;
    mTotal += max;
}

} }

namespace {

s32 compare(agl::env::EnvObj* const* a, agl::env::EnvObj* const* b)
{
    if (*a == nullptr)
        return 1;

    if (*b == nullptr)
        return -1;

    if ((*a)->getEnvObjName() < (*b)->getEnvObjName())
        return -1;

    return 1;
}

}
