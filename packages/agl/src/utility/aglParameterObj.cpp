#include <codec/seadHashCRC32.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterList.h>
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

    for (
        sead::OffsetList<ParameterBase>::iterator
            itr = mChildParameter.begin(),
            itr_end = mChildParameter.end()
        ;   itr != itr_end
        ; ++itr
    )
        child_hash[child_num++] = itr->mHash;

    mChildHash = sead::HashCRC32::calcHash(child_hash, child_num * sizeof(u32));

    mChildParameter.pushBack(p_node);
}

s32 IParameterObj::makeZero_()
{
    if (!preRead_())
        return 0;

    s32 ret = 0;

    for (
        sead::OffsetList<ParameterBase>::iterator
            itr = mChildParameter.begin(),
            itr_end = mChildParameter.end()
        ;   itr != itr_end
        ; ++itr
    )
        if (itr->makeZero())
            ret += 0;

    return ret;
}

void IParameterObj::applyResParameterObj_(ResParameterObj obj, bool lerp, f32 t, IParameterList* p_list)
{
    if (!preRead_())
        return;

    for (
        ResParameterObj::constIterator
            itr_res = obj.constBegin(),
            itr_res_end = obj.constEnd()
        ;   itr_res != itr_res_end
        ; ++itr_res
    )
    {
        ResParameter res = &(*itr_res);

        bool is_apply = false;

        for (
            sead::OffsetList<ParameterBase>::iterator
                itr_param = mChildParameter.begin(),
                itr_param_end = mChildParameter.end()
            ;   itr_param != itr_param_end
            ; ++itr_param
        )
        {
            if (itr_param->getNameHash() == res.getParameterNameHash())
            {
                if (lerp)
                    itr_param->applyResource(res, t);

                else
                    itr_param->applyResource(res);

                is_apply = true;
                break;
            }
        }

        if (!is_apply)
        {
            if (p_list != nullptr)
            {
                p_list->callbackNotAppliable_(this, res);

                IParameterList* p_list_parent = p_list->mpParent;
                if (p_list_parent != nullptr)
                    p_list_parent->callbackNotAppliable_(this, res);
            }
        }
    }

    postRead_();
}

} }
