#include <stream/seadBinaryStreamFormat.h>
#include <stream/seadStreamSrc.h>

namespace sead {

u8 BinaryStreamFormat::readU8(StreamSrc* src, Endian::Types endian)
{
    u8 ret = 0;
    u32 rb = src->read(&ret, sizeof(u8));
    // SEAD_ASSERT(rb == sizeof( u8 ));
    return Endian::toHostU8(endian, ret);
}

u16 BinaryStreamFormat::readU16(StreamSrc* src, Endian::Types endian)
{
    u16 ret = 0;
    u32 rb = src->read(&ret, sizeof(u16));
    // SEAD_ASSERT(rb == sizeof( u16 ));
    return Endian::toHostU16(endian, ret);
}

u32 BinaryStreamFormat::readU32(StreamSrc* src, Endian::Types endian)
{
    u32 ret = 0;
    u32 rb = src->read(&ret, sizeof(u32));
    // SEAD_ASSERT(rb == sizeof( u32 ));
    return Endian::toHostU32(endian, ret);
}

u64 BinaryStreamFormat::readU64(StreamSrc* src, Endian::Types endian)
{
    u64 ret = 0;
    u32 rb = src->read(&ret, sizeof(u64));
    // SEAD_ASSERT(rb == sizeof( u64 ));
    return Endian::toHostU64(endian, ret);
}

s8 BinaryStreamFormat::readS8(StreamSrc* src, Endian::Types endian)
{
    s8 ret = 0;
    u32 rb = src->read(&ret, sizeof(s8));
    // SEAD_ASSERT(rb == sizeof( s8 ));
    return Endian::toHostS8(endian, ret);
}

s16 BinaryStreamFormat::readS16(StreamSrc* src, Endian::Types endian)
{
    s16 ret = 0;
    u32 rb = src->read(&ret, sizeof(s16));
    // SEAD_ASSERT(rb == sizeof( s16 ));
    return Endian::toHostS16(endian, ret);
}

s32 BinaryStreamFormat::readS32(StreamSrc* src, Endian::Types endian)
{
    s32 ret = 0;
    u32 rb = src->read(&ret, sizeof(s32));
    // SEAD_ASSERT(rb == sizeof( s32 ));
    return Endian::toHostS32(endian, ret);
}

s64 BinaryStreamFormat::readS64(StreamSrc* src, Endian::Types endian)
{
    s64 ret = 0;
    u32 rb = src->read(&ret, sizeof(s64));
    // SEAD_ASSERT(rb == sizeof( s64 ));
    return Endian::toHostS64(endian, ret);
}

f32 BinaryStreamFormat::readF32(StreamSrc* src, Endian::Types endian)
{
    u32 ret = 0;
    u32 rb = src->read(&ret, sizeof(u32));
    // SEAD_ASSERT(rb == sizeof( u32 ));
    return Endian::toHostF32(endian, &ret);
}

void BinaryStreamFormat::readBit(StreamSrc* src, void* data, u32 bitnum)
{
    u8* data8 = static_cast<u8*>(data);
    u32 bytenum = bitnum / 8;
    u32 rb = src->read(data8, bytenum);
    // SEAD_ASSERT(rb == bytenum);
    bitnum -= bytenum * 8;
    if (bitnum > 0)
    {
        u8 buffer;
        rb = src->read(&buffer, 1);
        // SEAD_ASSERT(rb == 1);
        u8 mask = 0xff << (bitnum & 0xff);
        data8[bytenum] &= mask;
        data8[bytenum] |= buffer & ~mask;
    }
}

void BinaryStreamFormat::readString(StreamSrc* src, BufferedSafeString* dst, u32 size)
{
    u32 remain_size = 0;
    if (dst->getBufferSize() < size)
    {
        remain_size = size - dst->getBufferSize();
        size = dst->getBufferSize();
        // SEAD_WARNING(false, "not enough buffer length. drop %u char(s).", remain_size);
    }
    static_cast<void>(dst->cstr()); // Forces null termination
    u32 rb = src->read(dst->getBuffer(), size);
    // SEAD_ASSERT(rb == size);
    if (size + 1 < dst->getBufferSize())
    {
        dst->trim(size);
    }
    else
    {
        dst->trim(dst->getBufferSize() - 1);
    }
    if (remain_size > 0)
    {
        // IDK why they are doing this
        rb = src->read(dst->getBuffer(), remain_size);
        // SEAD_ASSERT(rb == remain_size);
    }
}

u32 BinaryStreamFormat::readMemBlock(StreamSrc* src, void* dst, u32 size)
{
    return src->read(dst, size);
}

void BinaryStreamFormat::writeU8(StreamSrc* src, Endian::Types endian, u8 value)
{
    value = Endian::fromHostU8(endian, value);
    u32 wb = src->write(&value, sizeof(u8));
    // SEAD_ASSERT(wb == sizeof( u8 ));
}

void BinaryStreamFormat::writeU16(StreamSrc* src, Endian::Types endian, u16 value)
{
    value = Endian::fromHostU16(endian, value);
    u32 wb = src->write(&value, sizeof(u16));
    // SEAD_ASSERT(wb == sizeof( u16 ));
}

void BinaryStreamFormat::writeU32(StreamSrc* src, Endian::Types endian, u32 value)
{
    value = Endian::fromHostU32(endian, value);
    u32 wb = src->write(&value, sizeof(u32));
    // SEAD_ASSERT(wb == sizeof( u32 ));
}

void BinaryStreamFormat::writeU64(StreamSrc* src, Endian::Types endian, u64 value)
{
    value = Endian::fromHostU64(endian, value);
    u32 wb = src->write(&value, sizeof(u64));
    // SEAD_ASSERT(wb == sizeof( u64 ));
}

void BinaryStreamFormat::writeS8(StreamSrc* src, Endian::Types endian, s8 value)
{
    value = Endian::fromHostS8(endian, value);
    u32 wb = src->write(&value, sizeof(s8));
    // SEAD_ASSERT(wb == sizeof( s8 ));
}

void BinaryStreamFormat::writeS16(StreamSrc* src, Endian::Types endian, s16 value)
{
    value = Endian::fromHostS16(endian, value);
    u32 wb = src->write(&value, sizeof(s16));
    // SEAD_ASSERT(wb == sizeof( s16 ));
}

void BinaryStreamFormat::writeS32(StreamSrc* src, Endian::Types endian, s32 value)
{
    value = Endian::fromHostS32(endian, value);
    u32 wb = src->write(&value, sizeof(s32));
    // SEAD_ASSERT(wb == sizeof( s32 ));
}

void BinaryStreamFormat::writeS64(StreamSrc* src, Endian::Types endian, s64 value)
{
    value = Endian::fromHostS64(endian, value);
    u32 wb = src->write(&value, sizeof(s64));
    // SEAD_ASSERT(wb == sizeof( s64 ));
}

void BinaryStreamFormat::writeF32(StreamSrc* src, Endian::Types endian, f32 value)
{
    u32 u_value = Endian::fromHostF32(endian, &value);
    u32 wb = src->write(&u_value, sizeof(u32));
    // SEAD_ASSERT(wb == sizeof( u32 ));
}

void BinaryStreamFormat::writeBit(StreamSrc* src, const void* data, u32 bitnum)
{
    const u8* data8 = static_cast<const u8*>(const_cast<void*>(data));
    u8 bytenum = bitnum / 8;
    u32 wb = src->write(data8, bytenum);
    // SEAD_ASSERT(wb == bytenum);
    bitnum -= bytenum * 8;
    if (bitnum > 0)
    {
        wb = src->write(&data8[bytenum], 1);
        // SEAD_ASSERT(wb == 1);
    }
}

void BinaryStreamFormat::writeString(StreamSrc* src, const SafeString& str, u32 size)
{
    u32 str_size = str.calcLength();
    if (size < str_size)
    {
        str_size = size;
    }
    u32 wb = src->write(str.cstr(), str_size);
    // SEAD_ASSERT(wb == str_size);
    char nullchar = '\0';
    for (; str_size < size; str_size++)
    {
        wb += src->write(&nullchar, 1);
    }
    // SEAD_ASSERT(wb == size);
}

void BinaryStreamFormat::writeMemBlock(StreamSrc* src, const void* data, u32 size)
{
    u32 wb = src->write(data, size);
    // SEAD_ASSERT(wb == size);
}

void BinaryStreamFormat::skip(StreamSrc* src, u32 size)
{
    src->skip(size);
}

void BinaryStreamFormat::rewind(StreamSrc* src)
{
    src->rewind();
}

} // namespace sead
