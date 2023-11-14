#include <common/aglTextureSampler.h>
#include <detail/aglShaderHolder.h>
#include <math/seadVector.h>
#include <utility/aglDevTools.h>
#include <utility/aglImageFilter2D.h>
#include <utility/aglPrimitiveShape.h>
#include <utility/aglVertexAttributeHolder.h>

namespace agl { namespace utl {

void DevTools::drawTexture_(const ShaderProgram& program, const TextureSampler& sampler, const sead::Matrix34f& world_view_mtx, const sead::Matrix44f& proj_mtx, const sead::Color4f& color)
{
    sampler.activate(program.getSamplerLocationValidate(ImageFilter2D::cSampler_Texture));

    sead::Matrix44f wvp_mtx;
    wvp_mtx.setMul(proj_mtx, world_view_mtx);

    program.getUniformLocation(ImageFilter2D::cUniform_ProjViewWorld).setVec4Array(wvp_mtx);

    program.getUniformLocation(ImageFilter2D::cUniform_Color).setVec4(color);

    VertexAttributeHolder::instance()->getVertexAttribute(VertexAttributeHolder::cAttribute_Quad).activate();
    PrimitiveShape::instance()->getIdxStreamQuad().draw();
}

ShaderMode DevTools::drawTextureTexCoord(const TextureSampler& sampler, const sead::Matrix34f& world_view_mtx, const sead::Matrix44f& proj_mtx, const sead::Vector2f& tex_scale, f32 tex_rotate, const sead::Vector2f& tex_coord, ShaderMode mode)
{
    const ShaderProgram& program =
        detail::ShaderHolder::instance()->getShader(detail::ShaderHolder::cShader_TextureTexCoord);

    mode = program.activate(mode);

    program.getUniformLocation(ImageFilter2D::cUniform_TexScale).setVec2(tex_scale);
    program.getUniformLocation(ImageFilter2D::cUniform_TexRotate).setFloat(tex_rotate);
    program.getUniformLocation(ImageFilter2D::cUniform_TexTrans).setVec2(tex_coord);

    drawTexture_(program, sampler, world_view_mtx, proj_mtx, sead::Color4f::cWhite);

    return mode;
}


} }
