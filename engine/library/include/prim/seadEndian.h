#ifndef SEAD_ENDIAN_H_
#define SEAD_ENDIAN_H_

#include <basis/seadTypes.h>

namespace sead {

class Endian
{
public:
    enum Types
    {
        cBig    = 0,
        cLittle = 1
    };

public:
    static u8  swapU8 (u8  x) { return cConvFuncTable.conv8 [1](x); }
    static u16 swapU16(u16 x) { return cConvFuncTable.conv16[1](x); }
    static u32 swapU32(u32 x) { return cConvFuncTable.conv32[1](x); }
    static u64 swapU64(u64 x) { return cConvFuncTable.conv64[1](x); }

    static s8  swapS8 (s8  x) { return cConvFuncTable.conv8 [1](x); }
    static s16 swapS16(s16 x) { return cConvFuncTable.conv16[1](x); }
    static s32 swapS32(s32 x) { return cConvFuncTable.conv32[1](x); }
    static s64 swapS64(s64 x) { return cConvFuncTable.conv64[1](x); }

    static f32 swapF32(f32 x)
    {
        union { u32* ui; f32*  f; } x_ = { . f = &x };
        *x_.ui = cConvFuncTable.conv32[1](*x_.ui);
        return *x_. f;
    }

    static u8  convertU8 (Types from, Types to, u8  x) { return cConvFuncTable.conv8 [from ^ to](x); }
    static u16 convertU16(Types from, Types to, u16 x) { return cConvFuncTable.conv16[from ^ to](x); }
    static u32 convertU32(Types from, Types to, u32 x) { return cConvFuncTable.conv32[from ^ to](x); }
    static u64 convertU64(Types from, Types to, u64 x) { return cConvFuncTable.conv64[from ^ to](x); }

    static s8  convertS8 (Types from, Types to, s8  x) { return cConvFuncTable.conv8 [from ^ to](x); }
    static s16 convertS16(Types from, Types to, s16 x) { return cConvFuncTable.conv16[from ^ to](x); }
    static s32 convertS32(Types from, Types to, s32 x) { return cConvFuncTable.conv32[from ^ to](x); }
    static s64 convertS64(Types from, Types to, s64 x) { return cConvFuncTable.conv64[from ^ to](x); }

    static u32 convertF32(Types from, Types to, const void* x)
    {
        union { const u32* ui; const f32*  f; } x_ = { . f = static_cast<const f32*>(x) };
        return cConvFuncTable.conv32[from ^ to](*x_.ui);
    }

    static u8  toHostU8 (Types from, u8  x) { return convertU8 (from, cHostEndian, x); }
    static u16 toHostU16(Types from, u16 x) { return convertU16(from, cHostEndian, x); }
    static u32 toHostU32(Types from, u32 x) { return convertU32(from, cHostEndian, x); }
    static u64 toHostU64(Types from, u64 x) { return convertU64(from, cHostEndian, x); }

    static u8  fromHostU8 (Types to, u8  x) { return convertU8 (cHostEndian, to, x); }
    static u16 fromHostU16(Types to, u16 x) { return convertU16(cHostEndian, to, x); }
    static u32 fromHostU32(Types to, u32 x) { return convertU32(cHostEndian, to, x); }
    static u64 fromHostU64(Types to, u64 x) { return convertU64(cHostEndian, to, x); }

    static s8  toHostS8 (Types from, s8  x) { return convertS8 (from, cHostEndian, x); }
    static s16 toHostS16(Types from, s16 x) { return convertS16(from, cHostEndian, x); }
    static s32 toHostS32(Types from, s32 x) { return convertS32(from, cHostEndian, x); }
    static s64 toHostS64(Types from, s64 x) { return convertS64(from, cHostEndian, x); }

    static s8  fromHostS8 (Types to, s8  x) { return convertS8 (cHostEndian, to, x); }
    static s16 fromHostS16(Types to, s16 x) { return convertS16(cHostEndian, to, x); }
    static s32 fromHostS32(Types to, s32 x) { return convertS32(cHostEndian, to, x); }
    static s64 fromHostS64(Types to, s64 x) { return convertS64(cHostEndian, to, x); }

    static f32 toHostF32(Types from, const u32* x)
    {
        union { u32 ui; f32  f; } x_ = { .ui = convertF32(from, cHostEndian, x) };
        return x_. f;
    }

    static u32 fromHostF32(Types to, const f32* x) { return convertF32(cHostEndian, to, x); }

    static Types getHostEndian() { return cHostEndian; }

    static Types markToEndian(u16 mark)
    {
        u8* mark8 = reinterpret_cast<u8*>(&mark);
        if (*mark8 == 0xff && *(mark8 + 1) == 0xfe)
            return cLittle;

        else if (*mark8 == 0xfe && *(mark8 + 1) == 0xff)
            return cBig;

        //SEAD_ASSERT_MSG(false, "Undefined endian mark(0x%02x 0x%02x).", *mark8, *(mark8 + 1));
        return cLittle;
    }

    static u16 endianToMark(Types endian) { return (endian == cBig) ? 0xfeff : 0xfffe; }

private:
    typedef u8 (*CONV8FUNC)(u8);
    typedef u16 (*CONV16FUNC)(u16);
    typedef u32 (*CONV32FUNC)(u32);
    typedef u64 (*CONV64FUNC)(u64);

    struct ConvFuncTable
    {
        CONV8FUNC conv8[2];
        CONV16FUNC conv16[2];
        CONV32FUNC conv32[2];
        CONV64FUNC conv64[2];
    };

    static const Types cHostEndian;
    static const ConvFuncTable cConvFuncTable;
};

} // namespace sead

#endif // SEAD_ENDIAN_H_
