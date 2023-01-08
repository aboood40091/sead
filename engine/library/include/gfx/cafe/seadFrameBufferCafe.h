#ifndef SEAD_FRAME_BUFFER_CAFE_H_
#define SEAD_FRAME_BUFFER_CAFE_H_

#include <gfx/seadFrameBuffer.h>

namespace sead {

class FrameBufferCafe : public FrameBuffer
{
    SEAD_RTTI_OVERRIDE(FrameBufferCafe, FrameBuffer)

public:
    FrameBufferCafe(GX2ColorBuffer* color_buffer, GX2DepthBuffer* depth_buffer)
        : FrameBuffer()
        , mColorBuffer(color_buffer)
        , mDepthBuffer(depth_buffer)
    {
    }

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

class DisplayBufferCafe : public DisplayBuffer
{
    SEAD_RTTI_OVERRIDE(DisplayBufferCafe, DisplayBuffer)

public:
    enum ScanOutTarget
    {
        cTarget_TV,
        cTarget_DRC,
    };
    static_assert(sizeof(ScanOutTarget) == 4, "sead::DisplayBufferCafe::ScanOutTarget size mismatch");

    enum Resolution
    {
        cResolution_Auto, // Maybe? Idk
        cResolution_720P
    };
    static_assert(sizeof(Resolution) == 4, "sead::DisplayBufferCafe::Resolution size mismatch");

public:
    DisplayBufferCafe(ScanOutTarget target, Resolution resolution);

    ScanOutTarget getScanOutTarget() const { return mScanOutTarget; }
    Resolution getResolution() const { return mResolution; }

private:
    virtual void initializeImpl_(sead::Heap* heap);

private:
    ScanOutTarget mScanOutTarget;
    Resolution mResolution;
    void* mpScanBuffer;
};
static_assert(sizeof(DisplayBufferCafe) == 0x18, "sead::DisplayBufferCafe size mismatch");

}  // namespace sead

#endif // SEAD_FRAME_BUFFER_CAFE_H_
