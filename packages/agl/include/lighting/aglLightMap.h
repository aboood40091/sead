#pragma once

#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>
#include <common/aglTextureSampler.h>
#include <common/aglUniformBlock.h>
#include <environment/aglEnvObjSet.h>

namespace agl { namespace lght {

class LightMapMgr;

class LightMap : public env::EnvObjSet
{
    struct MipRenderBuffer
    {
        RenderBuffer mRenderBuffer;
        RenderTargetColor mColorTarget[6];
    };
    static_assert(sizeof(MipRenderBuffer) == 0x848);

    struct ViewInfo
    {
        UniformBlock mUniformBlock;
        MipRenderBuffer mMipRenderBuffer[3];
    };
    static_assert(sizeof(ViewInfo) == 0x18EC);

    class LightObj
    {
    public:
        LightObj();

    private:
        u8 mCalcType;
        u8 mIndex; // Maybe
        sead::BitFlag8 mFlag;
        sead::Color4f mLightColor[2];
        sead::Color4f mLightColorWithEffect[2];
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
        u8 mIndex;
        utl::Parameter<bool> mEnableMip0;
        utl::Parameter<bool> mEnableMip1;
    };
    static_assert(sizeof(EnvObjEx) == 0xB8);

public:
    LightMap();
    virtual ~LightMap();

protected:
    virtual bool preRead_();
    virtual void postRead_();

public:
    virtual void genMessageEachObj(sead::hostio::Context*, s32, const env::EnvObj*);

protected:
    virtual bool preWrite_() const;
    virtual void callbackNotAppliable_(utl::IParameterObj*, utl::ResParameter);

public:
    TextureSampler& getTextureSampler() { return mTextureSampler; }
    const TextureSampler& getTextureSampler() const { return mTextureSampler; }

    void setLightingHint(s32 value)
    {
        *mLightingHint = value;
        mFlag.setBit(2);
    }

private:
    LightMapMgr* mpLightMapMgr;
    LightMap* mpParent;
    sead::Buffer<ViewInfo> mViewInfo;
    TextureSampler mTextureSampler;
    TextureData mTextureData;
    sead::BitFlag8 mFlag;
    s8 mChildIndex;
    s8 mCurrentView;
    s8 mParentIndex;
    s8 mCopyIndex;
    LightObj mLightObj[8];
    sead::Vector3f mDirection;
    sead::Color4f mLightColor;
    sead::Buffer<EnvObjEx> mEnvObjEx;
    UniformBlock mUniformBlock;
    sead::Buffer<UniformBlock> mMipUniformBlock;
    sead::PtrArray<LightMap> mChild;
    u32 _73c[4 / sizeof(u32)];
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
