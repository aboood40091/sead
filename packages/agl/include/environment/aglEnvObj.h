#pragma once

#include <common/aglShaderEnum.h>
#include <math/seadMatrix.h>
#include <prim/seadBitFlag.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <utility/aglNamedObj.h>
#include <utility/aglNamedObjIndex.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace env {

class EnvObjMgr;

class EnvObj : public utl::IParameterObj, public utl::INamedObj
{
public:
    class Index : public utl::INamedObjIndex
    {
    public:
        Index()
            : utl::INamedObjIndex()
            , mEnvType(0)
        {
        }

        virtual ~Index()
        {
        }

        virtual const sead::SafeString& getNamedObjName(s32 index) const;
        virtual s32 getNamedObjNum() const;

        void setType(s32 type)
        {
            mEnvType = type;
        }

        s32 getType() const
        {
            return mEnvType;
        }

    protected:
        s32 mEnvType;
    };
    static_assert(sizeof(Index) == 0x54);

    typedef EnvObj* (*TypeCreateFuncPtr)(sead::Heap*);

    template <typename T>
    class TypeToID
    {
    public:
        static EnvObj* createInstance(sead::Heap* heap)
        {
            return new (heap) T;
        }
    };

    enum MetaInfo
    {
        cMetaInfo_0 = 0,    // Red icon
        cMetaInfo_1,        // Orange icon
        cMetaInfo_2,        // Yellow Icon
        cMetaInfo_3,        // Purple Icon
        cMetaInfo_Num
    };
    static_assert(sizeof(MetaInfo) == 4);
    static_assert(cMetaInfo_Num == 4);

private:
    struct TypeInfo
    {
        const char* name;
        const char* label;
        TypeCreateFuncPtr p_create_func;
        MetaInfo meta_info;
        s32 index;
    };
    static_assert(sizeof(TypeInfo) == 0x14);

public:
    static const s32 cTypeMax = 64;
    static s32 getTypeNum() { return sTypeNum; }

    static sead::SafeString getTypeName(s32 type) { return sTypeInfoTable[type].name; }
    static TypeCreateFuncPtr getTypeCreateFunc(s32 type) { return sTypeInfoTable[type].p_create_func; }

public:
    EnvObj();

    virtual void postRead_() { callbackLoadData(); }

    SEAD_RTTI_BASE(EnvObj)

public:
    virtual ~EnvObj();

    virtual void initialize(s32 view_max, sead::Heap* heap) { }
    virtual void update() { }
    virtual void updateView(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index) { }
    virtual ShaderMode drawDebug(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index, ShaderMode mode) const { return mode; }
    virtual void callbackLoadData() { }
    virtual const sead::SafeString& getEnvObjName() const { return *mName; }
    virtual const sead::SafeString& getGroupName() const { return *mGroup; }
    virtual const sead::SafeString& getObjName() const { return getEnvObjName(); }
    virtual bool isHostIOEnabled() const { return isEnableHostIO(); }
    virtual s32 getObjType() const { return getTypeID(); }
    virtual s32 getTypeID() const = 0;

    s32 getIndex() const
    {
        return mIndex;
    }

    bool isEnableHostIO() const
    {
        return mFlag.isOnBit(0);
    }

    void setName(const sead::SafeString& name)
    {
        *mName = name;
    }

    bool isEnable() const { return *mEnable; }
    void setEnable(bool enable);

    ShaderMode drawFog(s32 view_index, f32 start, f32 end, const sead::Vector3f& direction, const sead::Color4f& color, ShaderMode mode) const;

private:
    void becomeDefaultName_();

public:
    static s32* registClass(const sead::SafeString& name, const sead::SafeString& label, TypeCreateFuncPtr p_create_func, MetaInfo meta_info);

private:
    void initialize_(s32 index, s32 view_max, EnvObjMgr* p_env_mgr, sead::Heap* heap);
    void clear_();
    void update_();
    friend class EnvObjMgr;

protected:
    EnvObjMgr* mpMgr;
    u16 mIndex;
    sead::BitFlag16 mFlag;
    utl::Parameter< sead::FixedSafeString<32> > mName;
    utl::Parameter< sead::FixedSafeString<32> > mGroup;
    utl::Parameter<bool> mEnable;

    static s32 sTypeNum;
    static TypeInfo sTypeInfoTable[cTypeMax];
};
static_assert(sizeof(EnvObj) == 0x108);

} }

#define AGL_ENV_OBJ_TYPE_INFO()                                 \
    private:                                                    \
        static s32* sTypeInfo;                                  \
                                                                \
    public:                                                     \
        static s32 getType() { return *sTypeInfo; }             \
        virtual s32 getTypeID() const { return *sTypeInfo; }

#define AGL_ENV_OBJ_REGIST_CLASS(CLASS, LABEL, META) \
    s32* CLASS::sTypeInfo = ::agl::env::EnvObj::registClass(#CLASS, LABEL, ::agl::env::EnvObj::TypeToID<CLASS>::createInstance, ::agl::env::EnvObj::cMetaInfo_##META)
