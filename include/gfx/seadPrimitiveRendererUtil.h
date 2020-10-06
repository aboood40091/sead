#ifndef SEAD_PRIMITIVE_RENDERER_UTIL_H_
#define SEAD_PRIMITIVE_RENDERER_UTIL_H_

#include <gfx/seadColor.h>
#include <math/seadVector.h>

namespace sead {

class PrimitiveRendererUtil
{
public:
    class Vertex
    {
    public:
        Vertex(const Vector3f& pos, const Vector2f& uv, const Color4f& color)
            : pos(pos)
            , uv(uv)
            , color(color)
        {
        }

        Vector3f pos;
        Vector2f uv;
        Color4f color;
    };

    static void setQuadVertex(Vertex* vtx, u16* idx);
    static void setLineVertex(Vertex* vtx, u16* idx);
    static void setCubeVertex(Vertex* vtx, u16* idx);
    static void setWireCubeVertex(Vertex* vtx, u16* idx);
    static void setSphereVertex(Vertex* vtx, u16* idx, s32 x, s32 y);
    static void setDiskVertex(Vertex* vtx, u16* idx, s32 div);
    static void setCylinderVertex(Vertex* vtx, u16* idx, s32 div);
};

} // namespace sead

#endif // SEAD_PRIMITIVE_RENDERER_UTIL_H_
