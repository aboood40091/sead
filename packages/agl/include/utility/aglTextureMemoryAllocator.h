#pragma once

#include <common/aglRenderTarget.h>
#include <container/seadBuffer.h>
#include <container/seadOffsetList.h>
#include <heap/seadHeap.h>

namespace agl { namespace utl {

class TextureMemoryAllocator
{
    struct AllocArg
    {
        u32 mSize;
        u32 mImageSize;
        u32 mMipSize;
        u32 mAlignment;
        void** mppTargetBuffer;
        u32 mTargetBufferSize;
        u32 mTargetBufferAlignment;
    };
    static_assert(sizeof(AllocArg) == 0x1C, "agl::utl::TextureMemoryAllocator::AllocArg size mismatch");

public:
    class MemoryBlock
    {
    public:
        MemoryBlock(); // TODO

        void* getImagePtr() const { return mpImagePtr; }
        void* getMipPtr() const { return mpMipPtr; }

    private:
        u8* mpBuffer;
        u32 mSize;
        u8* mpBufferFromDebugHeap;
        u8* mpImagePtr;
        u8* mpMipPtr;
        sead::ListNode mListNode;

        friend class TextureMemoryAllocator;
    };
    static_assert(sizeof(MemoryBlock) == 0x1C, "agl::utl::TextureMemoryAllocator::MemoryBlock size mismatch");

public:
    TextureMemoryAllocator();
    virtual ~TextureMemoryAllocator();

    u32 getUsedSize() const { return mUsedSize; }

    MemoryBlock* alloc(const TextureData& tex, void** pp_buffer = nullptr, bool allocate_from_head = true);
    void free(MemoryBlock* p_memory);

    bool isOverwrapperd(const TextureMemoryAllocator& allocator) const;

private:
    bool alloc_(MemoryBlock* p_memory, const AllocArg& arg, bool allocate_from_head);

private:
    void* mpBufferStart;
    void* mpBufferEnd;
    u32 mUsedSize; // I think
    sead::Buffer<MemoryBlock> mMemoryBlock;
    sead::OffsetList<MemoryBlock> mMemoryBlockUsed;
    sead::OffsetList<MemoryBlock> mMemoryBlockFree;
    sead::OffsetList<MemoryBlock> mMemoryBlockEmpty;
    RenderTargetColor mColorTarget;
    RenderTargetDepth mDepthTarget;
    sead::Heap* mpDebugHeap;
};
static_assert(sizeof(TextureMemoryAllocator) == 0x300, "agl::utl::TextureMemoryAllocator size mismatch");

} }
