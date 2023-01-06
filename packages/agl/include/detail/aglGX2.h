#pragma once

#include <common/aglShaderEnum.h>
#include <common/aglShaderOptimizeInfo.h>
#include <heap/seadDisposer.h>
#include <hostio/seadHostIODummy.h>
#include <thread/seadCriticalSection.h>

namespace agl { namespace driver {

class GX2Resource : public sead::hostio::Node
{
    SEAD_SINGLETON_DISPOSER(GX2Resource)

public:
    GX2Resource();
    virtual ~GX2Resource();

    u32 initialize(sead::Heap* heap, void* work_buffer, u32 gs_in_ring_item_size = 0x10, u32 gs_out_ring_item_size = 0x80);

    void setShaderMode(ShaderMode mode) const;
    void setShaderMode(ShaderMode mode, const ShaderOptimizeInfo& info) const;

    void setGeometryShaderRingBuffer() const;

    void restoreContextState();

private:
    ShaderOptimizeInfo* mpOptimizeInfo;
    ShaderOptimizeInfo mDefaultOptimizeInfo;
    size_t mGeometryShaderInputRingItemSize;
    size_t mGeometryShaderOutputRingItemSize;
    u8* mGeometryShaderInputRingBuffer;
    size_t mGeometryShaderInputRingBufferSize;
    u8* mGeometryShaderOutputRingBuffer;
    size_t mGeometryShaderOutputRingBufferSize;
    sead::CriticalSection mCriticalSection;
    bool mStateShadowEnable;
    bool mUseStateDisplayList;
};
static_assert(sizeof(GX2Resource) == 0x80, "agl::driver::GX2Resource size mismatch");

} }
