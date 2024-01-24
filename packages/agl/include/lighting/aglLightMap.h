#pragma once

#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>
#include <common/aglTextureSampler.h>
#include <common/aglUniformBlock.h>
#include <environment/aglEnvObjSet.h>
#include <gfx/seadGraphicsContextMRT.h>

namespace agl { namespace lght {

class LightMapMgr;

class LightMap : public env::EnvObjSet
{

public:
    static const s32 cLightMapMax = 256;
    static const s32 cLightObjMax = 8;
    static const s32 cMipMax = 3;
    static const s32 cBoolMax = 2;
    static const s32 cLightingHintMax = 3;
    
    enum MappingType
    {
        cMappingType_Sphere = 0,
        cMappingType_SphereBackside,
        cMappingType_Cube,
        cMappingType_Num
    };
    static_assert(cMappingType_Num == 3);

    enum SliceNum
    {
        cSliceNum_Sphere            = 1,
        cSliceNum_SphereBackside    = 2,
        cSliceNum_Cube              = 6,    // A cube map has 6 slices

        cSliceNum_Max               = 6     // max(cSliceNum_Sphere, cSliceNum_SphereBackside, cSliceNum_Cube)
    };

    enum UniformBlockID
    {
        cUniformBlockID_Light = 0,
        cUniformBlockID_LightView,
        cUniformBlockID_LightMip,
        cUniformBlockID_Num
    };
    static_assert(cUniformBlockID_Num == 3);

    enum Sampler
    {
        cSampler_LUT = 0,
        cSampler_Normal,
        cSampler_Num
    };
    static_assert(cSampler_Num == 2);

    enum LightMemberIndex
    {
        cLightMemberIndex_RimPow = 0,       // float cRimPow;
        cLightMemberIndex_RimWidth,         // float cRimWidth;
        cLightMemberIndex_ParamLight,       // float cParamLight[cLightObjMax];
        cLightMemberNum
    };
    static_assert(cLightMemberNum == 3);

    enum LightViewMemberIndex
    {
        cLightViewMemberIndex_RimColor = 0, // vec4 cRimColor;
        cLightViewMemberIndex_LightDir,     // vec4 cLightDir[cLightObjMax];
        cLightViewMemberNum
    };
    static_assert(cLightViewMemberNum == 2);

    enum LightMipMemberIndex
    {
        cLightMipMemberIndex_SpecPow = 0,   // float cSpecPow[cLightObjMax];
        cLightMipMemberIndex_ParamLightMip, // vec4 cParamLightMip[cLightObjMax * 2];
        cLightMipMemberNum
    };
    static_assert(cLightMipMemberNum == 2);

private:
    struct Context
    {
        RenderBuffer mRenderBuffer;
        sead::SafeArray<RenderTargetColor, cSliceNum_Max> mColorTarget;
    };
    static_assert(sizeof(Context) == 0x848);

    struct ViewInfo
    {
        UniformBlock mUniformBlock;
        Context mContext[cMipMax];
    };
    static_assert(sizeof(ViewInfo) == 0x18EC);

    class LightObj
    {
    public:
        LightObj();

    private:
        u8 mCalcType;
        s8 mLUTType;
        sead::BitFlag8 mFlag;
        sead::Color4f mLightColorMinMax[2];
        sead::Color4f mLightColorMinMaxWithEffect[2];
        sead::Vector3f mDirection;
        f32 mPow;
        f32 mPowMipMax;

        friend class LightMap;
    };
    static_assert(sizeof(LightObj) == 0x58);

    struct EnvObjEx
    {
        utl::Parameter<s32> mCalcType;
        utl::Parameter<f32> mEffect;
        utl::Parameter<f32> mPow;
        utl::Parameter<f32> mPowMipMax;
        utl::Parameter< sead::FixedSafeString<32> > mLUTName;
        s8 mLUTType;
        utl::Parameter<bool> mEnableMip0;
        utl::Parameter<bool> mEnableMip1;
    };
    static_assert(sizeof(EnvObjEx) == 0xB8);

    typedef sead::UnsafeArray<
        sead::UnsafeArray<
            sead::GraphicsContextMRT,
            cBoolMax
        >,
        cMappingType_Num
    > GraphicsContext;
    static_assert(sizeof(GraphicsContext) == 0x6A8);

    struct DrawArg
    {
        const GraphicsContext* mpGraphicsContext;
        sead::UnsafeArray<const TextureSampler*, cMappingType_Num> mNormalMap;
        const TextureSampler* mpLUT;
    };
    static_assert(sizeof(DrawArg) == 0x14);

    struct ArrayNode : public sead::hostio::Node
    {
    };

public:
    LightMap();
    virtual ~LightMap();

private:
    void initialize_(const LightMapMgr* p_mgr, const env::EnvObjBuffer::AllocateArg& alloc_arg, s32 view_max, sead::Heap* heap);
    void update_();
    void updateGPU_(bool invalidate_gpu) const;
    void updateViewGPU_(s32 view_index, const sead::Matrix34f& view_mtx, bool invalidate_gpu) const;
    ShaderMode draw_(s32 view_index, const DrawArg& arg, ShaderMode mode) const;

    void bindLUT_(s32 obj_index);
    void bindLUT_();

    void initializeRenderBuffer_(s32 view_index, u32 mip_level);
    void initializeTexture_(u32 size_sphere, u32 size_cube);
    void constructMemory_(sead::Heap* heap);

protected:
    virtual bool preRead_();
    virtual void postRead_();

public:
    virtual void genMessageEachObj(sead::hostio::Context*, s32, const env::EnvObj*);

protected:
    virtual bool preWrite_() const;
    virtual void callbackNotAppliable_(utl::IParameterObj* p_param_obj, utl::ResParameter res_param);

public:
    s32 applyEnvObjData(const env::EnvObj* p_obj, const EnvObjEx* p_ex, s32 light_index);

public:
    TextureSampler& getTextureSampler() { return mTextureSampler; }
    const TextureSampler& getTextureSampler() const { return mTextureSampler; }

    void setCalcType(s32 light_index, s32 calc_type)
    {
        if (mLightObj[light_index].mCalcType == calc_type)
            return;

        mFlag.setBit(4);
        mLightObj[light_index].mCalcType = calc_type;
    }

    void setPow(s32 light_index, f32 pow)
    {
        if (mLightObj[light_index].mPow == pow)
            return;

        mFlag.setBit(4);
        mLightObj[light_index].mPow = pow;
    }

    void setPowMipMax(s32 light_index, f32 pow_mip_max)
    {
        if (mLightObj[light_index].mPowMipMax == pow_mip_max)
            return;

        mFlag.setBit(4);
        mLightObj[light_index].mPowMipMax = pow_mip_max;
    }

    void setColorMinMax(s32 light_index, const sead::Color4f& min, const sead::Color4f& max)
    {
        if (mLightObj[light_index].mLightColorMinMax[0] == min && mLightObj[light_index].mLightColorMinMax[1] == max)
            return;

        mFlag.setBit(4);
        mLightObj[light_index].mLightColorMinMax[0] = min;
        mLightObj[light_index].mLightColorMinMax[1] = max;
    }

    void setDirection(s32 light_index, const sead::Vector3f& direction)
    {
        if (mLightObj[light_index].mDirection == direction)
            return;

        mFlag.setBit(4);
        mLightObj[light_index].mDirection = direction;
    }

    void setLUTType(s32 light_index, s32 lut_type)
    {
        if (mLightObj[light_index].mLUTType == lut_type)
            return;

        mFlag.setBit(4);
        mLightObj[light_index].mLUTType = lut_type;
        mFlag.setBit(4); // Nintendo: gotta set it twice to make sure
    }

    MappingType getMappingType() const
    {
        if (mpParent == nullptr)
            return MappingType(*mMappingType);
        
        return mpParent->getMappingType();
    }

    s32 getLightingHint() const
    {
        if (mpParent == nullptr)
            return *mLightingHint;
        
        return mpParent->getLightingHint();
    }

    void setLightingHint(s32 value)
    {
        *mLightingHint = value;
        mFlag.setBit(2);
    }

    s32 getChildNum() const
    {
        return mChild.size();
    }

    LightMap* get(s32 array_index)
    {
      //SEAD_ASSERT(0 == array_index || array_index <= getChildNum());
        return array_index == 0 ? this : mChild[array_index - 1];
    }

    const LightMap* get(s32 array_index) const
    {
      //SEAD_ASSERT(0 == array_index || array_index <= getChildNum());
        return array_index == 0 ? this : mChild[array_index - 1];
    }

private:
    const LightMapMgr* mpLightMapMgr;
    LightMap* mpParent;
    sead::Buffer<ViewInfo> mViewInfo;
    TextureSampler mTextureSampler;
    TextureData mTextureData;
    sead::BitFlag8 mFlag;
    s8 mChildIndex; // -1 = Independent (neither a parent nor a child), 0 = Root (i.e. a parent), 1+: Index in parent's child list + 1
    u8 mLightObjNum;
    s8 mParentIndex;
    s8 mCopyIndex;
    sead::UnsafeArray<LightObj, cLightObjMax> mLightObj;
    sead::Vector3f mDirection;
    sead::Color4f mLightColor;
    sead::Buffer<EnvObjEx> mEnvObjEx;
    UniformBlock mUniformBlock;
    sead::Buffer<UniformBlock> mMipUniformBlock;
    sead::PtrArray<LightMap> mChild;
    ArrayNode mArrayNode;
    utl::Parameter<s32> mLightingHint;
    utl::Parameter<bool> mHdrEnable;
    utl::Parameter<bool> mRimEnable;
    utl::Parameter<f32> mRimEffect;
    utl::Parameter<f32> mRimWidth;
    utl::Parameter<f32> mRimPow;
    utl::Parameter<f32> mRimAngle;
    env::EnvObj::Index mRimLightRef;
    utl::Parameter<s32> mMappingType;
    utl::Parameter< sead::FixedSafeString<32> > mParentMap;
    utl::Parameter< sead::FixedSafeString<32> > mCopyMap;

    friend class LightMapMgr;
};
static_assert(sizeof(LightMap) == 0x8AC, "agl::lght::LightMap size mismatch");

} }
