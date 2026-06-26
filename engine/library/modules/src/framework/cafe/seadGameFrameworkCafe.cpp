#include <basis/seadNew.h>
#include <framework/cafe/seadGameFrameworkCafe.h>
#include <framework/seadDualScreenMethodTreeMgr.h>
#include <framework/seadTaskMgr.h>
#include <gfx/cafe/seadFrameBufferCafe.h>
#include <gfx/cafe/seadGraphicsCafe.h>
#include <heap/seadHeap.h>

namespace {

void* allocFromMem1FrmHeap(size_t size, s32 alignment)
{
    return MEMAllocFromFrmHeapEx(MEMGetBaseHeapHandle(MEM_ARENA_1), size, alignment);
}

}

namespace sead {

void GameFrameworkCafe::initialize(const Framework::InitializeArg& arg)
{
    GameFramework::initialize(arg);
}

GameFrameworkCafe::GameFrameworkCafe(const CreateArg& arg)
    : GameFramework()
    , mArg(arg)
    , mFlag(0)
    , mDefaultFrameBuffer(nullptr)
    , mDefaultFrameBufferDRC(nullptr)
    , mDrawOrder(cDrawOrder_TV_DRC)
    , mFrameBuffer(nullptr)
    , mFrameBufferDRC(nullptr)
    , mDeferredCopyCallback(nullptr)
    , mCopyPostDrawAll(false)
{
}

GameFrameworkCafe::~GameFrameworkCafe()
{
}

void GameFrameworkCafe::initializeGraphicsSystem(
    Heap* heap, s32 argc, char** argv,
    const Vector2f& tv_virtual_fb_size, const Vector2f& drc_virtual_fb_size,
    Heap* heap_for_display_buffer
)
{
    mCommandBuffer = heap->alloc(mArg.cmd_buf_size, GX2_DEFAULT_BUFFER_ALIGNMENT);

    u32 init_attribs[] = {
        GX2_INIT_ATTRIB_CB_BASE, (u32)mCommandBuffer,
        GX2_INIT_ATTRIB_CB_SIZE, mArg.cmd_buf_size,
        GX2_INIT_ATTRIB_ARGC, (u32)argc,
        GX2_INIT_ATTRIB_ARGV, (u32)argv,
        GX2_INIT_ATTRIB_NULL
    };
    GX2Init(init_attribs);

    Graphics::setInstance(new (heap) GraphicsCafe());
    Graphics::instance()->initialize();

    GX2ContextState* context_state = (GX2ContextState*)heap->alloc(sizeof(GX2ContextState), GX2_CONTEXT_STATE_ALIGNMENT);
    GX2SetupContextStateEx(context_state, GX2_DISABLE);
    GraphicsCafe::instance()->setGX2ContextState(context_state);

    if (mArg.create_default_framebuffer)
    {
        GX2InitColorBuffer(&mColorBuffer, mArg.width, mArg.height, GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM, GX2_AA_MODE_1X);
        mColorBuffer.surface.imagePtr = allocFromMem1FrmHeap(mColorBuffer.surface.imageSize, mColorBuffer.surface.alignment);

        GX2InitDepthBuffer(&mDepthBuffer, mArg.width, mArg.height, GX2_SURFACE_FORMAT_TCD_R32_FLOAT, GX2_AA_MODE_1X);
        mDepthBuffer.surface.imagePtr = allocFromMem1FrmHeap(mDepthBuffer.surface.imageSize, mDepthBuffer.surface.alignment);

        u32 tv_hiZSize, tv_hiZAlign;
        GX2CalcDepthBufferHiZInfo(&mDepthBuffer, &tv_hiZSize, &tv_hiZAlign);
        void* tv_hiZPtr = allocFromMem1FrmHeap(tv_hiZSize, tv_hiZAlign);
        mDepthBuffer.hiZPtr = tv_hiZPtr;
        GX2InitDepthBufferHiZEnable(&mDepthBuffer, tv_hiZPtr ? GX2_ENABLE : GX2_DISABLE);

        GX2InitColorBuffer(&mColorBufferDRC, 854, 480, GX2_SURFACE_FORMAT_TCS_R8_G8_B8_A8_UNORM, GX2_AA_MODE_1X);
        // SEAD_ASSERT(mColorBufferDRC.surface.imageSize <= mColorBuffer.surface.imageSize);
        // SEAD_ASSERT(mColorBufferDRC.surface.alignment <= mColorBuffer.surface.alignment);
        mColorBufferDRC.surface.imagePtr = mColorBuffer.surface.imagePtr;

        GX2InitDepthBuffer(&mDepthBufferDRC, 854, 480, GX2_SURFACE_FORMAT_TCD_R32_FLOAT, GX2_AA_MODE_1X);
        // SEAD_ASSERT(mDepthBufferDRC.surface.imageSize <= mDepthBuffer.surface.imageSize);
        // SEAD_ASSERT(mDepthBufferDRC.surface.alignment <= mDepthBuffer.surface.alignment);
        mDepthBufferDRC.surface.imagePtr = mDepthBuffer.surface.imagePtr;

        u32 hiZSize, hiZAlign;
        GX2CalcDepthBufferHiZInfo(&mDepthBufferDRC, &hiZSize, &hiZAlign);
        // SEAD_ASSERT(mDepthBufferDRC.hiZSize <= hiZSize);
        // SEAD_ASSERT(tv_hiZAlign <= hiZAlign);
        void* hiZPtr = mDepthBuffer.hiZPtr;
        mDepthBufferDRC.hiZPtr = hiZPtr;
        GX2InitDepthBufferHiZEnable(&mDepthBufferDRC, hiZPtr ? GX2_ENABLE : GX2_DISABLE);

        mDefaultFrameBuffer = new (heap) FrameBufferCafe(
            (tv_virtual_fb_size.x == 0.0f && tv_virtual_fb_size.y == 0.0f) ? Vector2f(mArg.width, mArg.height) : tv_virtual_fb_size,
            0.0f, 0.0f, mArg.width, mArg.height, &mColorBuffer, &mDepthBuffer);
        // SEAD_ASSERT(mDefaultFrameBuffer);

        mDefaultFrameBufferDRC = new (heap) FrameBufferCafe(
            drc_virtual_fb_size,
            0.0f, 0.0f, 854.0f, 480.0f, &mColorBufferDRC, &mDepthBufferDRC);
        // SEAD_ASSERT(mDefaultFrameBufferDRC);

        mDefaultFrameBuffer->bind();
    }

    // SEAD_ASSERT(mArg.height == 720 || mArg.height == 1080);
    DisplayBufferCafe::Resolution resolution = mArg.height == 720 ? DisplayBufferCafe::cResolution_720P : DisplayBufferCafe::cResolution_1080P;

    mDisplayBuffer = new (heap) DisplayBufferCafe(DisplayBufferCafe::cTarget_TV, resolution);
    mDisplayBuffer->initialize(mArg.width, mArg.height, heap_for_display_buffer ? heap_for_display_buffer : heap);

    mDisplayBufferDRC = new (heap) DisplayBufferCafe(DisplayBufferCafe::cTarget_DRC, resolution);
    mDisplayBufferDRC->initialize(854.0f, 480.0f, heap_for_display_buffer ? heap_for_display_buffer : heap);

    mGpuCounters = (u64*)heap->alloc(
        SEAD_MACRO_UTIL_ROUNDUP(cGpuCounterSize, GX2_DEFAULT_BUFFER_ALIGNMENT),
        GX2_DEFAULT_BUFFER_ALIGNMENT
    );

    GX2SetSwapInterval(mArg.wait_vblank);

    mLogicalFrameBuffer.setVirtualSize((tv_virtual_fb_size.x == 0.0f && tv_virtual_fb_size.y == 0.0f) ? Vector2f(mArg.width, mArg.height) : tv_virtual_fb_size);

    BoundBox2f tv_physical_area(0.0f, 0.0f, mArg.width, mArg.height);
    mLogicalFrameBuffer.setPhysicalArea(tv_physical_area);

    mLogicalFrameBufferDRC.setVirtualSize(drc_virtual_fb_size);

    BoundBox2f drc_physical_area(0.0f, 0.0f, 854.0f, 480.0f);
    mLogicalFrameBufferDRC.setPhysicalArea(drc_physical_area);
}

void GameFrameworkCafe::initRun_(Heap*)
{
}

void GameFrameworkCafe::runImpl_()
{
    waitStartDisplayLoop_();
    mLastFrameBegin.setNow();
    mainLoop_();
}

MethodTreeMgr* GameFrameworkCafe::createMethodTreeMgr_(Heap* heap)
{
    DualScreenMethodTreeMgr* mgr = new (heap) DualScreenMethodTreeMgr();
    mgr->setSysDrawScreen(DualScreenMethodTreeMgr::cScreen_Top);
    return mgr;
}

void GameFrameworkCafe::mainLoop_()
{
    while (true)
        procFrame_();
}

void GameFrameworkCafe::gpuMesureEnd_()
{
    DCInvalidateRange(mGpuCounters, cGpuCounterSize);

    s32 idx_tv_draw_begin = mDrawOrder == cDrawOrder_TV_DRC ? cGpuCounter_TVDrawBegin : cGpuCounter_DRCDrawBegin;
    s32 idx_tv_draw_end = mDrawOrder == cDrawOrder_TV_DRC ? cGpuCounter_TVDrawEnd : cGpuCounter_DRCDrawEnd;
    s32 idx_drc_draw_begin = mDrawOrder == cDrawOrder_TV_DRC ? cGpuCounter_DRCDrawBegin : cGpuCounter_TVDrawBegin;
    s32 idx_drc_draw_end = mDrawOrder == cDrawOrder_TV_DRC ? cGpuCounter_DRCDrawEnd : cGpuCounter_TVDrawEnd;

    TickSpan pre_tv_draw_span(GX2GPUTimeToCPUTime(mGpuCounters[idx_tv_draw_begin]) - mLastFrameBegin.toU64());
    TickTime tv_draw_begin_time = mLastFrameBegin + pre_tv_draw_span;
    mGPUMeter.measureBegin(tv_draw_begin_time);

    TickSpan tv_draw_span(GX2GPUTimeToCPUTime(mGpuCounters[idx_tv_draw_end]) - GX2GPUTimeToCPUTime(mGpuCounters[idx_tv_draw_begin]));
    TickTime tv_draw_end_time = tv_draw_begin_time + tv_draw_span;
    mGPUMeter.measureEnd(tv_draw_end_time);

    TickSpan pre_drc_draw_span(GX2GPUTimeToCPUTime(mGpuCounters[idx_drc_draw_begin]) - GX2GPUTimeToCPUTime(mGpuCounters[idx_tv_draw_begin]));
    TickTime drc_draw_begin_time = tv_draw_begin_time + pre_drc_draw_span;
    mGPUMeter.measureBegin(drc_draw_begin_time);

    TickSpan drc_draw_span(GX2GPUTimeToCPUTime(mGpuCounters[idx_drc_draw_end]) - GX2GPUTimeToCPUTime(mGpuCounters[idx_tv_draw_begin]));
    TickTime drc_draw_end_time = tv_draw_begin_time + drc_draw_span;
    mGPUMeter.measureEnd(drc_draw_end_time);
}

void GameFrameworkCafe::procFrame_()
{
    bool draw = true;
    if (mFlag.isOnBit(0) && !mFlag.isOnBit(1))
        draw = false;

    if (draw && ProcessMeter::instance() != nullptr)
        ProcessMeter::instance()->measureBeginFrame();

    bool swap_buffer;

    Graphics::instance()->lockDrawContext();
    {
        // End calculation of the previous frame & draw it
        mTaskMgr->afterCalc();
        procDraw_();

        // Begin calculation of the next frame
        procCalc_();

        procReset_();

        swap_buffer = true;
        if (mFlag.isOnBit(0) && mFlag.isOnBit(1))
            swap_buffer = false;

        if (swap_buffer)
        {
            swapBuffer_();
            gpuMesureEnd_();
        }
    }
    Graphics::instance()->unlockDrawContext();

    if (swap_buffer && ProcessMeter::instance() != nullptr)
        ProcessMeter::instance()->measureEndFrame();

    mFrameSpan = mLastFrameBegin.diffToNow();
    mLastFrameBegin.setNow();

    if (mDisplayState == cDisplayState_Ready)
        mDisplayState = cDisplayState_Show;

    if (mFlag.isOnBit(0))
        mFlag.toggleBit(1);

    waitForNextFrame_();
}

void GameFrameworkCafe::procDraw_()
{
    if (mFlag.isOnBit(0) && !mFlag.isOnBit(1))
        return;

    mDrawMeter.measureBegin();
    {
        if (mDrawOrder == cDrawOrder_TV_DRC)
            drawTV_();
        else
            drawDRC_();

        if (!mCopyPostDrawAll)
        {
            if (mDrawOrder == cDrawOrder_TV_DRC)
                copyToTV_();
            else
                copyToDRC_();
        }

        if (mDrawOrder == cDrawOrder_TV_DRC)
            drawDRC_();
        else
            drawTV_();

        if (mDeferredCopyCallback == nullptr)
        {
            if (mCopyPostDrawAll)
            {
                if (mDrawOrder == cDrawOrder_TV_DRC)
                    copyToTV_();
                else
                    copyToDRC_();
            }

            if (mDrawOrder == cDrawOrder_TV_DRC)
                copyToDRC_();
            else
                copyToTV_();
        }

        GX2Flush();
    }
    mDrawMeter.measureEnd();
}

void GameFrameworkCafe::drawTV_()
{
    DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);

    GX2SampleTopGPUCycle(&(mGpuCounters[cGpuCounter_TVDrawBegin]));
    {
        if (mDefaultFrameBuffer != nullptr)
            mDefaultFrameBuffer->bind();

        clearFrameBuffers_(6);

        method_tree_mgr->drawTop();
    }
    GX2SampleBottomGPUCycle(&(mGpuCounters[cGpuCounter_TVDrawEnd]));
}

void GameFrameworkCafe::drawDRC_()
{
    DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);

    GX2SampleTopGPUCycle(&(mGpuCounters[cGpuCounter_DRCDrawBegin]));
    {
        if (mDefaultFrameBufferDRC != nullptr)
            mDefaultFrameBufferDRC->bind();

        clearFrameBuffers_(9);

        method_tree_mgr->drawBtm();
    }
    GX2SampleBottomGPUCycle(&(mGpuCounters[cGpuCounter_DRCDrawEnd]));
}

void GameFrameworkCafe::procCalc_()
{
    mCalcMeter.measureBegin();
    {
        mTaskMgr->beforeCalc();

        DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
        method_tree_mgr->calc();
    }
    mCalcMeter.measureEnd();
}

void GameFrameworkCafe::waitForNextFrame_()
{
    if (mArg.wait_vblank)
    {
        u32 swapCount, flipCount;
        OSTime lastFlipTime, lastVsyncTime;
        do
        {
            GX2WaitForVsync();
            GX2GetSwapStatus(&swapCount, &flipCount, &lastFlipTime, &lastVsyncTime);
        }
        while (flipCount < swapCount);
    }
}

void GameFrameworkCafe::swapBuffer_()
{
    if (mFlag.isOnBit(0) && mFlag.isOnBit(1))
        return;

    if (mDeferredCopyCallback != nullptr)
    {
        (*mDeferredCopyCallback)(0);
        {
            GX2DrawDone();

            if (mCopyPostDrawAll)
            {
                if (mDrawOrder == cDrawOrder_TV_DRC)
                    copyToTV_();
                else
                    copyToDRC_();
            }

            if (mDrawOrder == cDrawOrder_TV_DRC)
                copyToDRC_();
            else
                copyToTV_();

            GX2Flush();
        }
        (*mDeferredCopyCallback)(1);
    }

    if (mDisplayState == cDisplayState_Show)
        GX2SwapScanBuffers();

    GX2DrawDone();
}

void GameFrameworkCafe::copyToTV_()
{
    if (mFrameBuffer == nullptr)
    {
        if (mDefaultFrameBuffer != nullptr)
            mDefaultFrameBuffer->copyToDisplayBuffer(mDisplayBuffer);
    }
    else
    {
        mFrameBuffer->copyToDisplayBuffer(mDisplayBuffer);
        mFrameBuffer = nullptr;
    }

    GX2SetContextState(GraphicsCafe::instance()->getGX2ContextState());
}

void GameFrameworkCafe::copyToDRC_()
{
    if (mFrameBufferDRC == nullptr)
    {
        if (mDefaultFrameBufferDRC != nullptr)
            mDefaultFrameBufferDRC->copyToDisplayBuffer(mDisplayBufferDRC);
    }
    else
    {
        mFrameBufferDRC->copyToDisplayBuffer(mDisplayBufferDRC);
        mFrameBufferDRC = nullptr;
    }

    GX2SetContextState(GraphicsCafe::instance()->getGX2ContextState());
}

void GameFrameworkCafe::clearFrameBuffers_(s32 method_type)
{
    FrameBuffer* fb = getMethodFrameBuffer(method_type);
    if (fb != nullptr)
        fb->clear((
                1 << 0 |    // color
                1 << 1 |    // depth
                1 << 2      // stencil
            ), mArg.clear_color, 1.0f, 0);
}

void GameFrameworkCafe::setVBlankWaitInterval(u32 wait_vblank)
{
    mArg.wait_vblank = wait_vblank;
    GX2SetSwapInterval(mArg.wait_vblank);
}

FrameBuffer* GameFrameworkCafe::getMethodFrameBuffer(s32 method_type) const
{
    switch (method_type)
    {
    case 2:
        {
            DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
            if (method_tree_mgr->getSysDrawScreen() == DualScreenMethodTreeMgr::cScreen_Top)
                return mDefaultFrameBuffer;
            else
                return mDefaultFrameBufferDRC;
        }
    case 3:
    case 4:
        {
            DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
            if (method_tree_mgr->getAppDrawScreen() == DualScreenMethodTreeMgr::cScreen_Top)
                return mDefaultFrameBuffer;
            else
                return mDefaultFrameBufferDRC;
        }
    case 5:
    case 6:
    case 7:
        return mDefaultFrameBuffer;
    case 8:
    case 9:
    case 10:
        return mDefaultFrameBufferDRC;
    default:
        // SEAD_ASSERT_MSG(false, "Undefined MethodType(%d).", method_type);
        return nullptr;
    }
}

const LogicalFrameBuffer* GameFrameworkCafe::getMethodLogicalFrameBuffer(s32 method_type) const
{
    switch (method_type)
    {
    case 2:
        {
            DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
            if (method_tree_mgr->getSysDrawScreen() == DualScreenMethodTreeMgr::cScreen_Top)
                return &mLogicalFrameBuffer;
            else
                return &mLogicalFrameBufferDRC;
        }
    case 3:
    case 4:
        {
            DualScreenMethodTreeMgr* method_tree_mgr = DynamicCast<DualScreenMethodTreeMgr>(mMethodTreeMgr);
            if (method_tree_mgr->getAppDrawScreen() == DualScreenMethodTreeMgr::cScreen_Top)
                return &mLogicalFrameBuffer;
            else
                return &mLogicalFrameBufferDRC;
        }
    case 5:
    case 6:
    case 7:
        return &mLogicalFrameBuffer;
    case 8:
    case 9:
    case 10:
        return &mLogicalFrameBufferDRC;
    default:
        // SEAD_ASSERT_MSG(false, "Undefined MethodType(%d).", method_type);
        return nullptr;
    }
}

} // namespace sead
