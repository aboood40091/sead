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
    sead::Vector2f real_pos;
    getOnFrameBufferPos(&real_pos, frame_buffer);

    sead::Vector2f real_size;
    getOnFrameBufferSize(&real_size, frame_buffer);

    // SEAD_ASSERT(frame_buffer.getPhysicalArea().isInside(real_pos) && frame_buffer.getPhysicalArea().isInside(real_pos + real_size));

    real_pos.y = (frame_buffer.getPhysicalArea().getSizeY() - real_size.y) - real_pos.y;

    sead::Graphics::instance()->setViewportRealPosition(real_pos.x, real_pos.y, real_size.x, real_size.y);
    sead::Graphics::instance()->setScissorRealPosition(real_pos.x, real_pos.y, real_size.x, real_size.y);
}

}  // namespace sead
