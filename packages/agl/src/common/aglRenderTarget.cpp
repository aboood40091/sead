#include <common/aglRenderTarget.h>
#include <prim/seadMemUtil.h>

namespace agl {

void RenderTargetColor::onApplyTextureData_()
{
    mIsMSAA2D = mTextureData.getTextureType() == cTextureType_2D_MSAA;

    mInnerBuffer.surface = mTextureData.getSurface();
    mInnerBuffer.surface.use = GX2_SURFACE_USE_COLOR_BUFFER;

    if (mIsMSAA2D)
        GX2CalcColorBufferAuxInfo(&mInnerBuffer, &mAuxBufferSize, &mAuxBufferAlign);
}

RenderTargetColor::RenderTargetColor()
    : RenderTarget<RenderTargetColor>()
    , mIsMSAA2D(false)
    , mAuxBufferSize(0)
    , mAuxBufferAlign(0x800)
    , mpAuxBuffer(nullptr)
{
    sead::MemUtil::fillZero(&mInnerBuffer, sizeof(GX2ColorBuffer));
    GX2InitColorBufferAuxPtr(&mInnerBuffer, nullptr);
}

void RenderTargetColor::invalidateGPUCache() const
{
    if (mTextureData.getImagePtr())
        GX2Invalidate(GX2_INVALIDATE_COLOR_BUFFER, mTextureData.getImagePtr(), mTextureData.getImageByteSize());

    if (mTextureData.getMipPtr())
        GX2Invalidate(GX2_INVALIDATE_COLOR_BUFFER, mTextureData.getMipPtr(), mTextureData.getMipByteSize());
}

void RenderTargetColor::initRegs_() const
{
    mInnerBuffer.viewMip = mMipLevel;
    mInnerBuffer.viewFirstSlice = mSlice;
    mInnerBuffer.viewNumSlices = 1;
    GX2InitColorBufferRegs(&mInnerBuffer);
    mUpdateRegs = false;
}

void RenderTargetDepth::onApplyTextureData_()
{
    mInnerBuffer.surface = mTextureData.getSurface();
    mInnerBuffer.surface.use = GX2_SURFACE_USE_DEPTH_BUFFER;

    GX2CalcDepthBufferHiZInfo(&mInnerBuffer, &mHiZBufferSize, &mHiZBufferAlign);
}

RenderTargetDepth::RenderTargetDepth()
    : RenderTarget<RenderTargetDepth>()
    , mHiZBufferSize(0)
    , mHiZBufferAlign(0x800)
    , mpHiZBuffer(nullptr)
{
    sead::MemUtil::fillZero(&mInnerBuffer, sizeof(GX2DepthBuffer));
    GX2InitDepthBufferHiZPtr(&mInnerBuffer, nullptr);
    GX2InitDepthBufferHiZEnable(&mInnerBuffer, GX2_DISABLE);
}

void RenderTargetDepth::initRegs_() const
{
    mInnerBuffer.viewMip = mMipLevel;
    mInnerBuffer.viewFirstSlice = mSlice;
    mInnerBuffer.viewNumSlices = 1;
    mInnerBuffer.clearDepth = 1.0f;
    mInnerBuffer.clearStencil = 0;
    GX2InitDepthBufferRegs(&mInnerBuffer);
    mUpdateRegs = false;
}

}
