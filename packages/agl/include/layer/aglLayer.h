#pragma once

#include <container/seadBuffer.h>
#include <gfx/seadCamera.h>
#include <gfx/seadColor.h>
#include <gfx/seadProjection.h>
#include <gfx/seadViewport.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <layer/aglLayerEnum.h>
#include <prim/seadBitFlag.h>
#include <prim/seadSafeString.h>

namespace agl { namespace lyr {

class DrawMethod;
class Renderer;
class RenderInfo;
class RenderStep;

class Layer : public sead::IDisposer
{
public:
    Layer();
    virtual ~Layer();

    SEAD_RTTI_BASE(Layer)

    virtual u32 getRenderStepNum() const = 0;
    virtual void getRenderStepName(sead::SafeString* p_name, s32 idx) const = 0;
    virtual void initialize(sead::Heap* heap) { }
    virtual void calc() { }
    virtual void preDraw(const RenderInfo& render_info) const { }
    virtual void postDraw(const RenderInfo& render_info) const { }
    virtual bool isRenderStepGPUCalc(s32 idx) { return false; }
    virtual bool isRenderStepNoDependency(s32 idx) { return false; }

    DrawMethod* pushBackDrawMethod(u32 render_step, DrawMethod* p_method);
    DrawMethod* pushBackDrawMethod(DrawMethod* p_method);

    s32 removeDrawMethod(const DrawMethod* p_draw_method);

    void setCamera(sead::Camera* p_camera)
    {
        mpCamera = p_camera;
    }

    void setProjection(sead::Projection* p_projection)
    {
        mpProjection = p_projection;
    }

    // ...

protected:
    Renderer* mpRenderer;
    sead::Viewport mViewport;
    sead::Viewport mScissor; // I think
    sead::Camera* mpCamera;
    sead::Projection* mpProjection;
    sead::BitFlag32 _4c;
    sead::BitFlag32 _50;
    DisplayType mDisplayType;
    u32 mRenderDisplayFlag;
    u32 mDisplayType2; // ... ?
    sead::BitFlag32 mFlag;
    s32 mIndex;
    u32 _68;
    sead::Color4f mClearColor;
    f32 mClearDepth;
    sead::Buffer<RenderStep> mRenderStep;
    u32 _88;
    u32 mDebugInfo[0x390 / sizeof(u32)]; // TODO: agl::lyr::Layer::DebugInfo
};
static_assert(sizeof(Layer) == 0x41C, "agl::lyr::Layer size mismatch");

} }
