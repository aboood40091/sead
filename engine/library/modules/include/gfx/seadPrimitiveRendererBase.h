#ifndef SEAD_PRIMITIVE_RENDERER_BASE_H_
#define SEAD_PRIMITIVE_RENDERER_BASE_H_

#include <math/seadMatrix.h>
#include <prim/seadSafeString.h>

namespace sead {

class Heap;
class Camera;
struct Color4f;
class Projection;
class Texture;

class PrimitiveRendererBase
{
public:
    virtual void prepareFromBinaryImpl(Heap* heap, const void* bin_data, u32 bin_size) = 0;
    virtual void prepareImpl(Heap* heap, const SafeString& path) = 0;
    virtual void setCameraImpl(const Camera& camera) = 0;
    virtual void setProjectionImpl(const Projection& projection) = 0;
    virtual void beginImpl() = 0;
    virtual void endImpl() = 0;
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR) = 0;
    virtual void drawQuadImpl(const Matrix34f& model_mtx, const Texture& texture, const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src, const Vector2f& uv_size) = 0;
    virtual void drawBoxImpl(const Matrix34f& model_mtx, const Color4f& colorL, const Color4f& colorR) = 0;
    virtual void drawCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawWireCubeImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawLineImpl(const Matrix34f& model_mtx, const Color4f& c0, const Color4f& c1) = 0;
    virtual void drawSphere4x8Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south) = 0;
    virtual void drawSphere8x16Impl(const Matrix34f& model_mtx, const Color4f& north, const Color4f& south) = 0;
    virtual void drawDisk16Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge) = 0;
    virtual void drawDisk32Impl(const Matrix34f& model_mtx, const Color4f& center, const Color4f& edge) = 0;
    virtual void drawCircle16Impl(const Matrix34f& model_mtx, const Color4f& edge) = 0;
    virtual void drawCircle32Impl(const Matrix34f& model_mtx, const Color4f& edge) = 0;
    virtual void drawCylinder16Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm) = 0;
    virtual void drawCylinder32Impl(const Matrix34f& model_mtx, const Color4f& top, const Color4f& btm) = 0;
};
#ifdef cafe
static_assert(sizeof(PrimitiveRendererBase) == 4, "sead::PrimitiveRendererBase size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_PRIMITIVE_RENDERER_BASE_H_
