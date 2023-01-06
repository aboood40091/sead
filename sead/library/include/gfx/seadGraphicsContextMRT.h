#ifndef SEAD_GRAPHICS_CONTEXT_MRT_H_
#define SEAD_GRAPHICS_CONTEXT_MRT_H_

#include <container/seadSafeArray.h>
#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>
#include <prim/seadBitFlag.h>

namespace sead {

class GraphicsContextMRT
{
    struct BlendExpression
    {
        BlendExpression()
            : blend_factor_src_rgb(Graphics::cBlendFactor_SrcAlpha)
            , blend_factor_src_a(Graphics::cBlendFactor_SrcAlpha)
            , blend_factor_dst_rgb(Graphics::cBlendFactor_InvSrcAlpha)
            , blend_factor_dst_a(Graphics::cBlendFactor_InvSrcAlpha)
            , blend_equation_rgb(Graphics::cBlendEquation_Add)
            , blend_equation_a(Graphics::cBlendEquation_Add)
        {
        }

        Graphics::BlendFactor blend_factor_src_rgb;
        Graphics::BlendFactor blend_factor_src_a;
        Graphics::BlendFactor blend_factor_dst_rgb;
        Graphics::BlendFactor blend_factor_dst_a;
        Graphics::BlendEquation blend_equation_rgb;
        Graphics::BlendEquation blend_equation_a;
    };

public:
    GraphicsContextMRT();
    virtual ~GraphicsContextMRT()
    {
    }

    void apply() const;
    void applyAlphaTest() const;
    void applyDepthAndStencilTest() const;
    void applyColorMask() const;
    void applyBlendAndFastZ() const;
    void applyBlendConstantColor() const;
    void applyCullingAndPolygonModeAndPolygonOffset() const;

//private:
    bool mDepthTestEnable;
    bool mDepthWriteEnable;
    Graphics::DepthFunc mDepthFunc;
    Graphics::CullingMode mCullingMode;
    BitFlag32 mBlendEnableMask;
    SafeArray<BlendExpression, Graphics::cRenderTarget_Num> mBlendExpression;
    Color4f mBlendConstantColor;
    bool mAlphaTestEnable;
    Graphics::AlphaFunc mAlphaTestFunc;
    f32 mAlphaTestRef;
    u32 mColorMask;
    bool mStencilTestEnable;
    Graphics::StencilFunc mStencilTestFunc;
    s32 mStencilTestRef;
    u32 mStencilTestMask;
    Graphics::StencilOp mStencilOpFail;
    Graphics::StencilOp mStencilOpZFail;
    Graphics::StencilOp mStencilOpZPass;
#ifdef cafe
    Graphics::PolygonMode mPolygonModeFront;
    Graphics::PolygonMode mPolygonModeBack;
    bool mPolygonOffsetFrontEnable;
    bool mPolygonOffsetBackEnable;
    bool mPolygonOffsetPointLineEnable;
#endif // cafe
};
#ifdef cafe
static_assert(sizeof(GraphicsContextMRT) == 0x11C, "sead::GraphicsContextMRT size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_GRAPHICS_CONTEXT_MRT_H_
