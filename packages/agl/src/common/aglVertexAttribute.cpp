#include <common/aglVertexAttribute.h>
#include <common/aglVertexBuffer.h>

namespace agl {

VertexAttribute::VertexAttribute()
    : mSetupFinish(false)
    , mCreateFinish(false)
    , mpFetchShaderBuf(nullptr)
{
}

VertexAttribute::~VertexAttribute()
{
    destroy();
}

void VertexAttribute::create(u32 buffer_max, sead::Heap* heap)
{
    // SEAD_ASSERT(0 < buffer_max && buffer_max <= cVertexAttributeMax);
    mVertexBuffer.allocBuffer(buffer_max, heap);

    mpFetchShaderBuf = new (heap, GX2_SHADER_ALIGNMENT) u8[GX2CalcFetchShaderSize(cVertexAttributeMax)];

    mCreateFinish = true;

    cleanUp();
}

void VertexAttribute::cleanUp()
{
    for (s32 i = 0; i < cVertexAttributeMax; i++)
        mAttribute[i].mpVertexBuffer = nullptr;

    for (s32 i = 0; i < mVertexBuffer.size(); i++)
        mVertexBuffer[i] = nullptr;

    mSetupFinish = false;
}

void VertexAttribute::destroy()
{
    if (!mCreateFinish)
        return;

    mVertexBuffer.freeBuffer();

    if (mpFetchShaderBuf)
    {
        delete[] mpFetchShaderBuf;
        mpFetchShaderBuf = nullptr;
    }

    mCreateFinish = false;
    mSetupFinish = false;
}

void VertexAttribute::setVertexStream(s16 location, const VertexBuffer* buffer, u32 stream_index)
{
    // SEAD_ASSERT(mCreateFinish);
    // SEAD_ASSERT(buffer->isEnable( stream_index ));

    if (location == -1)
        return;

    if (mAttribute[location].mpVertexBuffer != nullptr)
        mAttribute[location].mBufferIndex = disableVertexBuffer_(&(mAttribute[location]));

    if (buffer != nullptr)
        mAttribute[location].mBufferIndex = enableVertexBuffer_(&(mAttribute[location]), buffer, stream_index);
}

s32 VertexAttribute::enableVertexBuffer_(Attribute_* attr, const VertexBuffer* buffer, u32 stream_index)
{
    // SEAD_ASSERT(attr->mpVertexBuffer == nullptr);

    attr->mpVertexBuffer = buffer;
    attr->mStreamIndex = stream_index;

    s32 null_buffer_index = -1;

    for (s32 i = mVertexBuffer.size() - 1; i >= 0; i--)
    {
        if (mVertexBuffer[i] == attr->mpVertexBuffer)
            return i;

        if (mVertexBuffer[i] == nullptr)
            null_buffer_index = i;
    }

    // SEAD_ASSERT(null_buffer_index != -1);
    mVertexBuffer[null_buffer_index] = buffer;
    return null_buffer_index;
}

s32 VertexAttribute::disableVertexBuffer_(Attribute_* attr)
{
    const VertexBuffer* buffer = attr->mpVertexBuffer;

    attr->mpVertexBuffer = nullptr;
    attr->mStreamIndex = 1;

    for (s32 i = 0; i < cVertexAttributeMax; i++)
        if (buffer == mAttribute[i].mpVertexBuffer)
            return -1;

    mVertexBuffer[attr->mBufferIndex] = nullptr;
    return -1;
}

void VertexAttribute::setUp()
{
    // SEAD_ASSERT(mCreateFinish);

    sead::UnsafeArray<GX2AttribStream, cVertexAttributeMax> stream_array;
    u32 stream_num = 0;

    for (s32 i = 0; i < cVertexAttributeMax; i++)
    {
        const Attribute_* attr = &(mAttribute[i]);
        GX2AttribStream* stream = &(stream_array[i]);

        if (attr->mpVertexBuffer != nullptr)
        {
            stream->buffer = attr->mBufferIndex;
            stream->offset = attr->mpVertexBuffer->getStreamOffset(attr->mStreamIndex);
            stream->location = i;
            stream->format = GX2AttribFormat(attr->mpVertexBuffer->getStreamFormat(attr->mStreamIndex));
            stream->destSel = attr->mpVertexBuffer->getStreamCompSel(attr->mStreamIndex);
            stream->indexType = attr->mpVertexBuffer->getStreamIndexType(attr->mStreamIndex);
            stream->aluDivisor = attr->mpVertexBuffer->getStreamDivisor(attr->mStreamIndex);
            stream->endianSwap = attr->mpVertexBuffer->getStreamEndianSwap(attr->mStreamIndex);

            stream_num++;
        }
    }

    GX2InitFetchShader(&mFetchShader, mpFetchShaderBuf, stream_num, stream_array.getBufferPtr());
    DCFlushRangeNoSync(mpFetchShaderBuf, GX2CalcFetchShaderSize(stream_num));

    mSetupFinish = true;
}

void VertexAttribute::activate() const
{
    // SEAD_ASSERT(mSetupFinish);

    GX2SetFetchShader(&mFetchShader);

    for (s32 i = 0; i < mVertexBuffer.size(); i++)
    {
        const VertexBuffer* buffer = mVertexBuffer[i];
        if (buffer != nullptr)
            GX2SetAttribBuffer(i, buffer->getBufferByteSize(), buffer->getStride(), buffer->getBufferPtr());
    }
}

}
