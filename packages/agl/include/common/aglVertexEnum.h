#pragma once

#include <basis/seadTypes.h>

namespace agl {

// No idea which headers these are actually supposed to go in

enum VertexStreamFormat
{
    cVertexStreamFormat_8_uNorm                     = 0x00000000,
    cVertexStreamFormat_8_uInt                      = 0x00000100,
    cVertexStreamFormat_8_sNorm                     = 0x00000200,
    cVertexStreamFormat_8_sInt                      = 0x00000300,
    cVertexStreamFormat_8_uInt_to_float             = 0x00000800,
    cVertexStreamFormat_8_sInt_to_float             = 0x00000a00,

    cVertexStreamFormat_4_4_uNorm                   = 0x00000001,

    cVertexStreamFormat_16_uNorm                    = 0x00000002,
    cVertexStreamFormat_16_uInt                     = 0x00000102,
    cVertexStreamFormat_16_sNorm                    = 0x00000202,
    cVertexStreamFormat_16_sInt                     = 0x00000302,
    cVertexStreamFormat_16_float                    = 0x00000803,
    cVertexStreamFormat_16_uInt_to_float            = 0x00000802,
    cVertexStreamFormat_16_sInt_to_float            = 0x00000a02,

    cVertexStreamFormat_8_8_uNorm                   = 0x00000004,
    cVertexStreamFormat_8_8_uInt                    = 0x00000104,
    cVertexStreamFormat_8_8_sNorm                   = 0x00000204,
    cVertexStreamFormat_8_8_sInt                    = 0x00000304,
    cVertexStreamFormat_8_8_uInt_to_float           = 0x00000804,
    cVertexStreamFormat_8_8_sInt_to_float           = 0x00000a04,

    cVertexStreamFormat_32_uInt                     = 0x00000105,
    cVertexStreamFormat_32_sInt                     = 0x00000305,
    cVertexStreamFormat_32_float                    = 0x00000806,

    cVertexStreamFormat_16_16_uNorm                 = 0x00000007,
    cVertexStreamFormat_16_16_uInt                  = 0x00000107,
    cVertexStreamFormat_16_16_sNorm                 = 0x00000207,
    cVertexStreamFormat_16_16_sInt                  = 0x00000307,
    cVertexStreamFormat_16_16_float                 = 0x00000808,
    cVertexStreamFormat_16_16_uInt_to_float         = 0x00000807,
    cVertexStreamFormat_16_16_sInt_to_float         = 0x00000a07,

    cVertexStreamFormat_10_11_11_float              = 0x00000809,

    cVertexStreamFormat_8_8_8_8_uNorm               = 0x0000000a,
    cVertexStreamFormat_8_8_8_8_uInt                = 0x0000010a,
    cVertexStreamFormat_8_8_8_8_sNorm               = 0x0000020a,
    cVertexStreamFormat_8_8_8_8_sInt                = 0x0000030a,
    cVertexStreamFormat_8_8_8_8_uInt_to_float       = 0x0000080a,
    cVertexStreamFormat_8_8_8_8_sInt_to_float       = 0x00000a0a,

    cVertexStreamFormat_10_10_10_2_uNorm            = 0x0000000b,
    cVertexStreamFormat_10_10_10_2_uInt             = 0x0000010b,
    cVertexStreamFormat_10_10_10_2_sNorm            = 0x0000020b,
    cVertexStreamFormat_10_10_10_2_sInt             = 0x0000030b,

    cVertexStreamFormat_32_32_uInt                  = 0x0000010c,
    cVertexStreamFormat_32_32_sInt                  = 0x0000030c,
    cVertexStreamFormat_32_32_float                 = 0x0000080d,

    cVertexStreamFormat_16_16_16_16_uNorm           = 0x0000000e,
    cVertexStreamFormat_16_16_16_16_uInt            = 0x0000010e,
    cVertexStreamFormat_16_16_16_16_sNorm           = 0x0000020e,
    cVertexStreamFormat_16_16_16_16_sInt            = 0x0000030e,
    cVertexStreamFormat_16_16_16_16_float           = 0x0000080f,
    cVertexStreamFormat_16_16_16_16_uInt_to_float   = 0x0000080e,
    cVertexStreamFormat_16_16_16_16_sInt_to_float   = 0x00000a0e,

    cVertexStreamFormat_32_32_32_uInt               = 0x00000110,
    cVertexStreamFormat_32_32_32_sInt               = 0x00000310,
    cVertexStreamFormat_32_32_32_float              = 0x00000811,

    cVertexStreamFormat_32_32_32_32_uInt            = 0x00000112,
    cVertexStreamFormat_32_32_32_32_sInt            = 0x00000312,
    cVertexStreamFormat_32_32_32_32_float           = 0x00000813
};
static_assert(sizeof(VertexStreamFormat) == 4, "agl::VertexStreamFormat size mismatch");

enum IndexStreamFormat
{
    cIndexStreamFormat_u16 = 4,
    cIndexStreamFormat_u32 = 9
};
static_assert(sizeof(IndexStreamFormat) == 4, "agl::IndexStreamFormat size mismatch");

}
