#include <container/seadBuffer.h>
#include <mc/seadCoreInfo.h>
#include <utility/aglDynamicTextureAllocator.h>

namespace agl { namespace utl {

DynamicTextureAllocator* DynamicTextureAllocator::sInstance = nullptr;

bool DynamicTextureAllocator::isValid_(const Context* p_ctx) const
{
    if (p_ctx)
        return isContextValid_(p_ctx);

    for (sead::UnsafeArray<Context, cContextType_Num>::constIterator itr_ctx = mContext.constBegin(), itr_ctx_end = --mContext.constEnd(); itr_ctx != itr_ctx_end; ++itr_ctx)
        if (!isContextValid_(&(*itr_ctx)))
            return false;

    return true;
}

bool DynamicTextureAllocator::isContextValid_(const Context* p_ctx) const
{
    const Context* p_self_ctx = &(mContext[cContextType_Persistent]);

    int overwrapperd =  // Yes, you read that correctly. "int", not "bool".
        (p_ctx->mTextureMemoryAllocator[cAllocateType_Main].isOverwrapperd(p_self_ctx->mTextureMemoryAllocator[cAllocateType_Main]) ||
         p_ctx->mTextureMemoryAllocator[cAllocateType_Sub1].isOverwrapperd(p_self_ctx->mTextureMemoryAllocator[cAllocateType_Sub1]) ||
         p_ctx->mTextureMemoryAllocator[cAllocateType_Sub2].isOverwrapperd(p_self_ctx->mTextureMemoryAllocator[cAllocateType_Sub2]));

    return !overwrapperd;
}

TextureData* DynamicTextureAllocator::alloc(
    const sead::SafeString& name,
    TextureFormat format,
    u32 width, u32 height, u32 mip_level_num,
    void** pp_buffer,
    AllocateType allocate_type,
    bool invalidate_gpu_cache
)
{
    return alloc_(
        &(mContext[s32(sead::CoreInfo::getCurrentCoreId())]),
        name,
        format, cTextureType_2D,
        width, height, mip_level_num,
        cMultiSampleType_1x,
        1,
        pp_buffer, true,
        allocate_type,
        invalidate_gpu_cache
    );
}

TextureData* DynamicTextureAllocator::alloc_(
    Context* p_ctx,
    const sead::SafeString& name,
    TextureFormat format, TextureType type,
    u32 width, u32 height, u32 mip_level_num,
    MultiSampleType multi_sample_type,
    u32 slice_num,
    void** pp_buffer, bool allocate_from_head,
    AllocateType allocate_type,
    bool invalidate_gpu_cache
)
{
    // SEAD_ASSERT(p_ctx != nullptr);

    u16 index = sead::MathCalcCommon<u16>::maxNumber();
    TextureDataEx* p_tex = nullptr;
    bool match_found = false;

    for (sead::Buffer<TextureDataEx>::iterator itr_tex = p_ctx->mTextureDataEx.begin(), itr_tex_end = p_ctx->mTextureDataEx.end(); itr_tex != itr_tex_end; ++itr_tex)
    {
        TextureDataEx& tex = *itr_tex;
        if (tex.mpMemoryBlock == nullptr)
        {
            if (tex.getTextureFormat() == format &&
                tex.getTextureType() == type &&
                tex.getWidth() == width &&
                tex.getHeight() == height &&
                tex.getMultiSampleType() == multi_sample_type &&
                tex.getSliceNum() == slice_num &&
                tex.getMipLevelNum() == mip_level_num)
            {
                match_found = true;
                p_tex = &tex;
                break;
            }

            if (tex.mLRUCounter > 0)
                tex.mLRUCounter--;

            if (tex.mLRUCounter < index)
            {
                index = tex.mLRUCounter;
                p_tex = &tex;
            }
        }
    }

    // SEAD_ASSERT(p_tex != nullptr);

    if (!match_found)
    {
        p_tex->mMipLevelNum = mip_level_num;

        switch (type)
        {
        case cTextureType_2D:
            p_tex->initialize(format, width, height, mip_level_num);
            break;
        case cTextureType_3D:
            p_tex->initialize3D(format, width, height, slice_num, mip_level_num);
            break;
        case cTextureType_CubeMap:
            if (slice_num == 1)
                p_tex->initializeCubeMap(format, width, height, mip_level_num);
            else
                p_tex->initializeCubeMapArray(format, width, height, slice_num, mip_level_num);
            break;
        case cTextureType_2D_Array:
            p_tex->initializeArray(format, width, height, slice_num, mip_level_num);
            break;
        case cTextureType_2D_MSAA:
            p_tex->initializeMultiSample(format, width, height, multi_sample_type);
            break;
        default:
            // SEAD_ASSERT_MSG(false, "illigal texture type:%d", s32(type));
            break;
        }
    }

    p_tex->mAllocType = allocate_type;
    {
        if (p_tex->mFlag.isOnBit(0)) // Force allocation from the main allocator (cAllocateType_Main)
            allocate_type = cAllocateType_Main;
        else if (p_tex->mFlag.isOnBit(1)) // Force allocation from the sub allocator (cAllocateType_Sub1)
            allocate_type = cAllocateType_Sub1;

        const u32 allocator_num = 1;
        u32 allocator_idx_start = u32(allocate_type);
        u32 allocator_idx_end = allocator_idx_start + allocator_num;

        u32 allocator_idx = allocator_idx_start;

        for (; allocator_idx < allocator_idx_end; allocator_idx++)
        {
            p_tex->mpMemoryAllocator = &(p_ctx->mTextureMemoryAllocator[allocator_idx]);
            p_tex->mpMemoryBlock = p_tex->mpMemoryAllocator->alloc(*p_tex, pp_buffer, allocate_from_head);

            if (p_tex->mpMemoryBlock == nullptr)
                continue;

            if (isValid_(allocate_from_head ? p_ctx : nullptr))
                break;

            if (p_tex->mpMemoryBlock != nullptr) // Always true
            {
                p_tex->mpMemoryAllocator->free(p_tex->mpMemoryBlock);
                p_tex->mpMemoryBlock = nullptr;
            }
        }
    }

    p_tex->setImagePtr(p_tex->mpMemoryBlock->getImagePtr());

    void* mip_ptr = p_tex->mpMemoryBlock->getMipPtr();
    if (mip_ptr != nullptr)
        p_tex->setMipPtr(mip_ptr);

    p_tex->mAllocatorUsedSize = p_tex->mpMemoryAllocator->getUsedSize();
    p_tex->mLRUCounter = p_ctx->mTextureDataEx.size();
    p_tex->mName = name;

    if (invalidate_gpu_cache)
        p_tex->invalidateGPUCache();

    p_tex->setSurfaceSwizzle(p_tex->mSurfaceSwizzle);

    p_tex->mpContext = p_ctx;

    return p_tex;
}

void DynamicTextureAllocator::free(const TextureData* ptr)
{
    // SEAD_ASSERT(ptr != nullptr);

    if (!free_(&mContext[s32(sead::CoreInfo::getCurrentCoreId())], ptr))
    {
        // TODO: scoped lock
        mCriticalSection.lock();
        {
            for (sead::UnsafeArray<Context, cContextType_Num>::iterator itr_ctx = mContext.begin(), itr_ctx_end = --mContext.end(); itr_ctx != itr_ctx_end; ++itr_ctx)
                if (itr_ctx.getIndex() != s32(sead::CoreInfo::getCurrentCoreId()))
                    if (free_(&(*itr_ctx), ptr))
                        break;
        }
        mCriticalSection.unlock();
    }
}

bool DynamicTextureAllocator::free_(Context* p_ctx, const TextureData* ptr)
{
    // SEAD_ASSERT(ptr != nullptr);
    TextureDataEx* p_tex = static_cast<TextureDataEx*>(const_cast<TextureData*>(ptr));

    if (p_tex->mpContext != p_ctx)
        return false;

    // SEAD_ASSERT(p_tex->mpMemoryBlock != nullptr);
    p_tex->mpMemoryAllocator->free(p_tex->mpMemoryBlock);
    p_tex->mpMemoryBlock = nullptr;

    return true;
}

} }
