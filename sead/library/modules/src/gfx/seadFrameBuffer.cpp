#include <gfx/seadFrameBuffer.h>

namespace sead {

void FrameBuffer::clearMRT(u32 target, const Color4f& color) const
{
}

void FrameBuffer::bind() const
{
    bindImpl_();
}

} // namespace sead
