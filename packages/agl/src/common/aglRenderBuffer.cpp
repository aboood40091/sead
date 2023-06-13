#include <common/aglRenderBuffer.h>
#include <common/aglRenderTarget.h>
#include <mc/seadCoreInfo.h>

namespace agl {

sead::Buffer<const RenderBuffer*> RenderBuffer::sBoundRenderBuffer;

RenderBuffer::RenderBuffer()
{
    initialize_();
}

RenderBuffer::~RenderBuffer()
{
    for (s32 i = 0; i < sBoundRenderBuffer.size(); i++)
        if (sBoundRenderBuffer[i] == this)
            sBoundRenderBuffer[i] = nullptr;
}

void RenderBuffer::initialize_()
{
    setRenderTargetColorNullAll();
    setRenderTargetDepthNull();
}

void RenderBuffer::copyToDisplayBuffer(const sead::DisplayBuffer* display_buffer) const
{
    const sead::DisplayBufferCafe* p_display_buffer_cafe = sead::DynamicCast<const sead::DisplayBufferCafe>(display_buffer);

    // SEAD_ASSERT(mColorTarget[ 0 ] != nullptr);

    mColorTarget[0]->invalidateGPUCache();
    mColorTarget[0]->getTextureData().invalidateGPUCache();

    mColorTarget[0]->copyToDisplayBuffer(p_display_buffer_cafe);
}

void RenderBuffer::clear(u32 clr_flag, const sead::Color4f& color, f32 depth, u32 stencil) const
{
    clear(0, clr_flag, color, depth, stencil);
}

void RenderBuffer::clear(u32 target_index, u32 clr_flag, const sead::Color4f& color, f32 depth, u32 stencil) const
{
    if (!clr_flag)
        return;

    if (target_index == 0)
    {
        if (mDepthTarget)
            GX2SetClearDepthStencil(&mDepthTarget->mInnerBuffer, depth, stencil);

        if (clr_flag & 1)
        {
            if ((clr_flag & 2) && (clr_flag & 4))
            {
                GX2ClearBuffersEx(
                    &mColorTarget[0]->mInnerBuffer,
                    &mDepthTarget->mInnerBuffer,
                    color.r, color.g, color.b, color.a,
                    depth, stencil, GX2_CLEAR_BOTH
                );
            }
            else if (clr_flag & 2)
            {
                GX2ClearBuffersEx(
                    &mColorTarget[0]->mInnerBuffer,
                    &mDepthTarget->mInnerBuffer,
                    color.r, color.g, color.b, color.a,
                    depth, stencil, GX2_CLEAR_DEPTH
                );
            }
            else if (clr_flag & 4)
            {
                GX2ClearBuffersEx(
                    &mColorTarget[0]->mInnerBuffer,
                    &mDepthTarget->mInnerBuffer,
                    color.r, color.g, color.b, color.a,
                    depth, stencil, GX2_CLEAR_STENCIL
                );
            }
            else
            {
                GX2ClearColor(
                    &mColorTarget[0]->mInnerBuffer,
                    color.r, color.g, color.b, color.a
                );
            }
        }
        else
        {
            if ((clr_flag & 2) && (clr_flag & 4))
            {
                GX2ClearDepthStencilEx(&mDepthTarget->mInnerBuffer, depth, stencil, GX2_CLEAR_BOTH);
            }
            else if (clr_flag & 2)
            {
                GX2ClearDepthStencilEx(&mDepthTarget->mInnerBuffer, depth, stencil, GX2_CLEAR_DEPTH);
            }
            else if (clr_flag & 4)
            {
                GX2ClearDepthStencilEx(&mDepthTarget->mInnerBuffer, depth, stencil, GX2_CLEAR_STENCIL);
            }
        }
    }
    else
    {
        if (clr_flag & 1)
        {
            GX2ClearColor(
                &getRenderTargetColor(target_index)->mInnerBuffer,
                color.r, color.g, color.b, color.a
            );
        }
    }

    driver::GX2Resource::instance()->restoreContextState();
}

void RenderBuffer::bindImpl_() const
{
    sBoundRenderBuffer[sead::CoreInfo::getCurrentCoreId()] = this;

    for (sead::SafeArray<RenderTargetColor*, 8>::constIterator it = mColorTarget.constBegin(), it_end = mColorTarget.constEnd(); it != it_end; ++it)
    {
        const RenderTargetColor* const p_color_target = *it;
        if (p_color_target)
            p_color_target->bind(it.getIndex());
    }

    if (mDepthTarget)
        mDepthTarget->bind();
}

void RenderBuffer::setRenderTargetColorNullAll()
{
    for (s32 i = 0; i < 8; i++)
        mColorTarget[i] = nullptr;
}

}
