#pragma once

#include <prim/seadSafeString.h>

#include <nw/g3d/res/g3d_ResTexture.h>

namespace agl {

class TextureData;

namespace g3d {

class TextureDataInitializerG3D
{
public:
    static void initialize(TextureData* p_texture_data, void* p_bfres_file_data, u32 index);
    static void initialize(TextureData* p_texture_data, void* p_bfres_file_data, const char* name);
    static void initialize(TextureData* p_texture_data, nw::g3d::res::ResTexture& res_texture);

    static void initialize(TextureData* p_texture_data, void* p_bfres_file_data, const sead::SafeString& name)
    {
        initialize(p_texture_data, p_bfres_file_data, name.cstr());
    }
};

} }
