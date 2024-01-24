#include <common/aglTextureData.h>
#include <common/aglTextureDataInitializer.h>
#include <detail/aglPrivateResource.h>
#include <detail/aglTextureDataUtil.h>

namespace agl {

void TextureDataInitializerRAW::initialize(TextureData* p_texture_data, void* p_raw_image, u32 raw_image_byte_syze, TextureFormat format, s32 width, s32 height, sead::Heap* heap)
{
  //SEAD_ASSERT(p_texture_data != nullptr);
  //SEAD_ASSERT(p_raw_image != nullptr);
  //SEAD_ASSERT(p_texture_data->getImageByteSize() <= raw_image_byte_syze);

    GX2Surface surface;
    surface.dim = GX2_SURFACE_DIM_2D;
    surface.width = width;
    surface.height = height;
    surface.depth = 1;
    surface.numMips = 1;
    surface.format = detail::TextureDataUtil::convFormatAGLToGX2(format);
    surface.aa = GX2_AA_MODE_1X;
    surface.use = GX2_SURFACE_USE_TEXTURE;
    surface.imagePtr = nullptr;
    surface.mipPtr = nullptr;
    surface.tileMode = GX2_TILE_MODE_DEFAULT;
    surface.swizzle = 0;
    GX2CalcSurfaceSizeAndAlignment(&surface);

    if (heap == nullptr)
        heap = detail::PrivateResource::instance()->getWorkHeap();

    surface.imagePtr = heap->tryAlloc(surface.imageSize, surface.alignment);

    GX2Surface surface_linear = surface;
    surface_linear.imagePtr = p_raw_image;
    surface_linear.tileMode = GX2_TILE_MODE_LINEAR_SPECIAL;
    GX2CalcSurfaceSizeAndAlignment(&surface_linear);
    GX2CopySurface(&surface_linear, 0, 0, &surface, 0, 0);

    sead::MemUtil::copy(p_raw_image, surface.imagePtr, raw_image_byte_syze);
    DCFlushRangeNoSync(p_raw_image, raw_image_byte_syze);

    heap->free(surface.imagePtr);
    surface.imagePtr = p_raw_image;

    detail::TextureDataUtil::initializeFromSurface(p_texture_data, surface);
}

}
