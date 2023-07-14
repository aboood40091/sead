#ifndef SEAD_BINARY_STREAM_FORMAT_H_
#define SEAD_BINARY_STREAM_FORMAT_H_

#include <stream/seadStreamFormat.h>

namespace sead {

class BinaryStreamFormat : public StreamFormat
{
public:
    u8 readU8(StreamSrc* src, Endian::Types endian) override;
    u16 readU16(StreamSrc* src, Endian::Types endian) override;
    u32 readU32(StreamSrc* src, Endian::Types endian) override;
    u64 readU64(StreamSrc* src, Endian::Types endian) override;
    s8 readS8(StreamSrc* src, Endian::Types endian) override;
    s16 readS16(StreamSrc* src, Endian::Types endian) override;
    s32 readS32(StreamSrc* src, Endian::Types endian) override;
    s64 readS64(StreamSrc* src, Endian::Types endian) override;
    f32 readF32(StreamSrc* src, Endian::Types endian) override;
    void readBit(StreamSrc* src, void* data, u32 bitnum) override;
    void readString(StreamSrc* src, BufferedSafeString* dst, u32 size) override;
    u32 readMemBlock(StreamSrc* src, void* dst, u32 size) override;
    void writeU8(StreamSrc* src, Endian::Types endian, u8 value) override;
    void writeU16(StreamSrc* src, Endian::Types endian, u16 value) override;
    void writeU32(StreamSrc* src, Endian::Types endian, u32 value) override;
    void writeU64(StreamSrc* src, Endian::Types endian, u64 value) override;
    void writeS8(StreamSrc* src, Endian::Types endian, s8 value) override;
    void writeS16(StreamSrc* src, Endian::Types endian, s16 value) override;
    void writeS32(StreamSrc* src, Endian::Types endian, s32 value) override;
    void writeS64(StreamSrc* src, Endian::Types endian, s64 value) override;
    void writeF32(StreamSrc* src, Endian::Types endian, f32 value) override;
    void writeBit(StreamSrc* src, const void* data, u32 bitnum) override;
    void writeString(StreamSrc* src, const SafeString& str, u32 size) override;
    void writeMemBlock(StreamSrc* src, const void* data, u32 size) override;
    void writeDecorationText(StreamSrc* src, const SafeString&) override { }
    void writeNullChar(StreamSrc* src) override { }
    void skip(StreamSrc* src, u32 size) override;
    void flush(StreamSrc* src) override { }
    void rewind(StreamSrc* src) override;
};

} // namespace sead

#endif // SEAD_BINARY_STREAM_FORMAT_H_
