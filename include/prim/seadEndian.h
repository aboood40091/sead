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

    static inline Types markToEndian(u16 bom)
    {
        u8* bom_ = reinterpret_cast<u8*>(&bom);
        if (*bom_ == 0xff && *(bom_ + 1) == 0xfe)
            return cLittle;

        else if (*bom_ == 0xfe && *(bom_ + 1) == 0xff)
            return cBig;

        return cLittle;
    }

    static inline Types getHostEndian()
    {
        return cHostEndian;
    }

    static inline u32 toHost(Types from, u32 x)
    {
        return cConvFuncTable.conv32[from ^ cHostEndian](x);
    }

private:
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

    static const Types cHostEndian;
    static const ConvFuncTable cConvFuncTable;
};

} // namespace sead

#endif // SEAD_ENDIAN_H_
