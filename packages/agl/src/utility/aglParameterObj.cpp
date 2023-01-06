#include <codec/seadHashCRC32.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace utl {

IParameterObj::IParameterObj()
    : mChildHash(0xFFFFFFFF)
{
    mChildParameter.initOffset(offsetof(ParameterBase, mListNode));

    mName = sead::SafeString::cEmptyString;
}

void IParameterObj::pushBackListNode(ParameterBase* p_node)
{
    // SEAD_ASSERT(p_node != nullptr);

    u32 child_hash[1024];
    u32 child_num = 0;

    for (sead::OffsetList<ParameterBase>::iterator itr = mChildParameter.begin(), itr_end = mChildParameter.end(); itr != itr_end; ++itr)
        child_hash[child_num++] = itr->mHash;

    mChildHash = sead::HashCRC32::calcHash(child_hash, child_num * sizeof(u32));

    mChildParameter.pushBack(p_node);
}

} }
