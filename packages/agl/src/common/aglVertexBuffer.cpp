#include <common/aglVertexBuffer.h>

namespace agl {

VertexBuffer::VertexBuffer()
    : mpBuffer(nullptr)
    , mStride(0)
    , mVertexNum(0)
    , mBufferByteSize(0)
{
}

VertexBuffer::~VertexBuffer()
{
    cleanUp_();
}

void VertexBuffer::cleanUp_()
{
    for (u32 i = 0; i < cVertexStreamMax; i++)
        mStream[i].mEnable = false;

    mpBuffer = nullptr;
    mBufferByteSize = 0;
    mStride = 0;
    mVertexNum = 0;
}

void VertexBuffer::setUpBuffer(const void* buffer, u32 stride, u32 buffer_byte_size)
{
    cleanUp_();

    // SEAD_ASSERT(buffer != nullptr);
    // SEAD_ASSERT(buffer_byte_size != 0);

    mpBuffer = buffer;
    mStride = stride;

    if (stride == 0)
    {
        mVertexNum = 1;
        mBufferByteSize = buffer_byte_size;
    }
    else
    {
        mVertexNum = buffer_byte_size / stride;
        mBufferByteSize = stride * mVertexNum;
    }

    // SEAD_ASSERT(buffer_byte_size == mBufferByteSize);

    DCFlushRangeNoSync(mpBuffer, mBufferByteSize);
}

void VertexBuffer::setUpStream(s32 index, VertexStreamFormat format, u32 offset)
{
    mStream[index].mFormat = format;
    mStream[index].mOffset = offset;
    mStream[index].mEnable = true;

    const GX2CompSel comp_sel[] = {
        GX2_COMP_SEL_X001, GX2_COMP_SEL_XY01,
        GX2_COMP_SEL_X001, GX2_COMP_SEL_X001,
        GX2_COMP_SEL_XY01, GX2_COMP_SEL_X001,
        GX2_COMP_SEL_X001, GX2_COMP_SEL_XY01,
        GX2_COMP_SEL_XY01, GX2_COMP_SEL_XYZ1,
        GX2_COMP_SEL_XYZW, GX2_COMP_SEL_XYZW,
        GX2_COMP_SEL_XY01, GX2_COMP_SEL_XY01,
        GX2_COMP_SEL_XYZW, GX2_COMP_SEL_XYZW,
        GX2_COMP_SEL_XYZ1, GX2_COMP_SEL_XYZ1,
        GX2_COMP_SEL_XYZW, GX2_COMP_SEL_XYZW
    };

    mStream[index].mCompSel = comp_sel[format & 0xff];
    mStream[index].mDivisor = 0;
    mStream[index].mEndianSwap = GX2_ENDIANSWAP_DEFAULT;
    mStream[index].mIndexType = GX2_ATTRIB_INDEX_VERTEX_ID;
}

}
