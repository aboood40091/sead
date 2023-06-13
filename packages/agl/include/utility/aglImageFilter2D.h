#pragma once

#include <common/aglShaderEnum.h>

namespace agl {

class ShaderProgram;
class TextureSampler;

namespace utl {

class ImageFilter2D
{
public:
    enum Attribute
    {
        cAttribute_Position = 0,
        cAttribute_Num
    };
    static_assert(cAttribute_Num == 1);

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

    enum XluSnapUniform
    {
        cXluSnapUniform_Alpha = cUniform_Num,
        cXluSnapUniform_Num
    };
    static_assert(cXluSnapUniform_Num == 23);

    enum Sampler
    {
        cSampler_Texture = 0,
        cSampler_Texture_1,
        cSampler_Num
    };
    static_assert(cSampler_Num == 2);

    enum XluSnapSampler
    {
        cXluSnapSampler_SnapColor = cSampler_Num,
        cXluSnapSampler_SnapDepth,
        cXluSnapSampler_TargetColor,
        cXluSnapSampler_TargetDepth,
        cXluSnapSampler_Num
    };
    static_assert(cXluSnapSampler_Num == 6);

    enum FrameBufferCopySampler
    {
        cFrameBufferCopySampler_TextureColor_0 = 0,
        cFrameBufferCopySampler_TextureColor_1,
        cFrameBufferCopySampler_TextureColor_2,
        cFrameBufferCopySampler_TextureColor_3,
        cFrameBufferCopySampler_TextureColor_4,
        cFrameBufferCopySampler_TextureColor_5,
        cFrameBufferCopySampler_TextureColor_6,
        cFrameBufferCopySampler_TextureColor_7,
        cFrameBufferCopySampler_TextureDepth,
        cFrameBufferCopySampler_Num
    };
    static_assert(cFrameBufferCopySampler_Num == 9);

public:
    static void drawQuadTriangle(const ShaderProgram& program, const TextureSampler& sampler);
    static ShaderMode drawTextureQuadTriangle(const TextureSampler& sampler, ShaderMode mode);
};

} }
