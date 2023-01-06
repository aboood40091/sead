#pragma once

#include <container/seadBuffer.h>
#include <container/seadPtrArray.h>
#include <heap/seadDisposer.h>
#include <layer/aglLayerEnum.h>
#include <math/seadVector.h>
#include <thread/seadCriticalSection.h>

namespace sead {

class Controller;

}

namespace agl {

class RenderBuffer;

namespace lyr {

class DrawMethod;
class Layer;

class Renderer
{
    SEAD_SINGLETON_DISPOSER(Renderer)

public:
    Renderer();
    virtual ~Renderer();

    void removeDrawMethod(const DrawMethod* p_draw_method);

    Layer* getLayer(s32 index) const { return mLayer[index]; }

private:
    u32 _10;
    u32 mMultiSampleType; // agl::MultiSampleType
    u32 mRenderDisplay[cDisplayType_Max][0x6C4 / sizeof(u32)]; // agl::lyr::RenderDisplay[cDisplayType_Max]
    RenderBuffer* mRenderBuffer[cDisplayType_Max];
    sead::Buffer<Layer*> mLayer;
    mutable sead::BitFlag16 mFlag;
    f32 _db4;
    u8 _db8[16];
    sead::CriticalSection mCriticalSection;
    u32 mJobQueue[2][0x64 / sizeof(u32)]; // sead::FixedSizeJQ[2]
    sead::PtrArrayImpl mRenderDisplayJob[cDisplayType_Max]; // sead::PtrArray<agl::lyr::RenderDisplay::Job>[cDisplayType_Max]
    u32 _ee4;
    u32 _ee8;
    s32 _eeC;
    sead::Controller* mpDebugController;
    f32 _ef4;
    sead::Vector2f _ef8;
    u32 _f00;
    u32 mRenderDLBuffer[0x58 / sizeof(u32)]; // agl::lyr::RenderDLBuffer
    u32 mGpuCounter;
    u32 mGpuCounterPeriod;
    u32 _f64;
    s32 mColorBufferSwizzle;
    s32 mDepthBufferSwizzle;
    f32 _f70;
};
static_assert(sizeof(Renderer) == 0xF78, "agl::lyr::Renderer size mismatch");

} }
