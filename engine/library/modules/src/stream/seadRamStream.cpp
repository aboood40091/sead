#include <prim/seadMemUtil.h>
#include <stream/seadRamStream.h>

namespace sead {

RamStreamSrc::RamStreamSrc(void* start, u32 size)
    : mStartAddr(static_cast<u8*>(start))
    , mSize(size)
    , mCurPos(0)
{
}

RamStreamSrc::~RamStreamSrc()
{
}

u32 RamStreamSrc::skip(s32 byte)
{
    if (byte > 0 && mCurPos + byte > mSize)
        byte = mSize - mCurPos;

    if (byte < 0 && s32(mCurPos) < -byte)
        byte = -s32(mCurPos);

    mCurPos += byte;
    return byte;
}

u32 RamStreamSrc::read(void* dst, u32 size)
{
    if (mCurPos + size > mSize)
        size = mSize - mCurPos;

    sead::MemUtil::copy(dst, mStartAddr + mCurPos, size);
    mCurPos += size;
    return size;
}

u32 RamStreamSrc::write(const void* src, u32 size)
{
    if (mCurPos + size > mSize)
        size = mSize - mCurPos;

    sead::MemUtil::copy(mStartAddr + mCurPos, src, size);
    mCurPos += size;
    return size;
}

RamReadStream::RamReadStream(const void* start, u32 size, Modes mode)
    : mRamStreamSrc(const_cast<void*>(start), size)
{
    setSrcStream_(&mRamStreamSrc);
    setMode(mode);
}

} // namespace sead
