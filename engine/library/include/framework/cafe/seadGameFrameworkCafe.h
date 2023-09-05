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

public:
    GameFrameworkCafe(const CreateArg& arg);
    virtual ~GameFrameworkCafe();

    virtual FrameBuffer* getMethodFrameBuffer(s32) const;
    virtual LogicalFrameBuffer* getMethodLogicalFrameBuffer(s32) const;

protected:
    virtual void initRun_(Heap*);
    virtual void runImpl_();
    virtual MethodTreeMgr* createMethodTreeMgr_(Heap*);

public:
    virtual f32 calcFps();

    virtual void setCaption(const SafeString&);

protected:
    virtual void mainLoop_();
    virtual void procFrame_();
    virtual void procDraw_();
    virtual void procCalc_();
    virtual void waitForNextFrame_();
    virtual void swapBuffer_();
    virtual void clearFrameBuffers_(s32);

public:
    void initializeGraphicsSystem(
        Heap* heap, s32 argc = 0, char** argv = nullptr,
        const Vector2f& tv_virtual_fb_size = Vector2f::zero, const Vector2f& drc_virtual_fb_size = Vector2f(854.0f, 480.0f),
        Heap* heap_for_display_buffer = nullptr
    );

protected:
    CreateArg mArg;
    BitFlag32 _6c;
    TickSpan mFrameSpan;
    TickTime mLastFrameBegin;
    FrameBuffer* mpDefaultFrameBuffer;
    FrameBuffer* mpDefaultFrameBufferDRC;
    LogicalFrameBuffer mLogicalFrameBuffer;
    LogicalFrameBuffer mLogicalFrameBufferDRC;
    u32 _c0;
    void* mpCommandBuffer;
    GX2ColorBuffer mColorBuffer;
    GX2ColorBuffer mColorBufferDRC;
    GX2DepthBuffer mDepthBuffer;
    GX2DepthBuffer mDepthBufferDRC;
    DisplayBuffer* mpDisplayBuffer;
    DisplayBuffer* mpDisplayBufferDRC;
    FrameBuffer* mpFrameBuffer;
    FrameBuffer* mpFrameBufferDRC;
    s32 mCurrentDisplaybuffer;
    u8 _36c;
    void* _370;
    u32 _374[4 / sizeof(u32)];
};
static_assert(sizeof(GameFrameworkCafe) == 0x378, "sead::GameFrameworkCafe size mismatch");

} // namespace sead

#endif // SEAD_GAME_FRAMEWORK_CAFE_H_
