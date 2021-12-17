#ifndef SEAD_VIEWPORT_H_
#define SEAD_VIEWPORT_H_

#include <gfx/seadGraphics.h>
#include <gfx/seadFrameBuffer.h>

namespace sead {

class Viewport : public BoundBox2f
{
public:
    Viewport();
    Viewport(f32, f32, f32, f32);
    Viewport(const BoundBox2f& box);
    Viewport(const LogicalFrameBuffer& frame_buffer);

    virtual ~Viewport()
    {
    }

    void apply(const LogicalFrameBuffer& frame_buffer) const;
    // ...

protected:
    Graphics::DevicePosture mDevicePos;
};
#ifdef cafe
static_assert(sizeof(Viewport) == 0x18, "sead::Viewport size mismatch");
#endif // cafe

}  // namespace sead

#endif // SEAD_VIEWPORT_H_
