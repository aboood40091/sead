#include <stream/seadBinaryStreamFormat.h>
#include <stream/seadStream.h>
#include <stream/seadStreamSrc.h>

namespace sead {

BinaryStreamFormat sBinaryStreamInstance;
//TextStreamFormat sTextStreamInstance; // TODO

StreamFormat* const Stream::BASIC_STREAM_FORMAT[Stream::cModeNum] = {
    &sBinaryStreamInstance,
  //&sTextStreamInstance
};

Stream::Stream()
    : mFormat(nullptr)
    , mSrcStream(nullptr)
    , mSrcEndian(Endian::getHostEndian())
{
}

void Stream::skip(u32 size)
{
    // SEAD_ASSERT(mFormat);
    // SEAD_ASSERT(mSrcStream);
    mFormat->skip(mSrcStream, size);
}

bool Stream::isEOF()
{
    // SEAD_ASSERT(mSrcStream);
    return mSrcStream->isEOF();
}

void Stream::setBinaryEndian(Endian::Types types)
{
    mSrcEndian = types;
}

void Stream::setMode(Modes mode)
{
    // SEAD_ASSERT(0 <= mode && mode < cModeNum);
    setUserFormat(BASIC_STREAM_FORMAT[mode]);
}

void Stream::setUserFormat(StreamFormat* format)
{
    mFormat = format;
}

u32 ReadStream::readU32()
{
    // SEAD_ASSERT(mFormat);
    // SEAD_ASSERT(mSrcStream);
    return mFormat->readU32(mSrcStream, mSrcEndian);
}

s32 ReadStream::readS32()
{
    // SEAD_ASSERT(mFormat);
    // SEAD_ASSERT(mSrcStream);
    return mFormat->readS32(mSrcStream, mSrcEndian);
}

f32 ReadStream::readF32()
{
    // SEAD_ASSERT(mFormat);
    // SEAD_ASSERT(mSrcStream);
    return mFormat->readF32(mSrcStream, mSrcEndian);
}

void ReadStream::readString(BufferedSafeString* dst, u32 size)
{
    // SEAD_ASSERT(mFormat);
    // SEAD_ASSERT(mSrcStream);
    // SEAD_ASSERT(static_cast<u32>( dst->getBufferSize() ) >= size);
    mFormat->readString(mSrcStream, dst, size);
}

} // namespace sead
