#ifndef SEAD_PRIMITIVE_RENDERER_H_
#define SEAD_PRIMITIVE_RENDERER_H_

#include <gfx/seadColor.h>
#include <gfx/seadPrimitiveRendererBase.h>
#include <heap/seadDisposer.h>
#include <math/seadBoundBox.h>

namespace sead {

class PrimitiveRenderer : public IDisposer
{
    SEAD_SINGLETON_DISPOSER(PrimitiveRenderer)

public:
    class QuadArg
    {
    public:
        QuadArg()
            : mCenter(Vector3f::zero)
            , mSize(Vector3f::ones) // Yes, not Vector2f::ones, for some reason
            , mColor0(Color4f::cWhite)
            , mColor1(Color4f::cWhite)
            , mHorizontal(false)
        {
        }

        QuadArg& setCenter(const Vector3f& p) { mCenter = p; return *this; }
        QuadArg& setSize(const Vector2f& size) { mSize = size; return *this; }
        QuadArg& setSize(f32 w, f32 h) { mSize.set(w, h); return *this; }
        QuadArg& setCornerAndSize(const Vector3f& p, const Vector2f& size);
        QuadArg& setBoundBox(const BoundBox2f& box, f32 z);
        QuadArg& setColor(const Color4f& colorT, const Color4f& colorB);
        QuadArg& setColor(const Color4f& color) { return setColor(color, color); }
        QuadArg& setColorHorizontal(const Color4f& colorL, const Color4f& colorR);

        const Vector3f& getCenter() const { return mCenter; }
        const Vector2f& getSize() const { return mSize; }
        const Color4f& getColor0() const { return mColor0; }
        const Color4f& getColor1() const { return mColor1; }
        bool isHorizontal() const { return mHorizontal; }

    private:
        Vector3f mCenter;
        Vector2f mSize;
        Color4f mColor0;
        Color4f mColor1;
        bool mHorizontal;
    };
#ifdef cafe
    static_assert(sizeof(QuadArg) == 0x38, "sead::PrimitiveRenderer::QuadArg size mismatch");
#endif // cafe

    class UVArg
    {
    public:
        UVArg()
            : mUVSrc(Vector2f::zero)
            , mUVSize(Vector2f::ones)
        {
        }

        UVArg& setUVSrc(const Vector2f& uv_src) { mUVSrc = uv_src; return *this; }
        UVArg& setUVSize(const Vector2f& uv_size) { mUVSize = uv_size; return *this; }

        const Vector2f& getUVSrc() const { return mUVSrc; }
        const Vector2f& getUVSize() const { return mUVSize; }

    private:
        Vector2f mUVSrc;
        Vector2f mUVSize;
    };
#ifdef cafe
    static_assert(sizeof(UVArg) == 0x10, "sead::PrimitiveRenderer::UVArg size mismatch");
#endif // cafe

    class CubeArg
    {
    public:
        CubeArg()
            : mCenter(Vector3f::zero)
            , mSize(Vector3f::ones)
            , mColor0(Color4f::cWhite)
            , mColor1(Color4f::cWhite)
        {
        }

        CubeArg& setCenter(const Vector3f& p) { mCenter = p; return *this; }
        CubeArg& setSize(const Vector3f& size) { mSize = size; return *this; }
        CubeArg& setCornerAndSize(const Vector3f& p, const Vector3f& size);
        CubeArg& setBoundBox(const BoundBox3f& box);
        CubeArg& setColor(const Color4f& c0, const Color4f& c1) { mColor0 = c0; mColor1 = c1; return *this; }
        CubeArg& setColor(const Color4f& color) { return setColor(color, color); }

        const Vector3f& getCenter() const { return mCenter; }
        const Vector3f& getSize() const { return mSize; }
        const Color4f& getColor0() const { return mColor0; }
        const Color4f& getColor1() const { return mColor1; }

    private:
        Vector3f mCenter;
        Vector3f mSize;
        Color4f mColor0;
        Color4f mColor1;
    };
#ifdef cafe
    static_assert(sizeof(CubeArg) == 0x38, "sead::PrimitiveRenderer::CubeArg size mismatch");
#endif // cafe

public:
    PrimitiveRenderer();
    virtual ~PrimitiveRenderer();

    void prepareFromBinary(Heap* heap, const void* bin_data, u32 bin_size);
    void prepare(Heap* heap, const SafeString& path);
    void setCamera(const Camera& camera);
    void setProjection(const Projection& projection);
    void setModelMatrix(const Matrix34f& model_to_world_matrix);
    void begin();
    void end();

    void drawQuad(const Color4f& colorL, const Color4f& colorR);
    void drawQuad(const Color4f& color) { drawQuad(color, color); }
    void drawQuad(const Texture& texture, const Color4f& colorL, const Color4f& colorR, const Vector2f& uv_src = Vector2f::zero, const Vector2f& uv_size = Vector2f::ones);
    void drawQuad(const Texture& texture, const Color4f& color = Color4f::cWhite, const Vector2f& uv_src = Vector2f::zero, const Vector2f& uv_size = Vector2f::ones) { drawQuad(texture, color, color, uv_src, uv_size); }
    void drawBox(const Color4f& colorL, const Color4f& colorR);
    void drawBox(const Color4f& color) { drawBox(color, color); }
    void drawCube(const Color4f& c0, const Color4f& c1);
    void drawCube(const Color4f& color) { drawCube(color, color); }
    void drawWireCube(const Color4f& c0, const Color4f& c1);
    void drawWireCube(const Color4f& color) { drawWireCube(color, color); }
    void drawLine(const Color4f& c0, const Color4f& c1);
    void drawLine(const Color4f& color) { drawLine(color, color); }
    void drawSphere4x8(const Color4f& north, const Color4f& south);
    void drawSphere4x8(const Color4f& color) { drawSphere4x8(color, color); }
    void drawSphere8x16(const Color4f& north, const Color4f& south);
    void drawSphere8x16(const Color4f& color) { drawSphere8x16(color, color); }
    void drawDisk16(const Color4f& center, const Color4f& edge);
    void drawDisk16(const Color4f& color) { drawDisk16(color, color); }
    void drawDisk32(const Color4f& center, const Color4f& edge);
    void drawDisk32(const Color4f& color) { drawDisk32(color, color); }
    void drawCircle16(const Color4f& color);
    void drawCircle32(const Color4f& color);
    void drawCylinder16(const Color4f& top, const Color4f& btm);
    void drawCylinder16(const Color4f& color) { drawCylinder16(color, color); }
    void drawCylinder32(const Color4f& top, const Color4f& btm);
    void drawCylinder32(const Color4f& color) { drawCylinder32(color, color); }

    void drawQuad(const QuadArg& arg);
    void drawQuad(const Texture& texture, const QuadArg& arg, const UVArg& uv_arg = UVArg());
    void drawBox(const QuadArg& arg);
    void drawCube(const CubeArg& arg);
    void drawWireCube(const CubeArg& arg);
    void drawLine(const Vector3f& from, const Vector3f& to, const Color4f& c0, const Color4f& c1);
    void drawLine(const Vector3f& from, const Vector3f& to, const Color4f& color);
    void drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south);
    void drawSphere4x8(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& north, const Color4f& south);
    void drawSphere8x16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawDisk16(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge);
    void drawDisk32(const Vector3f& pos, f32 radius, const Color4f& center, const Color4f& edge);
    void drawDisk16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawDisk32(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCircle16(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCircle32(const Vector3f& pos, f32 radius, const Color4f& color);
    void drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color);
    void drawCylinder16(const Vector3f& pos, f32 radius, f32 height, const Color4f& color);
    void drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& top_color, const Color4f& btm_color);
    void drawCylinder32(const Vector3f& pos, f32 radius, f32 height, const Color4f& color);

    void drawAxis(const Vector3f& pos, f32 scale);

private:
    void doPrepare_(Heap* heap);

private:
    PrimitiveRendererBase* mRendererImpl;
    Matrix34f mModelMtx;
};
#ifdef cafe
static_assert(sizeof(PrimitiveRenderer) == 0x54, "sead::PrimitiveRenderer size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_PRIMITIVE_RENDERER_H_
