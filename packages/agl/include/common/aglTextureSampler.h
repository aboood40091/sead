#pragma once

#include <common/aglShaderLocation.h>
#include <common/aglTextureData.h>
#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>
#include <prim/seadBitFlag.h>

namespace agl {

class TextureSampler
{
public:
    TextureSampler();
    TextureSampler(const TextureData& texture_data);
    virtual ~TextureSampler();

    const TextureData& getTextureData() const { return mTextureData; }
    void applyTextureData(const TextureData& texture_data);

    void setTextureCompSel(
        TextureCompSel r,
        TextureCompSel g,
        TextureCompSel b,
        TextureCompSel a
    );
    void resetTextureCompSel();

    void setWrapX(TextureWrapType wrap_x);
    void setWrapY(TextureWrapType wrap_y);
    void setWrapZ(TextureWrapType wrap_z);

    void setWrap(
        TextureWrapType wrap_x,
        TextureWrapType wrap_y,
        TextureWrapType wrap_z
    );

    void setFilterMag(TextureFilterType filter_mag);
    void setFilterMin(TextureFilterType filter_min);
    void setFilterMip(TextureMipFilterType filter_mip);

    void setFilter(
        TextureFilterType    filter_mag,
        TextureFilterType    filter_min,
        TextureMipFilterType filter_mip
    );

    void setMaxAnisoRatio(TextureAnisoRatio max_aniso);

    void setMipParam(f32 lod_min, f32 lod_max, f32 lod_bias);

    void setDepthCompareEnable(bool enable);
    void setDepthCompareFunc(sead::Graphics::DepthFunc func);

    void setBorderColor(f32 r, f32 g, f32 b, f32 a);
    void setBorderColor(const sead::Color4f& color);

    void setUnk1(u8 v) { _e9 = v; }
    u8 getUnk1() const { return _e9; }

    bool activate(const SamplerLocation& location, s32 = -1) const;

private:
    void applyTextureData_(const TextureData& texture_data);

    void initRegs_() const;

    void initTexture_() const;
    void initWrap_() const;
    void initFilter_() const;
    void initMipParam_() const;
    void initDepthComp_() const;
    void initBorderColor_() const;

private:
    TextureData mTextureData;
    TextureFilterType mFilterMag;
    TextureFilterType mFilterMin;
    TextureMipFilterType mFilterMip;
    TextureWrapType mWrapX;
    TextureWrapType mWrapY;
    TextureWrapType mWrapZ;
    sead::Color4f mBorderColor;
    f32 mLODMin;
    f32 mLODMax;
    f32 mLODBias;
    TextureAnisoRatio mMaxAniso;
    bool mReplaceCompSel;
    TextureCompSel mCompR;
    TextureCompSel mCompG;
    TextureCompSel mCompB;
    TextureCompSel mCompA;
    bool mIsTextureSet;
    u8 _e9;
    bool mDepthCompareEnable;
    sead::Graphics::DepthFunc mDepthCompareFunc;
    mutable GX2Sampler mGX2Sampler;
    mutable GX2Texture mGX2Texture;
    mutable bool mHasBorder;
    mutable sead::BitFlag8 mFlag;
};
static_assert(sizeof(TextureSampler) == 0x1A0, "agl::TextureSampler size mismatch");

}

#ifdef __cplusplus

#include <common/aglTextureSampler.hpp>

#endif // __cplusplus
