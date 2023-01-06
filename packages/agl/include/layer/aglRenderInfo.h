#pragma once

#include <layer/aglLayerEnum.h>

namespace sead {

class Camera;
class FrameBuffer;
class Projection;
class Viewport;

}

namespace agl {

class DisplayList;

namespace lyr {

class Layer;

class RenderInfo
{
public:
    RenderInfo(DisplayType display_type, const sead::FrameBuffer* frame_buffer, bool, const Layer* p_layer, DisplayList* p_display_list);
    RenderInfo(DisplayType display_type, const sead::FrameBuffer* frame_buffer, DisplayList* p_display_list);

    s32 getRenderStep() const { return mRenderStep; }
    DisplayType getDisplayType() const { return mDisplayType; }
    const sead::FrameBuffer* getFrameBuffer() const { return mpFrameBuffer; }
    s32 getLayerIndex() const { return mLayerIndex; }
    Layer* getLayer() const { return mpLayer; }
    const sead::Camera* getCamera() const { return mpCamera; }
    const sead::Projection* getProjection() const { return mpProjection; }
    const sead::Viewport* getViewport() const { return mpViewport; }
    DisplayList* getDisplayList() const { return mpDisplayList; }

private:
    s32 mRenderStep;
    DisplayType mDisplayType;
    const sead::FrameBuffer* mpFrameBuffer;
    s32 mLayerIndex;
    Layer* mpLayer;
    const sead::Camera* mpCamera;
    const sead::Projection* mpProjection;
    const sead::Viewport* mpViewport;
    DisplayList* mpDisplayList;
    bool _24;
};
static_assert(sizeof(RenderInfo) == 0x28, "agl::lyr::RenderInfo size mismatch");

} }
