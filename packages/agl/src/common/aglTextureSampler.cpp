#include <common/aglTextureSampler.h>

namespace agl {

TextureSampler::TextureSampler()
    : mTextureData()
    , mFilterMag(cTextureFilterType_Linear)
    , mFilterMin(cTextureFilterType_Linear)
    , mFilterMip(cTextureMipFilterType_Linear)
    , mWrapX(cTextureWrapType_Clamp)
    , mWrapY(cTextureWrapType_Clamp)
    , mWrapZ(cTextureWrapType_Clamp)
    , mBorderColor(0.0f, 0.0f, 0.0f, 1.0f)
    , mLODMin(0.0f)
    , mLODMax(14.0f)
    , mLODBias(0.0f)
    , mMaxAniso(cTextureAnisoRatio_1_to_1)
    , mReplaceCompSel(false)
    , mCompR(cTextureCompSel_R)
    , mCompG(cTextureCompSel_G)
    , mCompB(cTextureCompSel_B)
    , mCompA(cTextureCompSel_A)
    , mIsTextureSet(false)
    , _e9(0)
    , mDepthCompareEnable(false)
    , mDepthCompareFunc(sead::Graphics::cDepthFunc_Never)
{
    GX2InitSampler(&mGX2Sampler,
                   GX2_TEX_CLAMP_CLAMP,
                   GX2_TEX_XY_FILTER_BILINEAR);

    mFlag.makeAllOne();
}

TextureSampler::~TextureSampler()
{
}

void TextureSampler::applyTextureData_(const TextureData& texture_data)
{
    mTextureData = texture_data;
    mIsTextureSet = true;
    sead::MemUtil::copy(&mGX2Texture.surface, &mTextureData.getSurface(), sizeof(GX2Surface));
    mGX2Texture.viewFirstMip = 0;
    mGX2Texture.viewNumMips = mGX2Texture.surface.numMips;
    mGX2Texture.viewFirstSlice = 0;
    mGX2Texture.viewNumSlices = mGX2Texture.surface.depth;

    mFlag.set(1 << 0 |
              1 << 2);
}

void TextureSampler::initRegs_() const
{
    if (mFlag.isOnBit(0))
        initTexture_();

    if (mFlag.isOnBit(1))
        initWrap_();

    if (mFlag.isOnBit(2))
        initFilter_();

    if (mFlag.isOnBit(3))
        initMipParam_();

    if (mFlag.isOnBit(4))
        initDepthComp_();

    if (mFlag.isOnBit(5))
        initBorderColor_();

    mFlag.makeAllZero();
}

void TextureSampler::initTexture_() const
{
    if (mReplaceCompSel)
    {
        mGX2Texture.compSel = GX2_GET_COMP_SEL(mCompR,
                                               mCompG,
                                               mCompB,
                                               mCompA);
    }
    else if (mIsTextureSet)
    {
        mGX2Texture.compSel = GX2_GET_COMP_SEL(mTextureData.getComponentR(),
                                               mTextureData.getComponentG(),
                                               mTextureData.getComponentB(),
                                               mTextureData.getComponentA());
    }

    if (mIsTextureSet)
        GX2InitTextureRegs(&mGX2Texture);
}

void TextureSampler::initWrap_() const
{
    GX2InitSamplerClamping(&mGX2Sampler,
                           (GX2TexClamp)mWrapX,
                           (GX2TexClamp)mWrapY,
                           (GX2TexClamp)mWrapZ);
}

void TextureSampler::initFilter_() const
{
    GX2InitSamplerXYFilter(&mGX2Sampler,
                           (GX2TexXYFilterType)mFilterMag,
                           (GX2TexXYFilterType)mFilterMin,
                           (GX2TexAnisoRatio)mMaxAniso);

    GX2TexZFilterType z_filter = (mIsTextureSet && mTextureData.getTextureType() == GX2_SURFACE_DIM_2D_ARRAY)
                                    ? GX2_TEX_Z_FILTER_POINT
                                    : GX2_TEX_Z_FILTER_USE_XY;

    GX2InitSamplerZMFilter(&mGX2Sampler,
                           z_filter,
                           (GX2TexMipFilterType)mFilterMip);
}

void TextureSampler::initMipParam_() const
{
    GX2InitSamplerLOD(&mGX2Sampler, mLODMin, mLODMax, mLODBias);
}

void TextureSampler::initDepthComp_() const
{
    GX2InitSamplerDepthCompare(&mGX2Sampler, (GX2CompareFunction)mDepthCompareFunc);
}

void TextureSampler::initBorderColor_() const
{
    if (mBorderColor == sead::Color4f::cBlack)
    {
        GX2InitSamplerBorderType(&mGX2Sampler, GX2_TEX_BORDER_SOLID_BLACK);
        mHasBorder = false;
    }
    else if (mBorderColor == sead::Color4f::cWhite)
    {
        GX2InitSamplerBorderType(&mGX2Sampler, GX2_TEX_BORDER_SOLID_WHITE);
        mHasBorder = false;
    }
    else if (mBorderColor == sead::Color4f(0.0f, 0.0f, 0.0f, 0.0f))
    {
        GX2InitSamplerBorderType(&mGX2Sampler, GX2_TEX_BORDER_CLEAR_BLACK);
        mHasBorder = false;
    }
    else
    {
        GX2InitSamplerBorderType(&mGX2Sampler, GX2_TEX_BORDER_USE_REGISTER);
        mHasBorder = true;
    }
}

bool TextureSampler::activate(const SamplerLocation& location, s32) const
{
    if (!mFlag.isZero())
        initRegs_();

    if (!location.isValid())
        return false;

    if (location.getVertexLocation() == -1)
    {
        if (mHasBorder)
            GX2SetVertexSamplerBorderColor(location.getVertexLocation(), mBorderColor.r,
                                                                         mBorderColor.g,
                                                                         mBorderColor.b,
                                                                         mBorderColor.a);

        GX2SetVertexTexture(&mGX2Texture, location.getVertexLocation());
        GX2SetVertexSampler(&mGX2Sampler, location.getVertexLocation());
    }

    if (location.getFragmentLocation() != -1)
    {
        if (mHasBorder)
            GX2SetPixelSamplerBorderColor(location.getFragmentLocation(), mBorderColor.r,
                                                                          mBorderColor.g,
                                                                          mBorderColor.b,
                                                                          mBorderColor.a);

        GX2SetPixelTexture(&mGX2Texture, location.getFragmentLocation());
        GX2SetPixelSampler(&mGX2Sampler, location.getFragmentLocation());
    }

    if (location.getGeometryLocation() != -1)
    {
        if (mHasBorder)
            GX2SetGeometrySamplerBorderColor(location.getGeometryLocation(), mBorderColor.r,
                                                                             mBorderColor.g,
                                                                             mBorderColor.b,
                                                                             mBorderColor.a);

        GX2SetGeometryTexture(&mGX2Texture, location.getGeometryLocation());
        GX2SetGeometrySampler(&mGX2Sampler, location.getGeometryLocation());
    }

    return true;
}

}
