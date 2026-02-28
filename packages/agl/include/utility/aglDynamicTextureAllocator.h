#pragma once

#include <common/aglTextureData.h>
#include <container/seadSafeArray.h>
#include <heap/seadDisposer.h>
#include <prim/seadBitFlag.h>
#include <thread/seadCriticalSection.h>
#include <utility/aglTextureMemoryAllocator.h>

namespace agl { namespace utl {

class TextureDataEx;

class DynamicTextureAllocator
{
    SEAD_SINGLETON_DISPOSER(DynamicTextureAllocator)

public:
    enum AllocateType
    {
        cAllocateType_Main = 0, // User-side initializes the memory for the allocators. In NSMBU, this is 32 MB of MEM1 (eDRAM)
        cAllocateType_Sub1,     // In NSMBU, this is 32 MB of MEM2 (DDR3 RAM)
        cAllocateType_Sub2,     // In NSMBU, this is null
        cAllocateType_Num
    };
    static_assert(cAllocateType_Num == 3, "cAllocateType_Num mismatch");

private:
    struct Context
    {
        sead::Buffer<TextureDataEx> mTextureDataEx;
        sead::UnsafeArray<TextureMemoryAllocator, cAllocateType_Num> mTextureMemoryAllocator;
    };
    static_assert(sizeof(Context) == 0x908, "agl::utl::DynamicTextureAllocator::Context size mismatch");

    static const s32 cCoreMaxNum = 3;

    enum ContextType
    {
        cContextType_Persistent = cCoreMaxNum, // The context for persistent textures that are shared across all cores
        cContextType_Num
    };
    static_assert(cContextType_Num == 4, "cContextType_Num mismatch");

    friend class TextureDataEx;

public:
    DynamicTextureAllocator();
    virtual ~DynamicTextureAllocator();

    TextureData* alloc(
        const sead::SafeString& name,
        TextureFormat format,
        u32 width, u32 height, u32 mip_level_num,
        void** pp_buffer = nullptr,
        AllocateType allocate_type = cAllocateType_Main,
        bool invalidate_gpu_cache = true
    );

    void free(const TextureData* ptr);

private:
    bool isValid_(const Context* p_ctx) const;
    bool isContextValid_(const Context* p_ctx) const;

    TextureData* alloc_(
        Context* p_ctx,
        const sead::SafeString& name,
        TextureFormat format, TextureType type,
        u32 width, u32 height, u32 mip_level_num,
        MultiSampleType multi_sample_type,
        u32 slice_num,
        void** pp_buffer, bool allocate_from_head,
        AllocateType allocate_type,
        bool invalidate_gpu_cache
    );

    bool free_(Context* p_ctx, const TextureData* ptr);

private:
    sead::BitFlag32 _10;
    sead::UnsafeArray<Context, cContextType_Num> mContext;
    sead::CriticalSection mCriticalSection;
    u32 _2470;
    sead::Heap* mpDebugHeap;
};
static_assert(sizeof(DynamicTextureAllocator) == 0x247C, "agl::utl::DynamicTextureAllocator size mismatch");

class TextureDataEx : public TextureData
{
public:
    TextureDataEx();

    u8 getMipLevelNum() const { return mMipLevelNum; }

private:
    TextureMemoryAllocator*                 mpMemoryAllocator;
    TextureMemoryAllocator::MemoryBlock*    mpMemoryBlock;
    sead::SafeString                        mName;
    u32                                     mAllocatorUsedSize;
    u8                                      mAllocType;
    u8                                      mLRUCounter;        // "Least Recently Used" counter for eviction
    u8                                      mMipLevelNum;
    sead::BitFlag8                          mFlag;
    u8                                      mSurfaceSwizzle;
    u8                                      _b5;
    u8                                      _pad[2];
    DynamicTextureAllocator::Context*       mpContext;

    friend class DynamicTextureAllocator;
};
static_assert(sizeof(TextureDataEx) == 0xBC, "agl::utl::TextureDataEx size mismatch");

} }
