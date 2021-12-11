#ifndef SEAD_GRAPHICS_CONTEXT_H_
#define SEAD_GRAPHICS_CONTEXT_H_

#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>

namespace sead {

class GraphicsContext
{
public:
    GraphicsContext();
    virtual ~GraphicsContext()
    {
    }

    void apply() const;
    void applyAlphaTest() const;
    void applyDepthAndStencilTest() const;
    void applyColorMask() const;
    void applyBlendAndFastZ() const;
    void applyBlendConstantColor() const;
    void applyCullingAndPolygonModeAndPolygonOffset() const;

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

    void setBlendEnable(bool enable)
    {
        mBlendEnable = enable;
    }

    void setBlendFactor(Graphics::BlendFactor src_factor_rgb, Graphics::BlendFactor dst_factor_rgb, Graphics::BlendFactor src_factor_a, Graphics::BlendFactor dst_factor_a)
    {
        mBlendFactorSrcRGB = src_factor_rgb;
        mBlendFactorDstRGB = dst_factor_rgb;
        mBlendFactorSrcA = src_factor_a;
        mBlendFactorDstA = dst_factor_a;
    }

    void setBlendEquation(Graphics::BlendEquation equation_rgb, Graphics::BlendEquation equation_a)
    {
        mBlendEquationRGB = equation_rgb;
        mBlendEquationA = equation_a;
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

    void setColorMask(bool r, bool g, bool b, bool a)
    {
        mColorMaskR = r;
        mColorMaskG = g;
        mColorMaskB = b;
        mColorMaskA = a;
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

    void setPolygonOffsetEnable(bool front, bool back, bool line)
    {
#ifdef cafe
        mPolygonOffsetFrontEnable = front;
        mPolygonOffsetBackEnable = back;
        mPolylineOffsetEnable = line;
#endif // cafe
    }

private:
    bool mDepthTestEnable;
    bool mDepthWriteEnable;
    Graphics::DepthFunc mDepthFunc;
    Graphics::CullingMode mCullingMode;
    bool mBlendEnable;
    Graphics::BlendFactor mBlendFactorSrcRGB;
    Graphics::BlendFactor mBlendFactorSrcA;
    Graphics::BlendFactor mBlendFactorDstRGB;
    Graphics::BlendFactor mBlendFactorDstA;
    Graphics::BlendEquation mBlendEquationRGB;
    Graphics::BlendEquation mBlendEquationA;
    Color4f mBlendConstantColor;
    bool mAlphaTestEnable;
    Graphics::AlphaFunc mAlphaTestFunc;
    f32 mAlphaTestRef;
    bool mColorMaskR;
    bool mColorMaskG;
    bool mColorMaskB;
    bool mColorMaskA;
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
    bool mPolylineOffsetEnable;
#endif // cafe
};
static_assert(sizeof(GraphicsContext) == 0x74, "sead::GraphicsContext size mismatch");

} // namespace sead

#endif // SEAD_GRAPHICS_CONTEXT_H_
