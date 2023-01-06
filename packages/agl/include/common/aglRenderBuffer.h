#pragma once

#include <container/seadBuffer.h>
#include <container/seadSafeArray.h>
#include <gfx/seadFrameBuffer.h>
#include <heap/seadDisposer.h>
#include <math/seadVector.h>

namespace agl {

class RenderTargetColor;
class RenderTargetDepth;

class RenderBuffer : public sead::IDisposer, public sead::FrameBuffer
{
    SEAD_RTTI_OVERRIDE(RenderBuffer, sead::FrameBuffer)

public:
    RenderBuffer();
    RenderBuffer(const sead::Vector2f& virtual_size, const sead::BoundBox2f& physical_area);
    RenderBuffer(const sead::Vector2f& virtual_size, f32 physical_x, f32 physical_y, f32 physical_w, f32 physical_h);
    virtual ~RenderBuffer();

private:
    void initialize_();

public:
    virtual void copyToDisplayBuffer(const sead::DisplayBuffer* display_buffer) const;
    virtual void clear(u32 clr_flag, const sead::Color4f& color, f32 depth, u32 stencil) const;
    virtual void bindImpl_() const;

    RenderTargetColor* getRenderTargetColor(u32 target_index = 0) const
    {
        return mColorTarget[target_index];
    }

    RenderTargetDepth* getRenderTargetDepth() const
    {
        return mDepthTarget;
    }

    void setRenderTargetColor(RenderTargetColor* target, u32 target_index = 0)
    {
        mColorTarget[target_index] = target;
    }

    void setRenderTargetDepth(RenderTargetDepth* target)
    {
        mDepthTarget = target;
    }

    void setRenderTargetColorNullAll();
    void setRenderTargetDepthNull() { setRenderTargetDepth(nullptr); }

    void clear(u32 target_index, u32 clr_flag, const sead::Color4f& color, f32 depth, u32 stencil) const;

private:
    sead::SafeArray<RenderTargetColor*, 8> mColorTarget;
    RenderTargetDepth* mDepthTarget;

    static sead::Buffer<const RenderBuffer*> sBoundRenderBuffer;
};
static_assert(sizeof(RenderBuffer) == 0x50, "agl::RenderBuffer size mistmatch");

}
