#ifndef SEAD_GRAPHICS_CAFE_H_
#define SEAD_GRAPHICS_CAFE_H_

#include <container/seadSafeArray.h>
#include <gfx/seadGraphics.h>
#include <prim/seadBitFlag.h>

namespace sead {

class GraphicsCafe : public Graphics
{
    struct BlendExpression
    {
        BlendExpression()
            : blend_factor_src_rgb(cBlendFactor_SrcAlpha)
            , blend_factor_src_a(cBlendFactor_SrcAlpha)
            , blend_factor_dst_rgb(cBlendFactor_InvSrcAlpha)
            , blend_factor_dst_a(cBlendFactor_InvSrcAlpha)
            , blend_equation_rgb(cBlendEquation_Add)
            , blend_equation_a(cBlendEquation_Add)
        {
        }

        BlendFactor blend_factor_src_rgb;
        BlendFactor blend_factor_src_a;
        BlendFactor blend_factor_dst_rgb;
        BlendFactor blend_factor_dst_a;
        BlendEquation blend_equation_rgb;
        BlendEquation blend_equation_a;
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
