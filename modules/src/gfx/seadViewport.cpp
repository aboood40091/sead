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

void Viewport::getOnFrameBufferPos(Vector2f* dst, const LogicalFrameBuffer& fb) const
{
    *dst = getMin();

    switch (mDevicePos)
    {
    case Graphics::cDevicePosture_Same:
        break;
    case Graphics::cDevicePosture_RotateRight:
        {
            f32 y = (fb.getVirtualSize().y - getSizeX()) - dst->x;
            dst->set(dst->y, y);
        }
        break;
    case Graphics::cDevicePosture_RotateLeft:
        {
            f32 x = (fb.getVirtualSize().x - getSizeY()) - dst->y;
            dst->set(x, dst->x);
        }
        break;
    case Graphics::cDevicePosture_FlipXY:
        {
            f32 y = (fb.getVirtualSize().y - getSizeY()) - dst->y;
            f32 x = (fb.getVirtualSize().x - getSizeX()) - dst->x;
            dst->set(x, y);
        }
        break;
    case Graphics::cDevicePosture_FlipX:
        {
            f32 x = (fb.getVirtualSize().x - getSizeX()) - dst->x;
            dst->set(x, dst->y);
        }
        break;
    case Graphics::cDevicePosture_FlipY:
        {
            f32 y = (fb.getVirtualSize().y - getSizeY()) - dst->y;
            dst->set(dst->x, y);
        }
        break;
    default:
        // SEAD_ASSERT_MSG(false, "Undefined DevicePosture(%d)", s32(mDevicePos));
    }

    dst->div(fb.getVirtualSize());
    dst->x *= fb.getPhysicalArea().getSizeX();
    dst->y *= fb.getPhysicalArea().getSizeY();
    dst->add(fb.getPhysicalArea().getMin());
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
