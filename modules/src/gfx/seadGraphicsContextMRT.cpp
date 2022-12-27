#include <gfx/seadGraphicsContextMRT.h>

namespace sead {

GraphicsContextMRT::GraphicsContextMRT()
    : mDepthTestEnable(true)
    , mDepthWriteEnable(true)
    , mDepthFunc(Graphics::cDepthFunc_LessEqual)
    , mCullingMode(Graphics::cCullingMode_Back)
    , mBlendEnableMask(0xFF)
    , mBlendConstantColor(1.0f, 1.0f, 1.0f, 1.0f)
    , mAlphaTestEnable(false)
    , mAlphaTestFunc(Graphics::cAlphaFunc_Greater)
    , mAlphaTestRef(0.0f)
    , mColorMask(0xFFFFFFFF)
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
    , mPolygonOffsetPointLineEnable(false)
#endif // cafe
{
}

} // namespace sead
