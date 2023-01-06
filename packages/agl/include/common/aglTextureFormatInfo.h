#pragma once

#include <common/aglTextureEnum.h>

#include <cafe/gx2.h>

namespace agl {

class TextureFormatInfo
{
public:
    static u8 getPixelByteSize(TextureFormat format);
    static u8 getComponentNum(TextureFormat format);
    static u8 getComponentBitSize(TextureFormat format, s32 component);
    static u8 getComponentOrder(TextureFormat format, s32 component);
    static bool isCompressed(TextureFormat format);
    static bool isNormalized(TextureFormat format);
    static bool isFloat(TextureFormat format);
    static bool isUnsigned(TextureFormat format);
    static bool isSRGB(TextureFormat format);
    static bool isUsableAsRenderTargetColor(TextureFormat format);
    static bool isUsableAsRenderTargetDepth(TextureFormat format);
    static const char* getString(TextureFormat format);
    static TextureCompSel getDefaultCompSel(TextureFormat format, s32 component);
    static TextureFormat convFormatGX2ToAGL(GX2SurfaceFormat format);
    static GX2SurfaceFormat convFormatAGLToGX2(TextureFormat format);
};

}
