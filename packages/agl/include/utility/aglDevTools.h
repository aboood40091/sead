#pragma once

#include <common/aglShaderEnum.h>

namespace agl {

class ShaderProgram;
class TextureSampler;

namespace utl {

class DevTools
{
public:
    static ShaderMode drawTextureTexCoord(const TextureSampler& sampler, const sead::Matrix34f& world_view_mtx, const sead::Matrix44f& proj_mtx, const sead::Vector2f& tex_scale, f32 tex_rotate, const sead::Vector2f& tex_coord, ShaderMode mode);

private:
    static void drawTexture_(const ShaderProgram& program, const TextureSampler& sampler, const sead::Matrix34f& world_view_mtx, const sead::Matrix44f& proj_mtx, const sead::Color4f& color);
};

} }
