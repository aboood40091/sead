#ifndef SEAD_STREAM_H_
#define SEAD_STREAM_H_

#include <prim/seadEndian.h>
#include <prim/seadSafeString.h>

namespace sead {

class StreamFormat;
class StreamSrc;

class Stream
{
public:
    enum Modes
    {
        cBinary = 0,
        cText   = 1,
        cModeNum
    };

public:
    Stream();
    virtual ~Stream() { }

    void skip(u32 size);
    void skip(u32 size, u32 count);

    bool isEOF();

    void setBinaryEndian(Endian::Types types);
    void setMode(Modes mode);
    void setUserFormat(StreamFormat* format);

protected:
    StreamFormat*   mFormat;
    StreamSrc*      mSrcStream;
    Endian::Types   mSrcEndian;

private:
    static StreamFormat* const BASIC_STREAM_FORMAT[cModeNum];
};

class ReadStream : public Stream
{
public:
    ReadStream()
    {
    }

    virtual ~ReadStream()
    {
    }

    u8 readU8();
    u16 readU16();
    u32 readU32();
    u64 readU64();

    s8 readS8();
    s16 readS16();
    s32 readS32();
    s64 readS64();

    f32 readF32();

    void readU8(u8&);
    void readU16(u16&);
    void readU32(u32&);
    void readU64(u64&);

    void readS8(s8&);
    void readS16(s16&);
    void readS32(s32&);
    void readS64(s64&);

    void readF32(f32&);

    void readBit(void*, u32);
    void readString(BufferedSafeString* dst, u32 size);
    void readMemBlock(void*, u32);

protected:
    f32 readF32BitImpl_(u32 exp_bitnum, u32 significand_bitnum);
    f32 readF64BitImpl_(u32 exp_bitnum, u32 significand_bitnum);

    static const u32 cExpBitMax = 8;
    static const u32 cSignficandBitMax = 23;
};

} // namespace sead

#endif // SEAD_STREAM_H_
