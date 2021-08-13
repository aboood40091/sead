#ifdef cafe
#include <gfx/cafe/seadPrimitiveRendererCafe.h>
#endif // cafe

#include <gfx/seadPrimitiveRenderer.h>
#include <math/seadMathCalcCommon.hpp>
#include <math/seadMatrixCalcCommon.hpp>
#include <math/seadQuatCalcCommon.hpp>
#include <math/seadVectorCalcCommon.hpp>

namespace sead {

PrimitiveRenderer* PrimitiveRenderer::sInstance = NULL;
PrimitiveRenderer::SingletonDisposer_* PrimitiveRenderer::SingletonDisposer_::sStaticDisposer = NULL;

PrimitiveRenderer::PrimitiveRenderer()
    : IDisposer()
    , mRendererImpl(NULL)
    , mModelMtx(Matrix34f::ident)
{
}

SEAD_CREATE_SINGLETON_INSTANCE(PrimitiveRenderer)
SEAD_DELETE_SINGLETON_INSTANCE(PrimitiveRenderer)

void PrimitiveRenderer::doPrepare_(Heap* heap)
{
#ifdef cafe
    mRendererImpl = new(heap) PrimitiveRendererCafe(heap);
#else
    #error "Unknown platform"
#endif // cafe
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

void PrimitiveRenderer::setModelMatrix(const Matrix34f& model_mtx)
{
    mModelMtx = model_mtx;
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
    {
        const Vector2f& size = arg.getSize();
        Vector3f scale(size.y, size.x, 1.0f);
        Vector3f rotation(0.0f, 0.0f, Mathf::deg2rad(90));

        mtx.makeSRT(scale, rotation, arg.getCenter());
    }
    else
    {
        const Vector2f& size = arg.getSize();
        Vector3f scale(size.x, size.y, 1.0f);

        mtx.makeST(scale, arg.getCenter());
    }

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawQuadImpl(outMtx, arg.getColor0(), arg.getColor1());
}

void PrimitiveRenderer::drawQuad(const Texture& texture, const QuadArg& arg, const UVArg& uv_arg)
{
    Matrix34f mtx;

    if (arg.isHorizontal())
    {
        const Vector2f& size = arg.getSize();
        Vector3f scale(size.y, size.x, 1.0f);
        Vector3f rotation(0.0f, 0.0f, Mathf::deg2rad(90));

        mtx.makeSRT(scale, rotation, arg.getCenter());
    }
    else
    {
        const Vector2f& size = arg.getSize();
        Vector3f scale(size.x, size.y, 1.0f);

        mtx.makeST(scale, arg.getCenter());
    }

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawQuadImpl(outMtx, texture, arg.getColor0(), arg.getColor1(), uv_arg.getUVSrc(), uv_arg.getUVSize());
}

void PrimitiveRenderer::drawBox(const QuadArg& arg)
{
    Matrix34f mtx;

    if (arg.isHorizontal())
    {
        const Vector2f& size = arg.getSize();
        Vector3f scale(size.x, size.y, 1.0f); // No flipping of x and y. Bug?
        Vector3f rotation(0.0f, 0.0f, Mathf::deg2rad(90));

        mtx.makeSRT(scale, rotation, arg.getCenter());
    }
    else
    {
        const Vector2f& size = arg.getSize();
        Vector3f scale(size.x, size.y, 1.0f);

        mtx.makeST(scale, arg.getCenter());
    }

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

    Vector3f translation = to - from;
    translation.multScalar(0.5f);
    translation += from;
    mtx.setTranslation(translation);

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
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, diameter, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

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
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, diameter, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

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
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, diameter, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawDisk16Impl(outMtx, center, edge);
}

void PrimitiveRenderer::drawDisk32(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, diameter, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

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
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, diameter, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCircle16Impl(outMtx, color);
}

void PrimitiveRenderer::drawCircle32(const Vector3f& pos, f32 radius, const Color4f& color)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, diameter, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCircle32Impl(outMtx, color);
}

void PrimitiveRenderer::drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& top, const Color4f& btm)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, height, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCylinder16Impl(outMtx, top, btm);
}

void PrimitiveRenderer::drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& color)
{
    drawCylinder16(pos, radius, height, color, color);
}

void PrimitiveRenderer::drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& top, const Color4f& btm)
{
    f32 diameter = 2.0f * radius;
    Vector3f scale(diameter, height, diameter);

    Matrix34f mtx;
    mtx.makeST(scale, pos);

    Matrix34f outMtx;
    outMtx.setMul(mModelMtx, mtx);

    mRendererImpl->drawCylinder32Impl(outMtx, top, btm);
}

void PrimitiveRenderer::drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& color)
{
    drawCylinder32(pos, radius, height, color, color);
}

void PrimitiveRenderer::drawAxis(const Vector3f& pos, f32 length)
{
    drawLine(pos, Vector3f(length, 0.0f, 0.0f) += pos, Color4f::cRed);
    drawLine(pos, Vector3f(0.0f, length, 0.0f) += pos, Color4f::cGreen);
    drawLine(pos, Vector3f(0.0f, 0.0f, length) += pos, Color4f::cBlue);
}

void PrimitiveRenderer::QuadArg::setCornerAndSize(const Vector3f& corner, const Vector2f& size)
{
    mCenter.set(size.x * 0.5f + corner.x,
                size.y * 0.5f + corner.y,
                                corner.z);
    mSize = size;
}

void PrimitiveRenderer::QuadArg::setBoundBox(const BoundBox2f& bound_box, f32 centerZ)
{
    Vector2f center;
    bound_box.getCenter(&center);

    mCenter.set(center.x, center.y, centerZ);
    mSize.set(bound_box.getSizeX(), bound_box.getSizeY());
}

void PrimitiveRenderer::QuadArg::setColor(const Color4f& top, const Color4f& btm)
{
    mHorizontal = false;
    mColor0 = top;
    mColor1 = btm;
}

void PrimitiveRenderer::QuadArg::setColorHorizontal(const Color4f& colorL, const Color4f& colorR)
{
    mHorizontal = true;
    mColor0 = colorL;
    mColor1 = colorR;
}

void PrimitiveRenderer::CubeArg::setCornerAndSize(const Vector3f& corner, const Vector3f& size)
{
    mCenter = (size * 0.5f) += corner;
    mSize = size;
}

void PrimitiveRenderer::CubeArg::setBoundBox(const BoundBox3f& bound_box)
{
    Vector3f center;
    bound_box.getCenter(&center);

    mCenter = center;
    mSize.set(bound_box.getSizeX(), bound_box.getSizeY(), bound_box.getSizeZ());
}

} // namespace sead
