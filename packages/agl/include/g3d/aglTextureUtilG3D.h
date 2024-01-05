#pragma once

#include <basis/seadTypes.h>

#include <nw/g3d/res/g3d_ResTexture.h>

namespace agl {

class TextureData;

namespace g3d {

class TextureUtilG3D
{
public:
    static nw::g3d::res::ResTexture* convertToResTexture(nw::g3d::res::ResTextureData* p_res_texture_data, const TextureData& texture_data);
};

} }
