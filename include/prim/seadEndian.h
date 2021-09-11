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
    static u8  swap(u8  x) { return cConvFuncTable.conv8 [1](x); }
    static u16 swap(u16 x) { return cConvFuncTable.conv16[1](x); }
    static u32 swap(u32 x) { return cConvFuncTable.conv32[1](x); }
    static u64 swap(u64 x) { return cConvFuncTable.conv64[1](x); }

    static s8  swap(s8  x) { return cConvFuncTable.conv8 [1](x); }
    static s16 swap(s16 x) { return cConvFuncTable.conv16[1](x); }
    static s32 swap(s32 x) { return cConvFuncTable.conv32[1](x); }
    static s64 swap(s64 x) { return cConvFuncTable.conv64[1](x); }

    static f32 swap(f32 x)
    {
        union { u32 ui; f32 f; } x_ = { .f = x };
        x_.ui = swap(x_.ui);
        return x_.f;
    }

    static u8  convert(Types from, Types to, u8  x) { return cConvFuncTable.conv8 [from ^ to](x); }
    static u16 convert(Types from, Types to, u16 x) { return cConvFuncTable.conv16[from ^ to](x); }
    static u32 convert(Types from, Types to, u32 x) { return cConvFuncTable.conv32[from ^ to](x); }
    static u64 convert(Types from, Types to, u64 x) { return cConvFuncTable.conv64[from ^ to](x); }

    static s8  convert(Types from, Types to, s8  x) { return cConvFuncTable.conv8 [from ^ to](x); }
    static s16 convert(Types from, Types to, s16 x) { return cConvFuncTable.conv16[from ^ to](x); }
    static s32 convert(Types from, Types to, s32 x) { return cConvFuncTable.conv32[from ^ to](x); }
    static s64 convert(Types from, Types to, s64 x) { return cConvFuncTable.conv64[from ^ to](x); }

    static f32 convert(Types from, Types to, f32 x)
    {
        union { u32 ui; f32 f; } x_ = { .f = x };
        x_.ui = convert(from, to, x_.ui);
        return x_.f;
    }

    static u32 convertF32(Types from, Types to, const void* x) { return convert(from, to, *(u32*)x); }

    static u8  toHost(Types from, u8  x) { return convert(from, cHostEndian, x); }
    static u16 toHost(Types from, u16 x) { return convert(from, cHostEndian, x); }
    static u32 toHost(Types from, u32 x) { return convert(from, cHostEndian, x); }
    static u64 toHost(Types from, u64 x) { return convert(from, cHostEndian, x); }

    static u8  fromHost(Types to, u8  x) { return convert(cHostEndian, to, x); }
    static u16 fromHost(Types to, u16 x) { return convert(cHostEndian, to, x); }
    static u32 fromHost(Types to, u32 x) { return convert(cHostEndian, to, x); }
    static u64 fromHost(Types to, u64 x) { return convert(cHostEndian, to, x); }

    static s8  toHost(Types from, s8  x) { return convert(from, cHostEndian, x); }
    static s16 toHost(Types from, s16 x) { return convert(from, cHostEndian, x); }
    static s32 toHost(Types from, s32 x) { return convert(from, cHostEndian, x); }
    static s64 toHost(Types from, s64 x) { return convert(from, cHostEndian, x); }
    static f32 toHost(Types from, f32 x) { return convert(from, cHostEndian, x); }

    static s8  fromHost(Types to, s8  x) { return convert(cHostEndian, to, x); }
    static s16 fromHost(Types to, s16 x) { return convert(cHostEndian, to, x); }
    static s32 fromHost(Types to, s32 x) { return convert(cHostEndian, to, x); }
    static s64 fromHost(Types to, s64 x) { return convert(cHostEndian, to, x); }
    static f32 fromHost(Types to, f32 x) { return convert(cHostEndian, to, x); }

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
