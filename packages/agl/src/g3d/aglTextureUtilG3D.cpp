#include <common/aglTextureData.h>
#include <g3d/aglTextureUtilG3D.h>
#include <prim/seadMemUtil.h>

namespace agl { namespace g3d {
nw::g3d::res::ResTexture* TextureUtilG3D::convertToResTexture(nw::g3d::res::ResTextureData* p_res_texture_data, const TextureData& texture_data)
{
    nw::g3d::res::ResTexture* const p_res_texture = nw::g3d::res::ResTexture::ResCast(p_res_texture_data);
    GX2Texture* const p_gx2_texture = p_res_texture->GetGfxTexture()->GetGX2Texture();
    // GL
    {
        p_res_texture_data->gfxTexture.handle = 0;
    }
    // GX2
    {
        static const u32 compSel[54] = {
            0x04040405, 0x00040405, 0x00010405, 0x04040405,
            0x04040405, 0x00040405, 0x00040405, 0x00010405,
            0x00010205, 0x00010205, 0x00010203, 0x00010203,
            0x03020100, 0x00040405, 0x00040405, 0x00010405,
            0x00010405, 0x04040405, 0x04040405, 0x04040405,
            0x04040405, 0x04040405, 0x00010205, 0x04040405,
            0x04040405, 0x00010203, 0x00010203, 0x03020100,
            0x00010405, 0x00010405, 0x00010405, 0x00010203,
            0x00010203, 0x04040405, 0x00010203, 0x00010203,
            0x04040405, 0x04040405, 0x04040405, 0x00010205,
            0x00010205, 0x00040405, 0x00010405, 0x00010205,
            0x04040405, 0x04040405, 0x04040405, 0x00010205,
            0x00010205, 0x00010203, 0x00010203, 0x00010203,
            0x00040405, 0x00010405
        };
        sead::MemUtil::copy(&p_gx2_texture->surface, &texture_data.getSurface(), sizeof(GX2Surface));
        p_res_texture_data->gfxTexture.gx2Texture.surface.use = (GX2SurfaceUse)(p_res_texture_data->gfxTexture.gx2Texture.surface.use | GX2_SURFACE_USE_TEXTURE);
        p_res_texture_data->gfxTexture.gx2Texture.viewFirstMip = 0;
        p_res_texture_data->gfxTexture.gx2Texture.viewNumMips = p_res_texture_data->gfxTexture.gx2Texture.surface.numMips;
        p_res_texture_data->gfxTexture.gx2Texture.viewFirstSlice = 0;
        p_res_texture_data->gfxTexture.gx2Texture.viewNumSlices = p_res_texture_data->gfxTexture.gx2Texture.surface.depth;
        p_res_texture_data->gfxTexture.gx2Texture.compSel = compSel[p_res_texture_data->gfxTexture.gx2Texture.surface.format & 0x3f];
        p_res_texture_data->ofsData.set_ptr(p_res_texture_data->gfxTexture.gx2Texture.surface.imagePtr);
        p_res_texture_data->ofsMipData.set_ptr(p_gx2_texture->surface.mipPtr);
        GX2InitTextureRegs(p_gx2_texture);
    }
    return p_res_texture;
}

} }
