#include <gfx/seadGraphicsContext.h>

namespace sead {

GraphicsContext::GraphicsContext()
    : mDepthTestEnable(true)
    , mDepthWriteEnable(true)
    , mDepthFunc(Graphics::cDepthFunc_LessEqual)

    , mCullingMode(Graphics::cCullingMode_Back)

    , mBlendEnable(true)
    , mBlendFactorSrcRGB(Graphics::cBlendFactor_SrcAlpha)
    , mBlendFactorSrcA(Graphics::cBlendFactor_SrcAlpha)
    , mBlendFactorDstRGB(Graphics::cBlendFactor_InvSrcAlpha)
    , mBlendFactorDstA(Graphics::cBlendFactor_InvSrcAlpha)
    , mBlendEquationRGB(Graphics::cBlendEquation_Add)
    , mBlendEquationA(Graphics::cBlendEquation_Add)
    , mBlendConstantColor(1.0f, 1.0f, 1.0f, 1.0f)

    , mAlphaTestEnable(false)
    , mAlphaTestFunc(Graphics::cAlphaFunc_Greater)
    , mAlphaTestRef(0.0f)

    , mColorMaskR(true)
    , mColorMaskG(true)
    , mColorMaskB(true)
    , mColorMaskA(true)

    , mStencilTestEnable(false)
    , mStencilTestFunc(Graphics::cStencilFunc_Never)
    , mStencilTestRef(0)
    , mStencilTestMask(0xFFFFFFFF)
    , mStencilOpFail(Graphics::cStencilOp_Keep)
    , mStencilOpZFail(Graphics::cStencilOp_Keep)
    , mStencilOpZPass(Graphics::cStencilOp_Keep)

#ifdef cafe
    , mPolygonModeFront(Graphics::cPolygonMode_Fill)
    , mPolygonModeBack(Graphics::cPolygonMode_Fill)
    , mPolygonOffsetFrontEnable(false)
    , mPolygonOffsetBackEnable(false)
    , mPolylineOffsetEnable(false)
#endif // cafe
{
}

} // namespace sead
