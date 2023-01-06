#pragma once

#include <common/aglIndexStream.h>
#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>
#include <common/aglShaderEnum.h>
#include <common/aglTextureSampler.h>
#include <common/aglVertexAttribute.h>
#include <common/aglVertexBuffer.h>
#include <container/seadBuffer.h>
#include <gfx/seadGraphicsContextMRT.h>
#include <hostio/seadHostIODummy.h>
#include <utility/aglDebugTexturePage.h>
#include <utility/aglParameter.h>
#include <utility/aglParameterIO.h>
#include <utility/aglParameterObj.h>

namespace agl {

class RenderBuffer;
class TextureData;

namespace pfx {

class DepthOfField : public utl::IParameterIO, public sead::hostio::Node
{
    struct Context
    {
        bool mIsInitialized;
        s32 mIndex;
        TextureSampler mColorTargetTextureSampler;
        TextureSampler mDepthTargetTextureSampler;
        RenderBuffer mRenderBuffer;
        RenderTargetColor mColorTarget;
        TextureData* mpColorTextureData; // for "blur_mipmap"
        TextureSampler mColorTextureSampler;
        TextureData* mpDepthTextureData; // for "depth_mipmap"
        TextureSampler mDepthTextureSampler;
    };
    static_assert(sizeof(Context) == 0x834, "agl::pfx::DepthOfField::Context size mismatch");

    struct DrawArg
    {
        DrawArg(Context& ctx, const RenderBuffer& render_buffer, const TextureData& depth, bool view_depth_, f32 near_, f32 far_);

        s32 pass;
        Context* p_ctx;
        const RenderBuffer* p_render_buffer;
        f32 near;
        f32 far;
        s32 width;
        s32 height;
        bool view_depth;
    };
    static_assert(sizeof(DrawArg) == 0x20, "agl::pfx::DepthOfField::DrawArg size mismatch");

    struct TempVignetting : utl::IParameterObj
    {
        TempVignetting(DepthOfField* p_dof, const sead::SafeString& param_name);

        utl::Parameter<s32> mType;
        utl::Parameter<sead::Vector2f> mRange;
        utl::Parameter<sead::Vector2f> mScale;
        utl::Parameter<sead::Vector2f> mTrans;
    };
    static_assert(sizeof(TempVignetting) == 0xCC, "agl::pfx::DepthOfField::TempVignetting size mismatch");

    struct Vertex
    {
        sead::Vector2f position;
        sead::Vector2f texcoord;
    };
    static_assert(sizeof(Vertex) == 0x10);

    struct VignettingShape
    {
        sead::Buffer<Vertex> mVertex;
        sead::Buffer<u16> mIndex;
        VertexBuffer mVertexBuffer;
        VertexAttribute mVertexAttribute;
        IndexStream mIndexStream;
    };
    static_assert(sizeof(VignettingShape) == 0x2F0, "agl::pfx::DepthOfField::VignettingShape size mismatch");

public:
    enum Uniform
    {
        cUniform_Param0 = 0,
        cUniform_IndirectTexParam,
        cUniform_IndirectTexMtx0,
        cUniform_IndirectTexMtx1,
        cUniform_MulParam,
        cUniform_AddParam,
        cUniform_NearFarParam,
        cUniform_TexParam,
        cUniform_VignettingRadius,
        cUniform_VignettingParam,
        cUniform_VignettingTrans,
        cUniform_VignettingColor,
        cUniform_FarMulColor,
        cUniform_Num
    };
    static_assert(cUniform_Num == 13);

    enum Sampler
    {
        cSampler_TexColor = 0,
        cSampler_TexDepth,
        cSampler_TexMipMap,
        cSampler_TexMipMapDepth,
        cSampler_TexIndirect,
        cSampler_Num
    };
    static_assert(cSampler_Num == 5);

    enum VignettingBlendType
    {
        cVignettingBlendType_Normal = 0,    // op = Add,    src = SrcAlpha,     dst = InvSrcAlpha
        cVignettingBlendType_Add,           // op = Add,    src = SrcAlpha,     dst = One
        cVignettingBlendType_Mult,          // op = Add,    src = Zero,         dst = SrcColor
        cVignettingBlendType_Screen         // op = Add,    src = InvDstColor,  dst = One
    };

public:
    DepthOfField();
    virtual ~DepthOfField();

    void initialize(s32 ctx_num = 1, sead::Heap* heap = nullptr);

private:
    void assignShaderProgram_();

    void initVertex_(sead::Heap* heap);
    void initIndex_(sead::Heap* heap);

public:
    bool isEnable() const
    {
        return *mEnable;
    }

    void setEnable(bool enable)
    {
        *mEnable = enable;
    }

    VignettingBlendType getVignettingBlendType() const { return VignettingBlendType(*mVignettingBlend); }

    ShaderMode draw(s32 ctx_index, const RenderBuffer& render_buffer, f32 near, f32 far, ShaderMode mode = cShaderMode_Invalid) const;
    ShaderMode draw(s32 ctx_index, const RenderBuffer& render_buffer, const TextureData& depth, bool view_depth, f32 near, f32 far, ShaderMode mode = cShaderMode_Invalid) const;

    void allocBuffer(s32 ctx_index, const RenderBuffer& render_buffer) const;
    void allocBuffer(s32 ctx_index, TextureFormat format, s32 width, s32 height) const;
    void freeBuffer(s32 ctx_index) const;

private:
    bool enableDepthOfField_() const;
    bool enableBlurMipMapPass_() const;
    bool enableDepthBlur_() const;
    bool enableDifferntShape_() const;
    bool enableSeparateVignettingPass_() const;

    void bindRenderBuffer_(RenderBuffer& render_buffer, s32 mip_level_color, s32 mip_level_depth) const;

    void drawKick_(const DrawArg& arg) const;

    ShaderMode drawColorMipMap_(const DrawArg& arg, ShaderMode mode) const;
    ShaderMode drawDepthMipMap_(const DrawArg& arg, ShaderMode mode) const;
    ShaderMode drawCompose_(const DrawArg& arg, ShaderMode mode) const;
    ShaderMode drawVignetting_(const DrawArg& arg, ShaderMode mode) const;

    void uniformComposeParam_(const DrawArg& arg, const ShaderProgram* program) const;
    void uniformVignettingParam_(const DrawArg& arg, const ShaderProgram* program) const;

    static s32 roundUp_(f32 value)
    {
        s32 ivalue = s32(value);
        if (ivalue != value)
            if (value >= 0.0f)
                ivalue++;

        return ivalue;
    }

    static s32 roundDown_(f32 value)
    {
        s32 ivalue = s32(value);
        if (ivalue != value)
            if (value < 0.0f)
                ivalue--;

        return ivalue;
    }

    sead::Vector4f getTexParam_(const TextureData& data, u32 mip_level = 0) const
    {
        sead::Vector4f param;
        param.x = 0.5f / data.getWidth(mip_level);
        param.y = 0.5f / data.getHeight(mip_level);
        param.z = 0.0f; // Unused
        param.w = 0.0f; // ^^
        return param;
    }

public:
    void setIndirectTextureData(const TextureData* p_texture_data);
    void setIndirectEnable(bool enable);

    void setIndirectTextureTrans(const sead::Vector2f& trans);

private:
    void applyIndirectTextureData_();
    void updateIndirectMatrix_();

    virtual void postRead_();

private:
    mutable sead::Buffer<Context> mContext;
    s32 _1e8;
    f32 _1ec;
    bool mEnableColorBlur;
    f32 _1f4;
    sead::GraphicsContextMRT mGraphicsContext;
    utl::IParameterObj mParameterObj;
    utl::Parameter<bool> mEnable;
    utl::Parameter<bool> mNearEnable;
    utl::Parameter<bool> mFarEnable;
    utl::Parameter<bool> mDepthBlur;
    utl::Parameter<bool> mEnableVignettingColor;
    utl::Parameter<bool> mEnableVignettingBlur;
    utl::Parameter<bool> mEnableVignetting2Shape;
    utl::Parameter<bool> mEnableColorControl;
    utl::Parameter<bool> mIndirectEnable;
    utl::Parameter<f32> mStart;
    utl::Parameter<f32> mEnd;
    utl::Parameter<f32> mFarStart;
    utl::Parameter<f32> mFarEnd;
    utl::Parameter<f32> mLevel;
    utl::Parameter<f32> mDepthBlurAdd;
    utl::Parameter<f32> mSaturateMin;
    utl::Parameter<sead::Vector4f> mColorCtrlDepth;
    utl::Parameter<sead::Vector2f> mIndirectTexTrans;
    utl::Parameter<sead::Vector2f> mIndirectTexScale;
    utl::Parameter<f32> mIndirectTexRotate;
    utl::Parameter<f32> mIndirectScale;
    utl::Parameter<f32> mVignettingBlur;
    utl::Parameter<s32> mVignettingBlend; // -> enum VignettingBlendType
    utl::Parameter<sead::Color4f> mVignettingColor;
    TempVignetting mTempVignetting0;
    TempVignetting mTempVignetting1;
    utl::Parameter<sead::Color4f> mFarMulColor;
    utl::DebugTexturePage mDebugTexturePage;
    sead::SafeArray<ShaderProgram*, 2> mpCurrentProgramNearMask;
    sead::SafeArray<ShaderProgram*, 2> mpCurrentProgramMipMap;
    sead::SafeArray<const ShaderProgram*, 2> mpCurrentProgramDepthMask;
    sead::SafeArray<ShaderProgram*, 2> mpCurrentProgramFinal;
    ShaderProgram* mpCurrentProgramVignetting;
    const TextureData* mpIndirectTextureData;
    TextureSampler mIndirectTextureSampler;
    sead::Vector4f mIndirectTexParam;
    sead::Vector3f mIndirectTexMtx0;
    sead::Vector3f mIndirectTexMtx1;
    sead::SafeArray<VignettingShape, 2> mVignettingShape;
    u8 _10e8;
};
static_assert(sizeof(DepthOfField) == 0x10EC, "agl::pfx::DepthOfField size mismatch");

} }
