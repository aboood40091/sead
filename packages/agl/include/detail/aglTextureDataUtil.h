#pragma once

#include <common/aglTextureEnum.h>

#include <cafe/gx2.h>

namespace agl {

class TextureData;

namespace detail {

class TextureDataUtil
{
public:
    static TextureFormat convFormatGX2ToAGL(GX2SurfaceFormat format);
    static GX2SurfaceFormat convFormatAGLToGX2(TextureFormat format);

    static void calcSizeAndAlignment(GX2Surface* p_surface);
    static void initializeFromSurface(TextureData* p_texture_data, const GX2Surface& surface);

    static void printInfo(const GX2Surface& surface);
};

} }
