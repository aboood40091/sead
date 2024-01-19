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

void GraphicsContextMRT::apply() const
{
#ifdef cafe
    GX2SetDepthStencilControl(
        static_cast<GX2Boolean>(mDepthTestEnable),
        static_cast<GX2Boolean>(mDepthWriteEnable),
        static_cast<GX2CompareFunction>(mDepthFunc),
        static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail)
    );

    GX2SetStencilMask(mStencilTestMask, mStencilTestRef, mStencilTestRef,  // Bug: ref in place of write mask
                      mStencilTestMask, mStencilTestRef, mStencilTestRef); // ^^^

    {
        static const GX2Boolean cCullSettings[4][2] = {
            { GX2_ENABLE, GX2_DISABLE },
            { GX2_DISABLE, GX2_ENABLE },
            { GX2_DISABLE, GX2_DISABLE },
            { GX2_ENABLE, GX2_ENABLE }
        };
        const GX2Boolean* const& settings = cCullSettings[mCullingMode];

        GX2SetPolygonControl(
            GX2_FRONT_FACE_CCW,
            settings[0],
            settings[1],
            static_cast<GX2Boolean>(mPolygonModeFront != Graphics::cPolygonMode_Fill || mPolygonModeBack != Graphics::cPolygonMode_Fill),
            static_cast<GX2PolygonMode>(mPolygonModeFront),
            static_cast<GX2PolygonMode>(mPolygonModeBack),
            static_cast<GX2Boolean>(mPolygonOffsetFrontEnable),
            static_cast<GX2Boolean>(mPolygonOffsetBackEnable),
            static_cast<GX2Boolean>(mPolygonOffsetPointLineEnable)
        );
    }

    GX2SetColorControl(
        GX2_LOGIC_OP_NONE,
        mBlendEnableMask.getDirect() & 0xFF,
        GX2_DISABLE,
        static_cast<GX2Boolean>(mColorMask != 0)
    );

    for (SafeArray<BlendExpression, Graphics::cRenderTarget_Num>::constIterator itr = mBlendExpression.constBegin(), itr_end = mBlendExpression.constEnd(); itr != itr_end; ++itr)
    {
        GX2SetBlendControl(
            static_cast<GX2RenderTarget>(GX2_RENDER_TARGET_0 + itr.getIndex()),
            static_cast<GX2BlendFunction>(itr->getBlendFactorSrcRGB()),
            static_cast<GX2BlendFunction>(itr->getBlendFactorDstRGB()),
            static_cast<GX2BlendCombine>(itr->getBlendEquationRGB()),
            GX2_ENABLE,
            static_cast<GX2BlendFunction>(itr->getBlendFactorSrcAlpha()),
            static_cast<GX2BlendFunction>(itr->getBlendFactorDstAlpha()),
            static_cast<GX2BlendCombine>(itr->getBlendEquationAlpha())
        );
    }

    GX2SetBlendConstantColor(mBlendConstantColor.r,
                             mBlendConstantColor.g,
                             mBlendConstantColor.b,
                             mBlendConstantColor.a);

    GX2SetAlphaTest(static_cast<GX2Boolean>(mAlphaTestEnable),
                    static_cast<GX2CompareFunction>(mAlphaTestFunc),
                    mAlphaTestRef);

    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(mColorMask >>  0 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  4 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  8 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 12 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 16 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 20 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 24 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 28 & 0xF)
    );

    GX2SetAlphaToMask(GX2_DISABLE, GX2_ALPHA_TO_MASK_0);
#endif // cafe
}

void GraphicsContextMRT::applyAlphaTest() const
{
#ifdef cafe
    GX2SetAlphaTest(static_cast<GX2Boolean>(mAlphaTestEnable),
                    static_cast<GX2CompareFunction>(mAlphaTestFunc),
                    mAlphaTestRef);
#endif // cafe
}

void GraphicsContextMRT::applyDepthAndStencilTest() const
{
#ifdef cafe
    GX2SetDepthStencilControl(
        static_cast<GX2Boolean>(mDepthTestEnable),
        static_cast<GX2Boolean>(mDepthWriteEnable),
        static_cast<GX2CompareFunction>(mDepthFunc),
        static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2Boolean>(mStencilTestEnable),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail),
        static_cast<GX2CompareFunction>(mStencilTestFunc),
        static_cast<GX2StencilFunction>(mStencilOpZPass),
        static_cast<GX2StencilFunction>(mStencilOpZFail),
        static_cast<GX2StencilFunction>(mStencilOpFail)
    );

    GX2SetStencilMask(mStencilTestMask, mStencilTestRef, mStencilTestRef,  // Bug: ref in place of write mask
                      mStencilTestMask, mStencilTestRef, mStencilTestRef); // ^^^
#endif // cafe
}

void GraphicsContextMRT::applyColorMask() const
{
#ifdef cafe
    GX2SetTargetChannelMasks(
        static_cast<GX2ChannelMask>(mColorMask >>  0 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  4 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >>  8 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 12 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 16 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 20 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 24 & 0xF),
        static_cast<GX2ChannelMask>(mColorMask >> 28 & 0xF)
    );
#endif // cafe
}

void GraphicsContextMRT::applyBlendAndFastZ() const
{
#ifdef cafe
    GX2SetColorControl(
        GX2_LOGIC_OP_NONE,
        mBlendEnableMask.getDirect() & 0xFF,
        GX2_DISABLE,
        static_cast<GX2Boolean>(mColorMask != 0)
    );

    for (SafeArray<BlendExpression, Graphics::cRenderTarget_Num>::constIterator itr = mBlendExpression.constBegin(), itr_end = mBlendExpression.constEnd(); itr != itr_end; ++itr)
    {
        GX2SetBlendControl(
            static_cast<GX2RenderTarget>(GX2_RENDER_TARGET_0 + itr.getIndex()),
            static_cast<GX2BlendFunction>(itr->getBlendFactorSrcRGB()),
            static_cast<GX2BlendFunction>(itr->getBlendFactorDstRGB()),
            static_cast<GX2BlendCombine>(itr->getBlendEquationRGB()),
            GX2_ENABLE,
            static_cast<GX2BlendFunction>(itr->getBlendFactorSrcAlpha()),
            static_cast<GX2BlendFunction>(itr->getBlendFactorDstAlpha()),
            static_cast<GX2BlendCombine>(itr->getBlendEquationAlpha())
        );
    }
#endif // cafe
}

void GraphicsContextMRT::applyBlendConstantColor() const
{
#ifdef cafe
    GX2SetBlendConstantColor(mBlendConstantColor.r,
                             mBlendConstantColor.g,
                             mBlendConstantColor.b,
                             mBlendConstantColor.a);
#endif // cafe
}

void GraphicsContextMRT::applyCullingAndPolygonModeAndPolygonOffset() const
{
#ifdef cafe
    static const GX2Boolean cCullSettings[4][2] = {
        { GX2_ENABLE, GX2_DISABLE },
        { GX2_DISABLE, GX2_ENABLE },
        { GX2_DISABLE, GX2_DISABLE },
        { GX2_ENABLE, GX2_ENABLE }
    };
    const GX2Boolean* const& settings = cCullSettings[mCullingMode];

    GX2SetPolygonControl(
        GX2_FRONT_FACE_CCW,
        settings[0],
        settings[1],
        static_cast<GX2Boolean>(mPolygonModeFront != Graphics::cPolygonMode_Fill || mPolygonModeBack != Graphics::cPolygonMode_Fill),
        static_cast<GX2PolygonMode>(mPolygonModeFront),
        static_cast<GX2PolygonMode>(mPolygonModeBack),
        static_cast<GX2Boolean>(mPolygonOffsetFrontEnable),
        static_cast<GX2Boolean>(mPolygonOffsetBackEnable),
        static_cast<GX2Boolean>(mPolygonOffsetPointLineEnable)
    );
#endif // cafe
}

} // namespace sead
