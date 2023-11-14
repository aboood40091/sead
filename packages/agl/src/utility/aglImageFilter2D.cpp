#include <common/aglTextureSampler.h>
#include <detail/aglShaderHolder.h>
#include <math/seadVector.h>
#include <utility/aglImageFilter2D.h>
#include <utility/aglPrimitiveShape.h>
#include <utility/aglVertexAttributeHolder.h>

namespace agl { namespace utl {

void ImageFilter2D::drawQuadTriangle(const ShaderProgram& program, const TextureSampler& sampler)
{
    sead::Vector4f tex_size;
    tex_size.x = sampler.getTextureData().getWidth();
    tex_size.y = sampler.getTextureData().getHeight();
    tex_size.z = 1.0f / tex_size.x;
    tex_size.w = 1.0f / tex_size.y;

    program.getUniformLocation(cUniform_TexSize).setVec4(tex_size);
    sampler.activate(program.getSamplerLocationValidate(cSampler_Texture));

    VertexAttributeHolder::instance()->getVertexAttribute(VertexAttributeHolder::cAttribute_QuadTriangle).activate();
    PrimitiveShape::instance()->getIdxStreamQuadTriangle().draw();
}

ShaderMode ImageFilter2D::drawTextureQuadTriangle(const TextureSampler& sampler, ShaderMode mode)
{
    const ShaderProgram* program =
        detail::ShaderHolder::instance()->getShader(detail::ShaderHolder::cShader_Texture)
            .getVariation(1); // ENABLE_TEXTURE_LOD = 0, QUAD_TRIANGLE = 1

    mode = program->activate(mode);
    drawQuadTriangle(*program, sampler);

    return mode;
}

} }
