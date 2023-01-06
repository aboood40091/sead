#include <common/aglShaderLocation.h>
#include <common/aglShaderProgram.h>

namespace agl {

void UniformLocation::search(const ShaderProgram& program)
{
#ifdef cafe
    if (program.getVertexShaderBinary())
        mVS = GX2GetVertexUniformVarOffset(program.getVertexShaderBinary(), getName().cstr());
    else
        mVS = -1;

    if (program.getFragmentShaderBinary())
        mFS = GX2GetPixelUniformVarOffset(program.getFragmentShaderBinary(), getName().cstr());
    else
        mFS = -1;

    if (program.getGeometryShaderBinary())
        mGS = GX2GetGeometryUniformVarOffset(program.getGeometryShaderBinary(), getName().cstr());
    else
        mGS = -1;
#else
    mVS = -1;
    mFS = -1;
    mGS = -1;
#endif
}

void UniformBlockLocation::search(const ShaderProgram& program)
{
    mVS = -1;
    mFS = -1;
    mGS = -1;

#ifdef cafe
    const GX2VertexShader* vertex_shader = program.getVertexShaderBinary();
    if (vertex_shader)
    {
        GX2UniformBlock* uniform_block = GX2GetVertexUniformBlock(vertex_shader, getName().cstr());
        if (uniform_block)
            mVS = uniform_block->location;
    }
    const GX2PixelShader* pixel_shader = program.getFragmentShaderBinary();
    if (pixel_shader)
    {
        GX2UniformBlock* uniform_block = GX2GetPixelUniformBlock(pixel_shader, getName().cstr());
        if (uniform_block)
            mFS = uniform_block->location;
    }
    const GX2GeometryShader* geometry_shader = program.getGeometryShaderBinary();
    if (geometry_shader)
    {
        GX2UniformBlock* uniform_block = GX2GetGeometryUniformBlock(geometry_shader, getName().cstr());
        if (uniform_block)
            mGS = uniform_block->location;
    }
#endif // cafe
}

void SamplerLocation::search(const ShaderProgram& program)
{
    mVS = -1;
    mFS = -1;
    mGS = -1;

#ifdef cafe
    const GX2VertexShader* vertex_shader = program.getVertexShaderBinary();
    if (vertex_shader)
    {
        GX2SamplerVar* sampler_var = GX2GetVertexSamplerVar(vertex_shader, getName().cstr());
        if (sampler_var)
            mVS = sampler_var->location;
    }
    const GX2PixelShader* pixel_shader = program.getFragmentShaderBinary();
    if (pixel_shader)
    {
        GX2SamplerVar* sampler_var = GX2GetPixelSamplerVar(pixel_shader, getName().cstr());
        if (sampler_var)
            mFS = sampler_var->location;
    }
    const GX2GeometryShader* geometry_shader = program.getGeometryShaderBinary();
    if (geometry_shader)
    {
        GX2SamplerVar* sampler_var = GX2GetGeometrySamplerVar(geometry_shader, getName().cstr());
        if (sampler_var)
            mGS = sampler_var->location;
    }
#endif // cafe
}

void AttributeLocation::search(const ShaderProgram& program)
{
#ifdef cafe
    // if (program.getVertexShaderBinary())
        mVS = GX2GetVertexAttribVarLocation(program.getVertexShaderBinary(), getName().cstr());
    // else
    //     mVS = -1;
#else
    mVS = -1;
#endif
}

}
