#ifndef SEAD_PRIMITIVE_RENDERER_UTIL_H_
#define SEAD_PRIMITIVE_RENDERER_UTIL_H_

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace sead { namespace PrimitiveRendererUtil {

struct Vertex
{
    Vector3f pos;
    Vector2f uv;
    Color4f color;
};
#ifdef cafe
static_assert(sizeof(Vertex) == 0x24, "sead::PrimitiveRendererUtil::Vertex size mismatch");
#endif // cafe

void setQuadVertex(Vertex* vtx, u16* idx);
void setLineVertex(Vertex* vtx, u16* idx);
void setCubeVertex(Vertex* vtx, u16* idx);
void setWireCubeVertex(Vertex* vtx, u16* idx);
void setSphereVertex(Vertex* vtx, u16* idx, s32 sizeX, s32 sizeY);
void setDiskVertex(Vertex* vtx, u16* idx, s32 divNum);
void setCylinderVertex(Vertex* vtx, u16* idx, s32 divNum);

} } // namespace sead::PrimitiveRendererUtil

#endif // SEAD_PRIMITIVE_RENDERER_UTIL_H_
