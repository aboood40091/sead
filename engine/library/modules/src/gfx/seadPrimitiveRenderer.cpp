#ifdef cafe
#include <gfx/cafe/seadPrimitiveRendererCafe.h>
#endif // cafe

#include <gfx/seadPrimitiveRenderer.h>
#include <math/seadMathCalcCommon.h>
#include <math/seadMatrixCalcCommon.h>
#include <math/seadQuatCalcCommon.h>
#include <math/seadVectorCalcCommon.h>

namespace sead {

SEAD_SINGLETON_DISPOSER_IMPL(PrimitiveRenderer)

PrimitiveRenderer::PrimitiveRenderer()
    : IDisposer()
    , mRendererImpl(nullptr)
    , mModelMtx(Matrix34f::ident)
{
}

PrimitiveRenderer::~PrimitiveRenderer()
{
    delete mRendererImpl;
}

void PrimitiveRenderer::doPrepare_(Heap* heap)
{
#ifdef cafe
    mRendererImpl = new(heap) PrimitiveRendererCafe(heap);
#else
    #error "Unknown platform"
#endif // cafe

  //SEAD_ASSERT(mRendererImpl);
}

void PrimitiveRenderer::prepareFromBinary(Heap* heap, const void* bin_data, u32 bin_size)
{
    doPrepare_(heap);
    mRendererImpl->prepareFromBinaryImpl(heap, bin_data, bin_size);
}

void PrimitiveRenderer::prepare(Heap* heap, const SafeString& path)
{
    doPrepare_(heap);
    mRendererImpl->prepareImpl(heap, path);
}

void PrimitiveRenderer::setCamera(const Camera& camera)
{
    mRendererImpl->setCameraImpl(camera);
}

void PrimitiveRenderer::setProjection(const Projection& projection)
{
    mRendererImpl->setProjectionImpl(projection);
}

void PrimitiveRenderer::setModelMatrix(const Matrix34f& model_to_world_matrix)
{
    mModelMtx = model_to_world_matrix;
}

void PrimitiveRenderer::begin()
{
    mRendererImpl->beginImpl();
}

void PrimitiveRenderer::end()
{
    mRendererImpl->endImpl();
}

void PrimitiveRenderer::drawQuad(const Color4f& colorL, const Color4f& colorR)
{
    mRendererImpl->drawQuadImpl(mModelMtx, colorL, colorR);
}

void PrimitiveRenderer::drawQuad(const Texture& texture, const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src, const Vector2f& uv_size)
{
    mRendererImpl->drawQuadImpl(mModelMtx, texture, colorL, colorR, uv_src, uv_size);
}

void PrimitiveRenderer::drawBox(const Color4f& colorL, const Color4f& colorR)
{
    mRendererImpl->drawBoxImpl(mModelMtx, colorL, colorR);
}

void PrimitiveRenderer::drawCube(const Color4f& c0, const Color4f& c1)
{
    mRendererImpl->drawCubeImpl(mModelMtx, c0, c1);
}

void PrimitiveRenderer::drawWireCube(const Color4f& c0, const Color4f& c1)
{
    mRendererImpl->drawWireCubeImpl(mModelMtx, c0, c1);
}

void PrimitiveRenderer::drawLine(const Color4f& c0, const Color4f& c1)
{
    mRendererImpl->drawLineImpl(mModelMtx, c0, c1);
}

void PrimitiveRenderer::drawSphere4x8(const Color4f& north, const Color4f& south)
{
    mRendererImpl->drawSphere4x8Impl(mModelMtx, north, south);
}

void PrimitiveRenderer::drawSphere8x16(const Color4f& north, const Color4f& south)
{
    mRendererImpl->drawSphere8x16Impl(mModelMtx, north, south);
}

void PrimitiveRenderer::drawDisk16(const Color4f& center, const Color4f& edge)
{
    mRendererImpl->drawDisk16Impl(mModelMtx, center, edge);
}

void PrimitiveRenderer::drawDisk32(const Color4f& center, const Color4f& edge)
{
    mRendererImpl->drawDisk32Impl(mModelMtx, center, edge);
}

void PrimitiveRenderer::drawCircle16(const Color4f& color)
{
    mRendererImpl->drawCircle16Impl(mModelMtx, color);
}

void PrimitiveRenderer::drawCircle32(const Color4f& color)
{
    mRendererImpl->drawCircle32Impl(mModelMtx, color);
}

void PrimitiveRenderer::drawCylinder16(const Color4f& top, const Color4f& btm)
{
    mRendererImpl->drawCylinder16Impl(mModelMtx, top, btm);
}

void PrimitiveRenderer::drawCylinder32(const Color4f& top, const Color4f& btm)
{
    mRendererImpl->drawCylinder32Impl(mModelMtx, top, btm);
}

void PrimitiveRenderer::drawQuad(const QuadArg& arg)
{
    Matrix34f mtx;
    if (arg.isHorizontal())
        mtx.makeSRT(
            Vector3f(arg.getSize().y, arg.getSize().x, 1.0f),
            Vector3f(0.0f, 0.0f, Mathf::deg2rad(90)),
            arg.getCenter()
        );
    else
        mtx.makeST(
            Vector3f(arg.getSize().x, arg.getSize().y, 1.0f),
            arg.getCenter()
        );

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawQuadImpl(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawQuad(const Texture& texture, const QuadArg& arg, const UVArg& uv_arg)
{
    Matrix34f mtx;
    if (arg.isHorizontal())
        mtx.makeSRT(
            Vector3f(arg.getSize().y, arg.getSize().x, 1.0f),
            Vector3f(0.0f, 0.0f, Mathf::deg2rad(90)),
            arg.getCenter()
        );
    else
        mtx.makeST(
            Vector3f(arg.getSize().x, arg.getSize().y, 1.0f),
            arg.getCenter()
        );

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawQuadImpl(outMtx, texture, arg.getColor0(), arg.getColor1(), uv_arg.getUVSrc(), uv_arg.getUVSize());
}

void PrimitiveRenderer::drawBox(const QuadArg& arg)
{
    Matrix34f mtx;
    if (arg.isHorizontal())
        mtx.makeSRT(
            Vector3f(arg.getSize().x, arg.getSize().y, 1.0f), // No flipping of x and y. Bug?
            Vector3f(0.0f, 0.0f, Mathf::deg2rad(90)),
            arg.getCenter()
        );
    else
        mtx.makeST(
            Vector3f(arg.getSize().x, arg.getSize().y, 1.0f),
            arg.getCenter()
        );

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawBoxImpl(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawCube(const CubeArg& arg)
{
    Matrix34f mtx;
    mtx.makeST(arg.getSize(), arg.getCenter());

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCubeImpl(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawWireCube(const CubeArg& arg)
{
    Matrix34f mtx;
    mtx.makeST(arg.getSize(), arg.getCenter());

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawWireCubeImpl(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawLine(const Vector3f& from, const Vector3f& to, const Color4f& c0, const Color4f& c1)
{
    Vector3f dir = to - from;

    Matrix34f mtxS;
    mtxS.makeS(dir.length(), 1.0f, 1.0f);

    dir.normalize();

    Quatf q;
    q.makeVectorRotation(Vector3f(1.0f, 0.0f, 0.0f), dir);

    Matrix34f mtxR;
    mtxR.fromQuat(q);

    Matrix34f mtx;
    mtx.setMul(mtxR, mtxS);

    dir = to - from;
    dir.multScalar(0.5f);
    dir += from;
    mtx.setTranslation(dir);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawLineImpl(outMtx, c0, c1);
}

void PrimitiveRenderer::drawLine(const Vector3f& from, const Vector3f& to, const Color4f& color)
{
    drawLine(from, to, color, color);
}

void PrimitiveRenderer::drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, radius * 2.0f, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawSphere4x8Impl(outMtx, north, south);
}

void PrimitiveRenderer::drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& color)
{
    drawSphere4x8(pos, radius, color, color);
}

void PrimitiveRenderer::drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, radius * 2.0f, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawSphere8x16Impl(outMtx, north, south);
}

void PrimitiveRenderer::drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& color)
{
    drawSphere8x16(pos, radius, color, color);
}

void PrimitiveRenderer::drawDisk16(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, radius * 2.0f, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawDisk16Impl(outMtx, center, edge);
}

void PrimitiveRenderer::drawDisk32(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, radius * 2.0f, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawDisk32Impl(outMtx, center, edge);
}

void PrimitiveRenderer::drawDisk16(const Vector3f& pos, f32 radius, const Color4f& color)
{
    drawDisk16(pos, radius, color, color);
}

void PrimitiveRenderer::drawDisk32(const Vector3f& pos, f32 radius, const Color4f& color)
{
    drawDisk32(pos, radius, color, color);
}

void PrimitiveRenderer::drawCircle16(const Vector3f& pos, f32 radius, const Color4f& color)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, radius * 2.0f, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCircle16Impl(outMtx, color);
}

void PrimitiveRenderer::drawCircle32(const Vector3f& pos, f32 radius, const Color4f& color)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, radius * 2.0f, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCircle32Impl(outMtx, color);
}

void PrimitiveRenderer::drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, height, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCylinder16Impl(outMtx, top_color, btm_color);
}

void PrimitiveRenderer::drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& color)
{
    drawCylinder16(pos, radius, height, color, color);
}

void PrimitiveRenderer::drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color)
{
    Matrix34f mtx;
    mtx.makeST(Vector3f(radius * 2.0f, height, radius * 2.0f), pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCylinder32Impl(outMtx, top_color, btm_color);
}

void PrimitiveRenderer::drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& color)
{
    drawCylinder32(pos, radius, height, color, color);
}

void PrimitiveRenderer::drawAxis(const Vector3f& pos, f32 scale)
{
    { Vector3f v(scale, 0.0f, 0.0f); v.add(pos);
      drawLine(pos, v, Color4f::cRed); }

    { Vector3f v(0.0f, scale, 0.0f); v.add(pos);
      drawLine(pos, v, Color4f::cGreen); }

    { Vector3f v(0.0f, 0.0f, scale); v.add(pos);
      drawLine(pos, v, Color4f::cBlue); }
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setCornerAndSize(const Vector3f& p, const Vector2f& size)
{
    mCenter.set(p.x + size.x * 0.5f,
                p.y + size.y * 0.5f,
                p.z);
    mSize = size;
    return *this;
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setBoundBox(const BoundBox2f& box, f32 z)
{
    Vector2f p;
    box.getCenter(&p);

    mCenter.set(p.x, p.y, z);
    mSize.set(box.getSizeX(), box.getSizeY());
    return *this;
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setColor(const Color4f& colorT, const Color4f& colorB)
{
    mHorizontal = false;
    mColor0 = colorT;
    mColor1 = colorB;
    return *this;
}

PrimitiveRenderer::QuadArg&
PrimitiveRenderer::QuadArg::setColorHorizontal(const Color4f& colorL, const Color4f& colorR)
{
    mHorizontal = true;
    mColor0 = colorL;
    mColor1 = colorR;
    return *this;
}

PrimitiveRenderer::CubeArg&
PrimitiveRenderer::CubeArg::setCornerAndSize(const Vector3f& p, const Vector3f& size)
{
    mCenter.setScaleAdd(0.5f, size, p);
    mSize = size;
    return *this;
}

PrimitiveRenderer::CubeArg&
PrimitiveRenderer::CubeArg::setBoundBox(const BoundBox3f& box)
{
    box.getCenter(&mCenter);
    mSize.set(box.getSizeX(), box.getSizeY(), box.getSizeZ());
    return *this;
}

} // namespace sead
