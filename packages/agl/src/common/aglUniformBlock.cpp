#include <common/aglUniformBlock.h>

// TODO: Move to the proper headers
#define SEAD_MACRO_UTIL_ROUNDUP(x, y) ((x) + ((y) - 1) & ~((y) - 1))
#define SEAD_MACRO_UTIL_ROUNDDOWN(x, y) ((x) & ~((y) - 1))

namespace {

static const u8 sTypeInfo[agl::UniformBlock::cType_Num][3] = {
    // Stride, Alignment, Array Stride
    { 0x01, 0x01, 0x04 },
    { 0x01, 0x01, 0x04 },
    { 0x01, 0x01, 0x04 },
    { 0x02, 0x02, 0x04 },
    { 0x03, 0x04, 0x04 },
    { 0x04, 0x04, 0x04 },
    { 0x08, 0x08, 0x08 },
    { 0x08, 0x08, 0x08 },
    { 0x08, 0x08, 0x08 },
    { 0x0C, 0x0C, 0x0C },
    { 0x0C, 0x0C, 0x0C },
    { 0x0C, 0x0C, 0x0C },
    { 0x10, 0x10, 0x10 },
    { 0x10, 0x10, 0x10 },
    { 0x10, 0x10, 0x10 }
};

}

namespace agl {

UniformBlock::UniformBlock()
    : mpHeader(nullptr)
    , mCurrentBuffer(nullptr)
    , mBlockSize(0)
    , mFlag(0)
{
}

UniformBlock::~UniformBlock()
{
    destroy();
}

void UniformBlock::startDeclare(s32 num, sead::Heap* heap)
{
    // SEAD_ASSERT(0 < num);
    // SEAD_ASSERT(mpHeader == nullptr);

    mpHeader = new (heap) Header;
    mpHeader->mpMember = new (heap) Member[num];
    mpHeader->mMemberNum = num;
    mpHeader->mMemberCount = 0;

    mBlockSize = 0;

    mFlag.set(cFlag_OwnHeader);
}

void UniformBlock::declare(Type type, s32 num)
{
    // SEAD_ASSERT(0 < num);

    Member& member = mpHeader->mpMember[mpHeader->mMemberCount];
    member.mType = type;
    member.mNum = num;

    u8 stride;

    if (member.mNum == 1)
    {
        stride = sTypeInfo[member.mType][0];
        mBlockSize = SEAD_MACRO_UTIL_ROUNDUP(mBlockSize, sTypeInfo[member.mType][1] * sizeof(u32));
    }
    else
    {
        stride = sTypeInfo[member.mType][2];
        mBlockSize = SEAD_MACRO_UTIL_ROUNDUP(mBlockSize, sTypeInfo[member.mType][2] * sizeof(u32));
    }

    member.mOffset = mBlockSize;
    mBlockSize += member.mNum * stride * sizeof(u32);

    mpHeader->mMemberCount++;
}

void UniformBlock::declare(const UniformBlock& block)
{
    // SEAD_ASSERT(block.mpHeader != nullptr);
    // SEAD_ASSERT(mpHeader == nullptr);
    // SEAD_ASSERT(mBlockSize == 0);

    mpHeader = block.mpHeader;
    mBlockSize = block.mBlockSize;

    mFlag.reset(cFlag_OwnHeader);
}

void UniformBlock::create(sead::Heap* heap)
{
    // SEAD_ASSERT(mCurrentBuffer == nullptr);
    // SEAD_ASSERT(mFlag.isOff(cFlag_OwnBuffer), "This buffer has original buffer.");

    mBlockSize = SEAD_MACRO_UTIL_ROUNDUP(mBlockSize, cCPUCacheLineSize);
    mCurrentBuffer = new (heap, cUniformBlockAlignment) u8[mBlockSize];

    mFlag.set(cFlag_OwnBuffer);
}

void UniformBlock::destroy()
{
    if (mFlag.isOn(cFlag_OwnBuffer))
    {
        if (mCurrentBuffer)
        {
            delete[] mCurrentBuffer;
            mCurrentBuffer = nullptr;
        }
        mFlag.reset(cFlag_OwnBuffer);
    }

    mCurrentBuffer = nullptr;
    mBlockSize = 0;

    if (mFlag.isOn(cFlag_OwnHeader))
    {
        delete mpHeader->mpMember; // Nintendo forgot to use delete[] again
        delete mpHeader;
        mpHeader = nullptr;
        mFlag.reset(cFlag_OwnHeader);
    }
}

void UniformBlock::dcbz() const
{
#ifdef cafe
    u8* begin_ptr = mCurrentBuffer;
    u8* end_ptr = mCurrentBuffer + SEAD_MACRO_UTIL_ROUNDDOWN(mBlockSize, cCPUCacheLineSize);

    while (begin_ptr < end_ptr)
    {
        __dcbz(begin_ptr, 0);
        begin_ptr += cCPUCacheLineSize;
    }
#else
    sead::MemUtil::fillZero(mCurrentBuffer, mBlockSize);
#endif
}

void UniformBlock::flush(void* p_memory, bool invalidate_gpu) const
{
#ifdef cafe
    DCFlushRange(p_memory, mBlockSize);
    if (invalidate_gpu)
        GX2Invalidate(GX2_INVALIDATE_UNIFORM_BLOCK, p_memory, mBlockSize);
#endif // cafe
}

void UniformBlock::flushNoSync(void* p_memory, bool invalidate_gpu) const
{
#ifdef cafe
    DCFlushRangeNoSync(p_memory, mBlockSize);
    if (invalidate_gpu)
        GX2Invalidate(GX2_INVALIDATE_UNIFORM_BLOCK, p_memory, mBlockSize);
#endif // cafe
}

bool UniformBlock::setUniform(const void* p_data, const UniformBlockLocation& location, u32 offset, size_t size) const
{
    if (!location.isValid())
        return false;

#ifdef cafe
    const u8* ptr = (const u8*)p_data + offset;

    if (location.getVertexLocation() != -1)
        GX2SetVertexUniformBlock(location.getVertexLocation(), size, ptr);

    if (location.getFragmentLocation() != -1)
        GX2SetPixelUniformBlock(location.getFragmentLocation(), size, ptr);

    if (location.getGeometryLocation() != -1)
        GX2SetGeometryUniformBlock(location.getGeometryLocation(), size, ptr);

    return true;
#else
    return false;
#endif
}

void UniformBlock::setData_(void* p_memory, s32 index, const void* p_data, s32 array_index, s32 array_num) const
{
    // SEAD_ASSERT(mpHeader != nullptr && mpHeader->mpMember != nullptr);
    // SEAD_ASSERT(0 <= index && index < mpHeader->mMemberNum);
    // SEAD_ASSERT(p_memory != nullptr);
    // SEAD_ASSERT(p_data != nullptr);

    Member& member = mpHeader->mpMember[index];

    // SEAD_ASSERT_MSG(member.mType <  6 ||
    //                 member.mType > 11, "not implemented yet.");

    u8 stride_array = sTypeInfo[member.mType][2];
    u8* ptr = (u8*)p_memory + stride_array * array_index * sizeof(u32) + member.mOffset;
    u8 stride = sTypeInfo[member.mType][0];

#ifdef cafe
    if ((uintptr_t)ptr % cCPUCacheLineSize == 0)
    {
        u32 aligned_size = SEAD_MACRO_UTIL_ROUNDDOWN(array_num * stride_array * sizeof(u32), cCPUCacheLineSize);
        for (u32 i = 0; i < aligned_size; i += cCPUCacheLineSize)
            __dcbz(ptr, i);
    }
#endif // cafe

    const u32* src = (const u32*)p_data;

    for (s32 i = 0; i < array_num; i++)
    {
#ifdef cafe
        __bytereversed u32* const dst = (__bytereversed u32*)ptr;
#else
        u32* const dst = (u32*)ptr;
#endif

        for (s32 j = 0; j < stride; j++)
            dst[j] = *src++;

        ptr += stride_array * sizeof(u32);
    }
}

}
