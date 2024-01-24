#pragma once

#include <common/aglShaderEnum.h>
#include <gfx/seadColor.h>
#include <math/seadMatrix.h>

namespace agl {

class RenderBuffer;
class ShaderProgram;
class TextureSampler;

namespace utl {

class DevTools
{
public:
    static const u32 cDepthMax = 32;

public:
    static ShaderMode drawDepthGradation(const RenderBuffer& render_buffer, u32 depth_num, const f32* depth_values, const sead::Color4f* depth_colors, f32 near, f32 far, ShaderMode mode);
    static ShaderMode drawTextureTexCoord(const TextureSampler& sampler, const sead::Matrix34f& world_view_mtx, const sead::Matrix44f& proj_mtx, const sead::Vector2f& tex_scale, f32 tex_rotate, const sead::Vector2f& tex_coord, ShaderMode mode);

private:
    static void drawTexture_(const ShaderProgram& program, const TextureSampler& sampler, const sead::Matrix34f& world_view_mtx, const sead::Matrix44f& proj_mtx, const sead::Color4f& color);
};

} }
