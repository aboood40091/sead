#ifndef SEAD_GRAPHICS_CAFE_H_
#define SEAD_GRAPHICS_CAFE_H_

#include <container/seadSafeArray.h>
#include <gfx/seadGraphics.h>
#include <prim/seadBitFlag.h>

namespace sead {

class GraphicsCafe : public Graphics
{
public:
    class BlendExpression
    {
    public:
        BlendExpression()
            : mBlendFactorSrcRGB(Graphics::cBlendFactor_SrcAlpha)
            , mBlendFactorSrcA(Graphics::cBlendFactor_SrcAlpha)
            , mBlendFactorDstRGB(Graphics::cBlendFactor_InvSrcAlpha)
            , mBlendFactorDstA(Graphics::cBlendFactor_InvSrcAlpha)
            , mBlendEquationRGB(Graphics::cBlendEquation_Add)
            , mBlendEquationA(Graphics::cBlendEquation_Add)
        {
        }

        void setBlendFactor(Graphics::BlendFactor src_factor, Graphics::BlendFactor dst_factor)
        {
            setBlendFactorSrc(src_factor);
            setBlendFactorDst(dst_factor);
        }

        void setBlendFactorSeparate(Graphics::BlendFactor src_factor_rgb, Graphics::BlendFactor dst_factor_rgb, Graphics::BlendFactor src_factor_a, Graphics::BlendFactor dst_factor_a)
        {
            setBlendFactorSrcRGB(src_factor_rgb);
            setBlendFactorDstRGB(dst_factor_rgb);
            setBlendFactorSrcAlpha(src_factor_a);
            setBlendFactorDstAlpha(dst_factor_a);
        }

        void setBlendFactorSrc(Graphics::BlendFactor factor)
        {
            setBlendFactorSrcRGB(factor);
            setBlendFactorSrcAlpha(factor);
        }

        void setBlendFactorDst(Graphics::BlendFactor factor)
        {
            setBlendFactorDstRGB(factor);
            setBlendFactorDstAlpha(factor);
        }

        void setBlendFactorSrcRGB(Graphics::BlendFactor factor)
        {
            mBlendFactorSrcRGB = factor;
        }

        void setBlendFactorSrcAlpha(Graphics::BlendFactor factor)
        {
            mBlendFactorSrcA = factor;
        }

        void setBlendFactorDstRGB(Graphics::BlendFactor factor)
        {
            mBlendFactorDstRGB = factor;
        }

        void setBlendFactorDstAlpha(Graphics::BlendFactor factor)
        {
            mBlendFactorDstA = factor;
        }

        void setBlendEquation(Graphics::BlendEquation equation)
        {
            setBlendEquationRGB(equation);
            setBlendEquationAlpha(equation);
        }

        void setBlendEquationSeparate(Graphics::BlendEquation equation_rgb, Graphics::BlendEquation equation_a)
        {
            setBlendEquationRGB(equation_rgb);
            setBlendEquationAlpha(equation_a);
        }

        void setBlendEquationRGB(Graphics::BlendEquation equation)
        {
            mBlendEquationRGB = equation;
        }

        void setBlendEquationAlpha(Graphics::BlendEquation equation)
        {
            mBlendEquationA = equation;
        }

        Graphics::BlendFactor getBlendFactorSrcRGB() const
        {
            return mBlendFactorSrcRGB;
        }

        Graphics::BlendFactor getBlendFactorSrcAlpha() const
        {
            return mBlendFactorSrcA;
        }

        Graphics::BlendFactor getBlendFactorDstRGB() const
        {
            return mBlendFactorDstRGB;
        }

        Graphics::BlendFactor getBlendFactorDstAlpha() const
        {
            return mBlendFactorDstA;
        }

        Graphics::BlendEquation getBlendEquationRGB() const
        {
            return mBlendEquationRGB;
        }

        Graphics::BlendEquation getBlendEquationAlpha() const
        {
            return mBlendEquationA;
        }

    private:
        Graphics::BlendFactor mBlendFactorSrcRGB;
        Graphics::BlendFactor mBlendFactorSrcA;
        Graphics::BlendFactor mBlendFactorDstRGB;
        Graphics::BlendFactor mBlendFactorDstA;
        Graphics::BlendEquation mBlendEquationRGB;
        Graphics::BlendEquation mBlendEquationA;
    };

public:
    static GraphicsCafe* instance() { return static_cast<GraphicsCafe*>(sInstance); }

public:
    GraphicsCafe() { }
    virtual ~GraphicsCafe();

    GX2ContextState* getGX2ContextState() { return mContextState; }
    void setGX2ContextState(GX2ContextState* context_state) { mContextState = context_state; }

private:
    BitFlag32 mFlags;
    GX2CompareFunction mDepathCompareFunc;
    SafeArray<BlendExpression, cRenderTarget_Num> mBlendExpression;
    GX2CompareFunction mStencilCompareFunc;
    GX2StencilFunction mStencilStencilZPassFunction;
    GX2StencilFunction mStencilStencilZFailFunction;
    GX2StencilFunction mStencilStencilFailFunction;
    GX2CompareFunction mAlphaCompareFunc;
    f32 mAlphaTestRef;
    GX2ContextState* mContextState;
    BitFlag32 mBlendEnableMask;
    u32 mColorMask;
    GX2Boolean mIsCullFront;
    GX2Boolean mIsCullBack;
    GX2PolygonMode mPolygonModeFront;
    GX2PolygonMode mPolygonModeBack;
    GX2Boolean mPolygonOffsetFrontEnable;
    GX2Boolean mPolygonOffsetBackEnable;
    GX2Boolean mPolygonOffsetPointLineEnable;
};
static_assert(sizeof(GraphicsCafe) == 0x15C, "sead::GraphicsCafe size mismatch");

}  // namespace sead

#endif // SEAD_GRAPHICS_CAFE_H_
