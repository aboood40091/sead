#include <detail/aglGX2.h>
#include <detail/aglRootNode.h>
#include <gfx/cafe/seadGraphicsCafe.h>

namespace agl { namespace driver {

SEAD_SINGLETON_DISPOSER_IMPL(GX2Resource)

GX2Resource::GX2Resource()
    : mpOptimizeInfo(&mDefaultOptimizeInfo)
    , mDefaultOptimizeInfo()
    , mGeometryShaderInputRingItemSize(0)
    , mGeometryShaderOutputRingItemSize(0)
    , mGeometryShaderInputRingBuffer(nullptr)
    , mGeometryShaderInputRingBufferSize(0)
    , mGeometryShaderOutputRingBuffer(nullptr)
    , mGeometryShaderOutputRingBufferSize(0)
    , mCriticalSection()
    , mStateShadowEnable(true)
    , mUseStateDisplayList(false)
{
    detail::RootNode::setNodeMeta(this, "Icon = CIRCLE_BLUE");
}

GX2Resource::~GX2Resource()
{
    if (mGeometryShaderInputRingBuffer)
    {
        delete[] mGeometryShaderInputRingBuffer;
        mGeometryShaderInputRingBuffer = nullptr;
    }

    if (mGeometryShaderOutputRingBuffer)
    {
        delete[] mGeometryShaderOutputRingBuffer;
        mGeometryShaderOutputRingBuffer = nullptr;
    }
}

u32 GX2Resource::initialize(sead::Heap* heap, void* work_buffer, u32 gs_in_ring_item_size, u32 gs_out_ring_item_size)
{
    mGeometryShaderOutputRingItemSize = gs_out_ring_item_size;
    mGeometryShaderInputRingItemSize = gs_in_ring_item_size;

#ifdef cafe
    mGeometryShaderInputRingBufferSize = GX2CalcGeometryShaderInputRingBufferSize(mGeometryShaderInputRingItemSize);
    mGeometryShaderOutputRingBufferSize = GX2CalcGeometryShaderOutputRingBufferSize(mGeometryShaderOutputRingItemSize);

    u8* work = (u8*)(work_buffer);
    if (work)
    {
        if (mGeometryShaderInputRingBufferSize > 0)
        {
            mGeometryShaderInputRingBuffer = (u8*)(uintptr_t(work) + (0x100 - 1) & ~(0x100 - 1));
            work = mGeometryShaderInputRingBuffer + mGeometryShaderInputRingBufferSize;
        }
        if (mGeometryShaderOutputRingBufferSize > 0)
        {
            mGeometryShaderOutputRingBuffer = (u8*)(uintptr_t(work) + (0x100 - 1) & ~(0x100 - 1));
            work = mGeometryShaderOutputRingBuffer + mGeometryShaderOutputRingBufferSize;
        }
    }
    else
    {
        if (mGeometryShaderInputRingBufferSize > 0)
            mGeometryShaderInputRingBuffer = new (heap, 0x100) u8[mGeometryShaderInputRingBufferSize];

        if (mGeometryShaderOutputRingBufferSize > 0)
            mGeometryShaderOutputRingBuffer = new (heap, 0x100) u8[mGeometryShaderOutputRingBufferSize];
    }

    if (mGeometryShaderInputRingBuffer)
        DCFlushRange(mGeometryShaderInputRingBuffer, mGeometryShaderInputRingBufferSize);

    if (mGeometryShaderOutputRingBuffer)
        DCFlushRange(mGeometryShaderOutputRingBuffer, mGeometryShaderOutputRingBufferSize);

    return uintptr_t(work) - uintptr_t(work_buffer);
#else
    return 0;
#endif
}

void GX2Resource::setShaderMode(ShaderMode mode) const
{
    setShaderMode(mode, *mpOptimizeInfo);
}

void GX2Resource::setShaderMode(ShaderMode mode, const ShaderOptimizeInfo& info) const
{
#ifdef cafe
    switch (mode)
    {
    case cShaderMode_UniformRegister:
        GX2SetShaderModeEx(
            GX2_SHADER_MODE_UNIFORM_REGISTER,
            info.getVertexShaderGprs(),
            info.getVertexShaderStackSize(),
            0,
            0,
            info.getFragmentShaderGprs(),
            info.getFragmentShaderStackSize()
        );
        break;
    case cShaderMode_UniformBlock:
        GX2SetShaderModeEx(
            GX2_SHADER_MODE_UNIFORM_BLOCK,
            info.getVertexShaderGprs(),
            info.getVertexShaderStackSize(),
            0,
            0,
            info.getFragmentShaderGprs(),
            info.getFragmentShaderStackSize()
        );
        GX2Invalidate(GX2_INVALIDATE_UNIFORM_BLOCK, nullptr, 0xFFFFFFFF);
        break;
    case cShaderMode_GeometryShader:
        GX2SetShaderModeEx(
            GX2_SHADER_MODE_GEOMETRY_SHADER,
            info.getVertexShaderWithGSGprs(),
            info.getVertexShaderWithGSStackSize(),
            info.getGeometryShaderGprs(),
            info.getGeometryShaderStackSize(),
            info.getFragmentShaderWithGSGprs(),
            info.getFragmentShaderWithGSStackSize()
        );
        GX2Invalidate(GX2_INVALIDATE_UNIFORM_BLOCK, nullptr, 0xFFFFFFFF);
        break;
    }
#endif // cafe
}

void GX2Resource::setGeometryShaderRingBuffer() const
{
#ifdef cafe
    GX2SetGeometryShaderInputRingBuffer(mGeometryShaderInputRingBuffer, mGeometryShaderInputRingBufferSize);
    GX2SetGeometryShaderOutputRingBuffer(mGeometryShaderOutputRingBuffer, mGeometryShaderOutputRingBufferSize);
#endif // cafe
}

#ifdef cafe

namespace {

inline void GetContextStateDisplayList(GX2ContextState* state, void** pp_dl, u32* p_size)
{
    // Either this function was inline back in the GX2 headers NSMBU used...
    // GX2GetContextStateDisplayList(state, pp_dl, p_size);

    // ... Or they did this
    *pp_dl = &state->data[0x9E00 / sizeof(u32)];
    *p_size = state->data[0x9804 / sizeof(u32)];
}

}

#endif // cafe

void GX2Resource::restoreContextState()
{
#ifdef cafe
    if (mStateShadowEnable)
    {
        GX2ContextState* state = sead::GraphicsCafe::instance()->getGX2ContextState();

        if (!mUseStateDisplayList)
            GX2SetContextState(state);

        else
        {
            void* p_dl;
            u32 size;
            GetContextStateDisplayList(state, &p_dl, &size);

            GX2CallDisplayList(p_dl, size);
        }
    }
    else
    {
        GX2SetContextState(nullptr);
    }
#endif // cafe
}

} }
