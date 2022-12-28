#include <gfx/seadViewport.h>

namespace sead {

Viewport::Viewport(const LogicalFrameBuffer& frame_buffer)
    : mDevicePos(Graphics::getDefaultDevicePosture())
{
    setByFrameBuffer(frame_buffer);
}

void Viewport::setByFrameBuffer(const LogicalFrameBuffer& frame_buffer)
{
    switch (mDevicePos)
    {
    case Graphics::cDevicePosture_Same:
    case Graphics::cDevicePosture_FlipX:
    case Graphics::cDevicePosture_FlipY:
    case Graphics::cDevicePosture_FlipXY:
        set(0.0f, 0.0f, frame_buffer.getVirtualSize().x, frame_buffer.getVirtualSize().y);
        break;
    case Graphics::cDevicePosture_RotateRight:
    case Graphics::cDevicePosture_RotateLeft:
        set(0.0f, 0.0f, frame_buffer.getVirtualSize().y, frame_buffer.getVirtualSize().x);
        break;
    default:
        // SEAD_ASSERT_MSG(false, "Undefined DevicePosture(%d)", s32(mDevicePos));
    }
}

void Viewport::apply(const LogicalFrameBuffer& frame_buffer) const
{
    sead::Vector2f fb_pos, fb_size;

    getOnFrameBufferPos(&fb_pos, frame_buffer);
    getOnFrameBufferSize(&fb_size, frame_buffer);

    fb_pos.y = (frame_buffer.getPhysicalArea().getSizeY() - fb_size.y) - fb_pos.y;

    sead::Graphics::instance()->setViewportImpl(fb_pos.x, fb_pos.y, fb_size.x, fb_size.y);
    sead::Graphics::instance()->setScissorImpl(fb_pos.x, fb_pos.y, fb_size.x, fb_size.y);
}

}  // namespace sead
