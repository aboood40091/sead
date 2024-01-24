#include <detail/aglRootNode.h>
#include <environment/aglEnvObj.h>
#include <environment/aglEnvObjMgr.h>

#include <algorithm>

namespace {

inline u8 getBoolParamValue(const agl::utl::Parameter<bool>& param)
{
    BOOL value = FALSE;
    if (*param)
        value = TRUE;

    return value;
}

}

namespace agl { namespace env {

s32 EnvObj::sTypeNum = 0;
EnvObj::TypeInfo EnvObj::sTypeInfoTable[EnvObj::cTypeMax];

EnvObj::EnvObj()
    : mpMgr(nullptr)
    , mIndex(0)
    , mFlag(0x0001)
    , mName(sead::FixedSafeString<32>("env"), "name", "オブジェクト名", "AcceptReturn = False", this)
    , mGroup(sead::FixedSafeString<32>(utl::INamedObj::getDefaultGroupName()), "group", "グループ名", "AcceptReturn = False", this)
    , mEnable(false, "enable", "有効", this)
{
}

EnvObj::~EnvObj()
{
}

void EnvObj::setEnable(bool enable)
{
    if (getBoolParamValue(mEnable) == enable)
        return;

    *mEnable = enable;
    mpMgr->mFlag.setBit(0);
}

ShaderMode EnvObj::drawFog(s32 view_index, f32 start, f32 end, const sead::Vector3f& direction, const sead::Color4f& color, ShaderMode mode) const
{
    return mpMgr->drawFog_(view_index, *this, start, end, direction, color, mode);
}

void EnvObj::becomeDefaultName_()
{
    mName->format("%s%d", getTypeName(getTypeID()).cstr(), s32(mIndex));
}

s32* EnvObj::registClass(const sead::SafeString& name, const sead::SafeString& label, TypeCreateFuncPtr p_create_func, MetaInfo meta_info)
{
    static s32 s_type_num = 0;
    static s32 s_type_id_table[cTypeMax];

    s32 index = s_type_num;
    sTypeNum = ++s_type_num;

    sTypeInfoTable[index].name = name.cstr();
    sTypeInfoTable[index].label = label.cstr();
    sTypeInfoTable[index].p_create_func = p_create_func;
    sTypeInfoTable[index].meta_info = meta_info;
    sTypeInfoTable[index].index = index;

    s32* p_type_id = &(s_type_id_table[index]);
    *p_type_id = index;

    for (; index > 0; index--)
    {
        bool swap = true;

        if (sTypeInfoTable[index - 1].meta_info == sTypeInfoTable[index].meta_info)
        {
            if (sead::SafeString(sTypeInfoTable[index - 1].name).compare(sTypeInfoTable[index].name) <= 0)
                swap = false;
        }
        else if (sTypeInfoTable[index - 1].meta_info < sTypeInfoTable[index].meta_info)
        {
            swap = false;
        }

        if (!swap)
            break;

        s_type_id_table[sTypeInfoTable[index].index]--;
        s_type_id_table[sTypeInfoTable[index - 1].index]++;

        std::swap(sTypeInfoTable[index - 1].name, sTypeInfoTable[index].name);
        std::swap(sTypeInfoTable[index - 1].label, sTypeInfoTable[index].label);
        std::swap(sTypeInfoTable[index - 1].p_create_func, sTypeInfoTable[index].p_create_func);
        std::swap(sTypeInfoTable[index - 1].meta_info, sTypeInfoTable[index].meta_info);
        std::swap(sTypeInfoTable[index - 1].index, sTypeInfoTable[index].index);
    }

    return p_type_id;
}

void EnvObj::initialize_(s32 index, s32 view_max, EnvObjMgr* p_env_mgr, sead::Heap* heap)
{
  //SEAD_ASSERT(p_env_mgr != nullptr);
    mpMgr = p_env_mgr;
    mIndex = index;
    getEnvObjName();
    {
        static const char* s_icon[cMetaInfo_Num] = {
            "CIRCLE_RED",
            "CIRCLE_ORENGE",
            "CIRCLE_YELLOW",
            "CIRCLE_PURPLE"
        };
        detail::RootNode::setNodeMeta(
            this,
            sead::FormatFixedSafeString<1024>(
                "Icon = %s",
                s_icon[sTypeInfoTable[getTypeID()].meta_info]
            )
        );
    }
    initialize(view_max, heap);
    clear_();
}

void EnvObj::clear_()
{
    becomeDefaultName_();
    *mGroup = getDefaultGroupName();
    setEnable(false);
}

void EnvObj::update_()
{
    update();
    mFlag.resetBit(5);
}

const sead::SafeString& EnvObj::Index::getNamedObjName(s32 index) const
{
    return mpObjMgr->getNamedObjName(index, mEnvType);
}

s32 EnvObj::Index::getNamedObjNum() const
{
    return mpObjMgr->getNamedObjNum(mEnvType);
}

} }
