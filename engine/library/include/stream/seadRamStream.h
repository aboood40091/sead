#ifndef SEAD_RAM_STREAM_H_
#define SEAD_RAM_STREAM_H_

#include <stream/seadStream.h>
#include <stream/seadStreamSrc.h>

namespace sead {

class RamStreamSrc : public StreamSrc
{
public:
    RamStreamSrc(void* start, u32 size);
    ~RamStreamSrc();

    u32 skip(s32 byte) override;
    u32 read(void* dst, u32 size) override;
    u32 write(const void* src, u32 size) override;
    void rewind() override { mCurPos = 0; }
    bool isEOF() override { return mCurPos >= mSize; }

    u8* getCurrentAddres() const { return mStartAddr + mCurPos; }
    u32 getCurrentPosition() const { return mCurPos; }

protected:
    u8* mStartAddr;
    u32 mSize;
    u32 mCurPos;
};
#ifdef cafe
static_assert(sizeof(RamStreamSrc) == 0x10);
#endif // cafe

class RamReadStream : public ReadStream
{
public:
    RamReadStream(const void* start, u32 size, Modes mode);

    u8* getCurrentAddres() const { return mRamStreamSrc.getCurrentAddres(); }
    u32 getCurrentPosition() const { return mRamStreamSrc.getCurrentPosition(); }

protected:
    RamStreamSrc    mRamStreamSrc;
};
#ifdef cafe
static_assert(sizeof(RamReadStream) == 0x20);
#endif // cafe

} // namespace sead

#endif // SEAD_RAM_STREAM_H_
