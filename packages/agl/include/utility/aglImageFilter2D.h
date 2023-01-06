#pragma once

#include <common/aglShaderEnum.h>

namespace agl {

class ShaderProgram;
class TextureSampler;

namespace utl {

class ImageFilter2D
{
public:
    enum Uniform
    {
        cUniform_ProjViewWorld = 0,
        cUniform_TexSize,
        cUniform_MipLevel,
        cUniform_Slice,
        cUniform_TexScale,
        cUniform_TexRotate,
        cUniform_TexTrans,
        cUniform_MultiSampleNum,
        cUniform_Color,
        cUniform_TexFlipY,
        cUniform_TexFetchAdjust,
        cUniform_ColorDriftR,
        cUniform_ColorDriftG,
        cUniform_ColorDriftB,
        cUniform_ColorT,
        cUniform_ColorB,
        cUniform_CubeMapFace,
        cUniform_BlurOffset,
        cUniform_DepthNear,
        cUniform_OneMinusNearDivFar,
        cUniform_InvDepthRange,
        cUniform_NV12DecodeParam,
        cUniform_Num
    };
    static_assert(cUniform_Num == 22);

public:
    static void drawQuadTriangle(const ShaderProgram& program, const TextureSampler& sampler);
    static ShaderMode drawTextureQuadTriangle(const TextureSampler& sampler, ShaderMode mode);
};

} }
