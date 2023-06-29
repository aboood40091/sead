#include <utility/aglPrimitiveTexture.h>

namespace agl { namespace utl {

SEAD_SINGLETON_DISPOSER_IMPL(PrimitiveTexture)

PrimitiveTexture::PrimitiveTexture()
{
}

PrimitiveTexture::~PrimitiveTexture()
{
}

void PrimitiveTexture::initialize(sead::Heap* heap)
{
    for (sead::SafeArray<TextureSampler, cSampler_Num>::iterator itr_sampler = mTextureSampler.begin(), itr_sampler_end = mTextureSampler.end(); itr_sampler != itr_sampler_end; ++itr_sampler)
    {
        TextureSampler& sampler = *itr_sampler;
        TextureSamplerType type = TextureSamplerType(itr_sampler.getIndex());

        TextureFormat format = cTextureFormat_R8_G8_B8_A8_uNorm;
        u8 pixel[4];

        switch (type)
        {
        case cSampler_White2D:
            *(u32*)&pixel = 0xffffffff;
            break;
        case cSampler_Gray2D:
            *(u32*)&pixel = 0x808080ff;
            break;
        case cSampler_Black2D:
        case cSampler_Black2DArray:
            *(u32*)&pixel = 0x000000ff;
            break;
        case cSampler_BlackCube:
        case cSampler_BlackCubeArray:
            *(u32*)&pixel = 0x00000000;
            break;
        case cSampler_Depth32_0:
            format = cTextureFormat_Depth_32;
            *(f32*)&pixel = 0.0f;
            break;
        case cSampler_Depth32_1:
        case cSampler_DepthShadow:
        case cSampler_DepthShadowArray:
            format = cTextureFormat_Depth_32;
            *(f32*)&pixel = 1.0f;
            break;
        default:
            break;
        }

        if (type == cSampler_MipLevel)
            initializeMipLevelTexture_(heap);

        else
        {
            TextureData texture_data;

            switch (type)
            {
          //case cSampler_White2D:
          //case cSampler_Gray2D:
          //case cSampler_Black2D:
          //case cSampler_Depth32_0:
          //case cSampler_Depth32_1:
          //case cSampler_DepthShadow:
            default:
                texture_data.initialize(format, 4, 4, 1);
                break;
            case cSampler_Black2DArray:
            case cSampler_DepthShadowArray:
                texture_data.initializeArray(format, 4, 4, 1, 1);
                break;
            case cSampler_BlackCube:
                texture_data.initializeCubeMap(format, 4, 4, 1);
                break;
            case cSampler_BlackCubeArray:
                texture_data.initializeCubeMapArray(format, 4, 4, 1, 1);
                break;
            }

            void* image_ptr = new (heap, texture_data.getAlignment()) u8[texture_data.getImageByteSize()];
            for (u32 i = 0; i < texture_data.getImageByteSize() / sizeof(u32); i++)
                ((u32*)image_ptr)[i] = *(u32*)&pixel;

            texture_data.setImagePtr(image_ptr);

            sampler.applyTextureData(texture_data);

            if (type >= cSampler_DepthShadow && type <= cSampler_DepthShadowArray)
            {
                sampler.setDepthCompareEnable(true);
                sampler.setWrap(cTextureWrapType_Clamp, cTextureWrapType_Clamp, cTextureWrapType_Clamp);
                sampler.setDepthCompareFunc(sead::Graphics::cDepthFunc_Always);
            }
        }
    }
}

} }

