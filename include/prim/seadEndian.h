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

    struct ConvFuncTable
    {
        typedef u8 (*CONV8FUNC)(u8);
        typedef u16 (*CONV16FUNC)(u16);
        typedef u32 (*CONV32FUNC)(u32);
        typedef u64 (*CONV64FUNC)(u64);

        CONV8FUNC conv8[2];
        CONV16FUNC conv16[2];
        CONV32FUNC conv32[2];
        CONV64FUNC conv64[2];
    };

    static Types markToEndian(u16 bom)
    {
        u8* bom_ = reinterpret_cast<u8*>(&bom);
        if (*bom_ == 0xff && *(bom_ + 1) == 0xfe)
            return cLittle;

        else if (*bom_ == 0xfe && *(bom_ + 1) == 0xff)
            return cBig;

        return cLittle;
    }

    static u8 readU8(const void* ptr)
    {
        return cConvFuncTable.conv8[cHostEndian](*static_cast<const u8*>(ptr));
    }

    static s8 readS8(const void* ptr)
    {
        return static_cast<s8>(readU8(ptr));
    }

    static u16 readU16(const void* ptr)
    {
        return cConvFuncTable.conv16[cHostEndian](*static_cast<const u16*>(ptr));
    }

    static s16 readS16(const void* ptr)
    {
        return static_cast<s16>(readU16(ptr));
    }

    static u32 readU32(const void* ptr)
    {
        return cConvFuncTable.conv32[cHostEndian](*static_cast<const u32*>(ptr));
    }

    static s32 readS32(const void* ptr)
    {
        return static_cast<s32>(readU32(ptr));
    }

    static u64 readU64(const void* ptr)
    {
        return cConvFuncTable.conv64[cHostEndian](*static_cast<const u64*>(ptr));
    }

    static s64 readS64(const void* ptr)
    {
        return static_cast<s64>(readU64(ptr));
    }

    static const Types cHostEndian;
    static const ConvFuncTable cConvFuncTable;
};

} // namespace sead

#endif // SEAD_ENDIAN_H_
