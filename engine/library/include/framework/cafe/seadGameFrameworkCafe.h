#ifndef SEAD_GAME_FRAMEWORK_CAFE_H_
#define SEAD_GAME_FRAMEWORK_CAFE_H_

#include <gfx/seadFrameBuffer.h>
#include <framework/seadGameFramework.h>

namespace sead {

class GameFrameworkCafe : public GameFramework
{
    SEAD_RTTI_OVERRIDE(GameFrameworkCafe, GameFramework)

public:
    static void initialize(const InitializeArg& arg);

public:
    struct CreateArg
    {
        u32     wait_vblank;
        Color4f clear_color;
        u32     width;
        u32     height;
        u32     cmd_buf_size;
        bool    create_default_framebuffer;

        CreateArg()
             : wait_vblank(1)
             , clear_color(Color4f::cGray)
             , width(1280)
             , height(720)
             , cmd_buf_size(4 * 1024 * 1024)
             , create_default_framebuffer(true)
        {
        }
    };
    static_assert(sizeof(CreateArg) == 0x24, "sead::GameFrameworkCafe::CreateArg size mismatch");

    enum DrawOrder
    {
        cDrawOrder_TV_DRC = 0,
        cDrawOrder_DRC_TV,
        cDrawOrder_Num
    };
    static_assert(sizeof(DrawOrder) == 4, "sead::GameFrameworkCafe::DrawOrder size mismatch");
    static_assert(cDrawOrder_Num == 2);

    typedef void (*DeferredCopyCallback)(s32 stage); // 0 = begin, 1 = end

public:
    GameFrameworkCafe(const CreateArg& arg);
    virtual ~GameFrameworkCafe();

    void initializeGraphicsSystem(
        Heap* heap, s32 argc = 0, char** argv = nullptr,
        const Vector2f& tv_virtual_fb_size = Vector2f::zero, const Vector2f& drc_virtual_fb_size = Vector2f(854.0f, 480.0f),
        Heap* heap_for_display_buffer = nullptr
    );

    void setVBlankWaitInterval(u32 wait_vblank);

    virtual FrameBuffer* getMethodFrameBuffer(s32 method_type) const;
    virtual const LogicalFrameBuffer* getMethodLogicalFrameBuffer(s32 method_type) const;

protected:
    virtual void initRun_(Heap*);
    virtual void runImpl_();
    virtual MethodTreeMgr* createMethodTreeMgr_(Heap* heap);

public:
    virtual f32 calcFps()
    {
        TickSpan sec_span = TickSpan::makeFromSeconds(1);
        return static_cast<f32>(sec_span.toS64()) / static_cast<f32>(mFrameSpan.toS64());
    }

    virtual void setCaption(const SafeString& caption)
    {
        // SEAD_PRINT("%s\n", caption.cstr());
    }

protected:
    virtual void mainLoop_();
    virtual void procFrame_();
    virtual void procDraw_();
    virtual void procCalc_();
    virtual void waitForNextFrame_();
    virtual void swapBuffer_();
    virtual void clearFrameBuffers_(s32 method_type);

    void gpuMesureEnd_();

    void drawTV_();
    void copyToTV_();

    void drawDRC_();
    void copyToDRC_();

protected:
    enum
    {
        cGpuCounter_TVDrawBegin,
        cGpuCounter_TVDrawEnd,
        cGpuCounter_DRCDrawBegin,
        cGpuCounter_DRCDrawEnd,
        cGpuCounter_NumMax
    };
    static const size_t cGpuCounterSize = sizeof(u64) * cGpuCounter_NumMax;

    CreateArg mArg;
    BitFlag32 mFlag;
    TickSpan mFrameSpan;
    TickTime mLastFrameBegin;
    FrameBuffer* mDefaultFrameBuffer;
    FrameBuffer* mDefaultFrameBufferDRC;
    LogicalFrameBuffer mLogicalFrameBuffer;
    LogicalFrameBuffer mLogicalFrameBufferDRC;
    DrawOrder mDrawOrder;
    void* mCommandBuffer;
    GX2ColorBuffer mColorBuffer;
    GX2ColorBuffer mColorBufferDRC;
    GX2DepthBuffer mDepthBuffer;
    GX2DepthBuffer mDepthBufferDRC;
    DisplayBuffer* mDisplayBuffer;
    DisplayBuffer* mDisplayBufferDRC;
    FrameBuffer* mFrameBuffer;
    FrameBuffer* mFrameBufferDRC;
    DeferredCopyCallback mDeferredCopyCallback;
    bool mCopyPostDrawAll;
    u64* mGpuCounters;
  //u32 _374[4 / sizeof(u32)];  // alignment?
};
static_assert(sizeof(GameFrameworkCafe) == 0x378, "sead::GameFrameworkCafe size mismatch");

} // namespace sead

#endif // SEAD_GAME_FRAMEWORK_CAFE_H_
