#include <common/aglIndexStream.h>

namespace agl {

IndexStream::IndexStream()
    : mFormat(cIndexStreamFormat_u16)
    , mPrimitiveType(GX2_PRIMITIVE_TRIANGLES)
    , mpBuffer(nullptr)
    , mCount(0)
    , mStride(0)
{
}

IndexStream::~IndexStream()
{
    cleanUp_();
}

void IndexStream::cleanUp_()
{
    mpBuffer = nullptr;
    mCount = 0;
    mStride = 0;
}

void IndexStream::setUpStream_(const void* addr, IndexStreamFormat format, u32 count)
{
    // SEAD_ASSERT(count != 0);
    // SEAD_ASSERT(addr != nullptr);

    cleanUp_();

    mpBuffer = addr;
    mCount = count;
    mFormat = format;
    mStride = mFormat == cIndexStreamFormat_u16 ? sizeof(u16) : sizeof(u32);

    DCFlushRangeNoSync(mpBuffer, mStride * mCount);
}

}
