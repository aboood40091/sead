#pragma once

#include <common/aglTextureEnum.h>
#include <heap/seadHeap.h>

namespace agl {

class TextureData;

class TextureDataInitializerRAW
{
public:
    static void initialize(TextureData* p_texture_data, void* p_raw_image, u32 raw_image_byte_syze, TextureFormat format, s32 width, s32 height, sead::Heap* heap = nullptr); // Awesome typo
};

}
