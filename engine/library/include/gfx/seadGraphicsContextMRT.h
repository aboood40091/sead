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

    void setDepthEnable(bool test_enable, bool write_enable)
    {
        setDepthTestEnable(test_enable);
        setDepthWriteEnable(write_enable);
    }

    void setDepthTestEnable(bool test_enable)
    {
        mDepthTestEnable = test_enable;
    }

    void setDepthWriteEnable(bool write_enable)
    {
        mDepthWriteEnable = write_enable;
    }

    void setDepthFunc(Graphics::DepthFunc func)
    {
        mDepthFunc = func;
    }

    void setCullingMode(Graphics::CullingMode mode)
    {
        mCullingMode = mode;
    }

    void setBlendEnable(u32 target, bool blend)
    {
        mBlendEnableMask.changeBit(target, blend);
    }

    void setBlendEnableMask(u32 mask)
    {
        mBlendEnableMask.setDirect(mask);
    }

    void setBlendFactor(u32 target, Graphics::BlendFactor src_factor, Graphics::BlendFactor dst_factor)
    {
        setBlendFactorSrc(target, src_factor);
        setBlendFactorDst(target, dst_factor);
    }

    void setBlendFactorSeparate(u32 target, Graphics::BlendFactor src_factor_rgb, Graphics::BlendFactor dst_factor_rgb, Graphics::BlendFactor src_factor_a, Graphics::BlendFactor dst_factor_a)
    {
        setBlendFactorSrcRGB(target, src_factor_rgb);
        setBlendFactorDstRGB(target, dst_factor_rgb);
        setBlendFactorSrcAlpha(target, src_factor_a);
        setBlendFactorDstAlpha(target, dst_factor_a);
    }

    void setBlendFactorSrc(u32 target, Graphics::BlendFactor factor)
    {
        setBlendFactorSrcRGB(target, factor);
        setBlendFactorSrcAlpha(target, factor);
    }

    void setBlendFactorDst(u32 target, Graphics::BlendFactor factor)
    {
        setBlendFactorDstRGB(target, factor);
        setBlendFactorDstAlpha(target, factor);
    }

    void setBlendFactorSrcRGB(u32 target, Graphics::BlendFactor factor)
    {
        mBlendExpression[target].blend_factor_src_rgb = factor;
    }

    void setBlendFactorSrcAlpha(u32 target, Graphics::BlendFactor factor)
    {
        mBlendExpression[target].blend_factor_src_a = factor;
    }

    void setBlendFactorDstRGB(u32 target, Graphics::BlendFactor factor)
    {
        mBlendExpression[target].blend_factor_dst_rgb = factor;
    }

    void setBlendFactorDstAlpha(u32 target, Graphics::BlendFactor factor)
    {
        mBlendExpression[target].blend_factor_dst_a = factor;
    }

    void setBlendEquation(u32 target, Graphics::BlendEquation equation)
    {
        setBlendEquationRGB(target, equation);
        setBlendEquationAlpha(target, equation);
    }

    void setBlendEquationSeparate(u32 target, Graphics::BlendEquation equation_rgb, Graphics::BlendEquation equation_a)
    {
        setBlendEquationRGB(target, equation_rgb);
        setBlendEquationAlpha(target, equation_a);
    }

    void setBlendEquationRGB(u32 target, Graphics::BlendEquation equation)
    {
        mBlendExpression[target].blend_equation_rgb = equation;
    }

    void setBlendEquationAlpha(u32 target, Graphics::BlendEquation equation)
    {
        mBlendExpression[target].blend_equation_a = equation;
    }

    void setBlendConstantColor(const Color4f& color)
    {
        mBlendConstantColor = color;
    }

    void setAlphaTestEnable(bool enable)
    {
        mAlphaTestEnable = enable;
    }

    void setAlphaTestFunc(Graphics::AlphaFunc func, f32 ref)
    {
        mAlphaTestFunc = func;
        mAlphaTestRef = ref;
    }

    void setColorMask(u32 target, bool r, bool g, bool b, bool a)
    {
        u32 color_mask = r << 0 |
                         g << 1 |
                         b << 2 |
                         a << 3;
        mColorMask = (mColorMask & ~(0xF << (target * 4))) | (color_mask << (target * 4));
    }

    void setColorMask(u32 mask)
    {
        mColorMask = mask;
    }

    void setStencilTestEnable(bool enable)
    {
        mStencilTestEnable = enable;
    }

    void setStencilTestFunc(Graphics::StencilFunc func, s32 ref, u32 mask)
    {
        mStencilTestFunc = func;
        mStencilTestRef = ref;
        mStencilTestMask = mask;
    }

    void setStencilTestOp(Graphics::StencilOp fail, Graphics::StencilOp zfail, Graphics::StencilOp zpass)
    {
        mStencilOpFail = fail;
        mStencilOpZFail = zfail;
        mStencilOpZPass = zpass;
    }

    void setPolygonMode(Graphics::PolygonMode front, Graphics::PolygonMode back)
    {
#ifdef cafe
        mPolygonModeFront = front;
        mPolygonModeBack = back;
#endif // cafe
    }

    void setPolygonOffsetEnable(bool fill_front_enable, bool fill_back_enable, bool point_line_enable)
    {
#ifdef cafe
        mPolygonOffsetFrontEnable = fill_front_enable;
        mPolygonOffsetBackEnable = fill_back_enable;
        mPolygonOffsetPointLineEnable = point_line_enable;
#endif // cafe
    }

    bool getDepthTestEnable() const
    {
        return mDepthTestEnable;
    }

    bool getDepthWriteEnable() const
    {
        return mDepthWriteEnable;
    }

    Graphics::DepthFunc getDepthFunc() const
    {
        return mDepthFunc;
    }

    Graphics::CullingMode getCullingMode() const
    {
        return mCullingMode;
    }

    bool getBlendEnable(u32 target) const
    {
        return mBlendEnableMask.isOnBit(target);
    }

    u32 getBlendEnableMask() const
    {
        return mBlendEnableMask.getDirect();
    }

    Graphics::BlendFactor getBlendFactorSrcRGB(u32 target) const
    {
        return mBlendExpression[target].blend_factor_src_rgb;
    }

    Graphics::BlendFactor getBlendFactorSrcAlpha(u32 target) const
    {
        return mBlendExpression[target].blend_factor_src_a;
    }

    Graphics::BlendFactor getBlendFactorDstRGB(u32 target) const
    {
        return mBlendExpression[target].blend_factor_dst_rgb;
    }

    Graphics::BlendFactor getBlendFactorDstAlpha(u32 target) const
    {
        return mBlendExpression[target].blend_factor_dst_a;
    }

    Graphics::BlendEquation getBlendEquationRGB(u32 target) const
    {
        return mBlendExpression[target].blend_equation_rgb;
    }

    Graphics::BlendEquation getBlendEquationAlpha(u32 target) const
    {
        return mBlendExpression[target].blend_equation_a;
    }

    const Color4f& getBlendConstantColor() const
    {
        return mBlendConstantColor;
    }

    bool getAlphaTestEnable() const
    {
        return mAlphaTestEnable;
    }

    Graphics::AlphaFunc getAlphaTestFunc() const
    {
        return mAlphaTestFunc;
    }

    f32 getAlphaTestRef() const
    {
        return mAlphaTestRef;
    }

    bool getColorMaskR(u32 target) const
    {
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 0 & 1;
    }

    bool getColorMaskG(u32 target) const
    {
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 1 & 1;
    }

    bool getColorMaskB(u32 target) const
    {
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 2 & 1;
    }

    bool getColorMaskA(u32 target) const
    {
        u32 color_mask = mColorMask >> (target * 4) & 0xF;
        return color_mask >> 3 & 1;
    }

    u32 getColorMask() const
    {
        return mColorMask;
    }

    bool getStencilTestEnable() const
    {
        return mStencilTestEnable;
    }

    Graphics::StencilFunc getStencilTestFunc() const
    {
        return mStencilTestFunc;
    }

    s32 getStencilTestRef() const
    {
        return mStencilTestRef;
    }

    u32 getStencilTestMask() const
    {
        return mStencilTestMask;
    }

    Graphics::StencilOp getStencilTestOpFail() const
    {
        return mStencilOpFail;
    }

    Graphics::StencilOp getStencilTestOpZFail() const
    {
        return mStencilOpZFail;
    }

    Graphics::StencilOp getStencilTestOpZPass() const
    {
        return mStencilOpZPass;
    }

    Graphics::PolygonMode getPolygonModeFront() const
    {
#ifdef cafe
        return mPolygonModeFront;
#endif // cafe
    }

    Graphics::PolygonMode getPolygonModeBack() const
    {
#ifdef cafe
        return mPolygonModeBack;
#endif // cafe
    }

    bool getPolygonOffsetFrontEnable() const
    {
#ifdef cafe
        return mPolygonOffsetFrontEnable;
#endif // cafe
    }

    bool getPolygonOffsetBackEnable() const
    {
#ifdef cafe
        return mPolygonOffsetBackEnable;
#endif // cafe
    }

    bool getPolygonOffsetPointLineEnable() const
    {
#ifdef cafe
        return mPolygonOffsetPointLineEnable;
#endif // cafe
    }

private:
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
