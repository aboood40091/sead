#ifndef SEAD_FRAME_BUFFER_CAFE_H_
#define SEAD_FRAME_BUFFER_CAFE_H_

#include <gfx/seadFrameBuffer.h>

namespace sead {

class FrameBufferCafe : public FrameBuffer
{
    SEAD_RTTI_OVERRIDE(FrameBufferCafe, FrameBuffer)

public:
    FrameBufferCafe(const Vector2f& virtual_size, const BoundBox2f& physical_area, GX2ColorBuffer* color_buffer, GX2DepthBuffer* depth_buffer)
        : FrameBuffer(virtual_size, physical_area)
        , mColorBuffer(color_buffer)
        , mDepthBuffer(depth_buffer)
    {
    }

    FrameBufferCafe(const Vector2f& virtual_size, f32 physical_x, f32 physical_y, f32 physical_w, f32 physical_h, GX2ColorBuffer* color_buffer, GX2DepthBuffer* depth_buffer)
        : FrameBuffer(virtual_size, physical_x, physical_y, physical_w, physical_h)
        , mColorBuffer(color_buffer)
        , mDepthBuffer(depth_buffer)
    {
    }

    FrameBufferCafe(const Vector2f& virtual_size, f32 physical_x, f32 physical_y, u32 physical_w, u32 physical_h, GX2ColorBuffer* color_buffer, GX2DepthBuffer* depth_buffer)
        : FrameBuffer(virtual_size, physical_x, physical_y, physical_w, physical_h)
        , mColorBuffer(color_buffer)
        , mDepthBuffer(depth_buffer)
    {
    }

    virtual void copyToDisplayBuffer(const DisplayBuffer* display_buffer) const;
    virtual void clear(u32 clr_flag, const Color4f& color, f32 depth, u32 stencil) const;
    virtual void bindImpl_() const;

private:
    GX2ColorBuffer* mColorBuffer;
    GX2DepthBuffer* mDepthBuffer;
};
static_assert(sizeof(FrameBufferCafe) == 0x24, "sead::FrameBufferCafe size mismatch");

}  // namespace sead

#endif // SEAD_FRAME_BUFFER_CAFE_H_
