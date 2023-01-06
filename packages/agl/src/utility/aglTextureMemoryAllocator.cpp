#include <utility/aglTextureMemoryAllocator.h>

namespace agl { namespace utl {

TextureMemoryAllocator::MemoryBlock* TextureMemoryAllocator::alloc(const TextureData& tex, void** pp_buffer, bool allocate_from_head)
{
    if (!mpBufferStart)
        return nullptr;

    AllocArg arg;
    arg.mImageSize = tex.getImageByteSize();
    arg.mMipSize = tex.getMipLevelNum() > 1 ? tex.getMipByteSize() : 0;
    arg.mAlignment = tex.getAlignment();
    arg.mppTargetBuffer = nullptr;
    arg.mTargetBufferSize = 0;
    arg.mTargetBufferAlignment = sizeof(void*);

    if (tex.getTextureFormat() >= cTextureFormat_Depth_16 && tex.getTextureFormat() <= cTextureFormat_Depth_24_uNorm_Stencil_8)
    {
        if (pp_buffer != nullptr)
        {
            mDepthTarget.applyTextureData(tex);

            arg.mppTargetBuffer = pp_buffer;
            arg.mTargetBufferAlignment = mDepthTarget.getHiZBufferAlign();
            arg.mTargetBufferSize = mDepthTarget.getHiZBufferSize();
        }
    }
    else
    {
        if (tex.getMultiSampleType() != cMultiSampleType_1x)
        {
            mColorTarget.applyTextureData(tex);

            arg.mppTargetBuffer = pp_buffer;
            arg.mTargetBufferAlignment = mColorTarget.getAuxBufferAlign();
            arg.mTargetBufferSize = mColorTarget.getAuxBufferSize();
        }
    }

    const u32 alignment = sead::Mathu::max(arg.mAlignment, arg.mTargetBufferAlignment);

    arg.mSize  = sead::Mathu::roundUpPow2(arg.mImageSize, arg.mAlignment);
    arg.mSize += sead::Mathu::roundUpPow2(arg.mMipSize, arg.mTargetBufferAlignment);
    arg.mSize += arg.mTargetBufferSize;
    arg.mSize += alignment;

    if (allocate_from_head)
    {
        MemoryBlock* p_memory = mMemoryBlockFree.front();
        while (p_memory != nullptr)
        {
            if (alloc_(p_memory, arg, true))
                return p_memory;

            p_memory = mMemoryBlockFree.next(p_memory);
        }
    }
    else
    {
        MemoryBlock* p_memory = mMemoryBlockFree.back();
        while (p_memory != nullptr)
        {
            if (alloc_(p_memory, arg, false))
                return p_memory;

            p_memory = mMemoryBlockFree.prev(p_memory);
        }
    }

    return nullptr;
}

bool TextureMemoryAllocator::alloc_(MemoryBlock* p_memory, const AllocArg& arg, bool allocate_from_head)
{
    // SEAD_ASSERT(p_memory != nullptr);

    if (arg.mSize >= p_memory->mSize)
    {
        if (arg.mSize > p_memory->mSize)
            return false;
    }
    else
    {
        MemoryBlock* p_new_free = mMemoryBlockEmpty.popBack();
        // SEAD_ASSERT(p_new_free != nullptr);
        p_new_free->mSize = p_memory->mSize - arg.mSize;

        if (allocate_from_head)
        {
            p_new_free->mpBuffer = p_memory->mpBuffer + arg.mSize;

            mMemoryBlockFree.insertAfter(p_memory, p_new_free);
        }
        else
        {
            p_new_free->mpBuffer = p_memory->mpBuffer;
            p_memory->mpBuffer = p_memory->mpBuffer + p_new_free->mSize;

            mMemoryBlockFree.insertBefore(p_memory, p_new_free);
        }
    }

    p_memory->mSize = arg.mSize;

    if (mpDebugHeap == nullptr)
    {
        p_memory->mpBufferFromDebugHeap = nullptr;
        p_memory->mpImagePtr = (u8*)sead::Mathu::roundUpPow2((uintptr_t)p_memory->mpBuffer, arg.mAlignment);
    }
    else
    {
        u8* p_buffer_from_debug_heap = new (mpDebugHeap, arg.mAlignment) u8[p_memory->mSize];
        p_memory->mpBufferFromDebugHeap = p_buffer_from_debug_heap;
        DCFlushRange(p_buffer_from_debug_heap, p_memory->mSize);
        p_memory->mpImagePtr = (u8*)sead::Mathu::roundUpPow2((uintptr_t)p_buffer_from_debug_heap, arg.mAlignment);
    }

    u8* p_buffer = p_memory->mpImagePtr + arg.mImageSize;

    if (arg.mMipSize > 0)
    {
        p_memory->mpMipPtr = (u8*)sead::Mathu::roundUpPow2((uintptr_t)p_buffer, arg.mAlignment);
        p_buffer = p_memory->mpMipPtr + arg.mMipSize;
    }
    else
    {
        p_memory->mpMipPtr = nullptr;
    }

    if (arg.mppTargetBuffer != nullptr)
        *arg.mppTargetBuffer = (u8*)sead::Mathu::roundUpPow2((uintptr_t)p_buffer, arg.mTargetBufferAlignment);

    mMemoryBlockFree.erase(p_memory);
    mMemoryBlockUsed.pushBack(p_memory);
    mUsedSize += p_memory->mSize;

    return true;
}

void TextureMemoryAllocator::free(MemoryBlock* p_memory)
{
    // SEAD_ASSERT(p_memory != nullptr);

    mUsedSize -= p_memory->mSize;

    if (p_memory->mpBufferFromDebugHeap)
    {
        delete[] p_memory->mpBufferFromDebugHeap;
        p_memory->mpBufferFromDebugHeap = nullptr;
    }

    mMemoryBlockUsed.erase(p_memory);

    if (!mMemoryBlockFree.front())
    {
        mMemoryBlockFree.pushFront(p_memory);
        return;
    }

    for (sead::OffsetList<MemoryBlock>::iterator itr = mMemoryBlockFree.begin(), itr_end = mMemoryBlockFree.end(); itr != itr_end; ++itr)
    {
        MemoryBlock* p_temp_memory = &(*itr);

        if (p_memory->mpBuffer + p_memory->mSize <= p_temp_memory->mpBuffer)
        {
            mMemoryBlockFree.insertBefore(p_temp_memory, p_memory);

            if (p_memory->mpBuffer + p_memory->mSize == p_temp_memory->mpBuffer)
            {
                p_memory->mSize += p_temp_memory->mSize;
                mMemoryBlockFree.erase(p_temp_memory);
                mMemoryBlockEmpty.pushBack(p_temp_memory);
            }

            MemoryBlock* p_prev = mMemoryBlockFree.prev(p_memory);
            if (p_prev && p_prev->mpBuffer + p_prev->mSize == p_memory->mpBuffer)
            {
                p_memory->mpBuffer = p_prev->mpBuffer;
                p_memory->mSize += p_prev->mSize;
                mMemoryBlockFree.erase(p_prev);
                mMemoryBlockEmpty.pushBack(p_prev);
            }

            break;
        }
    }

    if (!p_memory->mListNode.isLinked())
    {
        mMemoryBlockFree.pushBack(p_memory);

        MemoryBlock* p_prev = mMemoryBlockFree.prev(p_memory);
        if (p_prev && p_prev->mpBuffer + p_prev->mSize == p_memory->mpBuffer)
        {
            p_memory->mpBuffer = p_prev->mpBuffer;
            p_memory->mSize += p_prev->mSize;
            mMemoryBlockFree.erase(p_prev);
            mMemoryBlockEmpty.pushBack(p_prev);
        }
    }

    for (sead::OffsetList<MemoryBlock>::iterator itr = mMemoryBlockFree.begin(), itr_end = mMemoryBlockFree.end(); itr != itr_end; ++itr)
    {
        // Ok...
    }
}

bool TextureMemoryAllocator::isOverwrapperd(const TextureMemoryAllocator& allocator) const
{
    // SEAD_ASSERT(&allocator != this);

    const void* buffer1 = nullptr;
    const void* buffer2 = nullptr;

    // Mismatches if ternary operator not used

    buffer2 = mMemoryBlockFree.back()
        ? mMemoryBlockFree.back()->mpBuffer
        : nullptr;

    buffer1 = allocator.mMemoryBlockFree.front()
        ? allocator.mMemoryBlockFree.front()->mpBuffer + allocator.mMemoryBlockFree.back()->mSize
        : nullptr;

    return buffer1 < buffer2;
}

} }
