#ifndef SEAD_PROJECTION_H_
#define SEAD_PROJECTION_H_

#include <gfx/seadGraphics.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class Camera;
class Viewport;

template <typename T>
class Ray;

class Projection
{
    SEAD_RTTI_BASE(Projection)

public:
    enum Type
    {
        cType_Perspective = 0,
        cType_Ortho       = 1,
        cType_Undefined   = 2
    };

public:
    Projection();
    virtual ~Projection();

    void setDirty()
    {
        mDirty = true;
    }

    void setDeviceDirty()
    {
        mDeviceDirty = true;
    }

    void setDevicePosture(Graphics::DevicePosture pose)
    {
        mDevicePosture = pose;
        setDeviceDirty();
    }

    const Matrix44f& getProjectionMatrix() const;
    Matrix44f& getProjectionMatrixMutable();

    const Matrix44f& getDeviceProjectionMatrix() const;

    void cameraPosToScreenPos(Vector3f* dst, const Vector3f& camera_pos) const;
    void screenPosToCameraPos(Vector3f* dst, const Vector3f& screen_pos) const;
    void screenPosToCameraPos(Vector3f* dst, const Vector2f& screen_pos) const;

    void project(Vector2f* dst, const Vector3f& camera_pos, const Viewport& viewport) const;
    void unproject(Vector3f* dst, const Vector3f& screen_pos, const Camera& camera) const;
    void unprojectRay(Ray<Vector3f>* dst, const Vector3f& screen_pos, const Camera& camera) const;

    virtual Type getProjectionType() const = 0;
    virtual void doUpdateMatrix(Matrix44f* dst) const = 0;
    virtual void doUpdateDeviceMatrix(Matrix44f* dst, const Matrix44f& src, Graphics::DevicePosture pose) const;
    virtual void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const = 0;

private:
    void updateMatrixImpl_() const;

private:
    mutable bool mDirty;
    mutable bool mDeviceDirty;
    Matrix44f mMatrix;
    Matrix44f mDeviceMatrix;
    Graphics::DevicePosture mDevicePosture;
    f32 mDeviceZScale;
    f32 mDeviceZOffset;
};
#ifdef cafe
static_assert(sizeof(Projection) == 0x94, "sead::Projection size mismatch");
#endif // cafe

class PerspectiveProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(PerspectiveProjection, Projection)

public:
    PerspectiveProjection();
    PerspectiveProjection(f32 near, f32 far, f32 fovy_rad, f32 aspect);
    virtual ~PerspectiveProjection();

    void set(f32 _near, f32 _far, f32 fovy_rad, f32 aspect);

    void setNear(f32 near)
    {
        mNear = near;
        setDirty();
    }

    void setFar(f32 far)
    {
        mFar = far;
        setDirty();
    }

    void setFovx(f32 fovx);

    void setFovy(f32 fovy)
    {
        setFovy_(fovy);
    }

    void setAspect(f32 aspect)
    {
        mAspect = aspect;
        setDirty();
    }

    void setOffset(const Vector2f& offset)
    {
        mOffset = offset;
        setDirty();
    }

    void createDividedProjection(PerspectiveProjection* dst, s32 partno_x, s32 partno_y, s32 divnum_x, s32 divnum_y) const;

    f32 getNear() const { return mNear; }
    f32 getFar() const { return mFar; }
    f32 getFovy() const { return mAngle; }
    f32 getAspect() const { return mAspect; }
    const Vector2f& getOffset() const { return mOffset; }

    f32 getTop() const;
    f32 getBottom() const;
    f32 getLeft() const;
    f32 getRight() const;

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

    virtual Type getProjectionType() const { return Projection::cType_Perspective; }
    virtual void doUpdateMatrix(Matrix44f* dst) const;
    virtual void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const;

private:
    void setFovy_(f32 fovy);

    f32 calcNearClipHeight_() const
    {
        return mNear * 2 * mFovyTan;
    }

    f32 calcNearClipWidth_() const
    {
        return calcNearClipHeight_() * mAspect;
    }

private:
    f32 mNear;
    f32 mFar;
    f32 mAngle;
    f32 mFovySin;
    f32 mFovyCos;
    f32 mFovyTan;
    f32 mAspect;
    Vector2f mOffset;
};
#ifdef cafe
static_assert(sizeof(PerspectiveProjection) == 0xB8, "sead::PerspectiveProjection size mismatch");
#endif // cafe

class OrthoProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(OrthoProjection, Projection)

public:
    OrthoProjection();
    OrthoProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right);
    OrthoProjection(f32 _near, f32 _far, const BoundBox2f& box);
    OrthoProjection(f32 _near, f32 _far, const Viewport& vp);
    virtual ~OrthoProjection();

    void setNear(f32 near)
    {
        mNear = near;
        setDirty();
    }

    void setFar(f32 far)
    {
        mFar = far;
        setDirty();
    }

    void setTop(f32 top)
    {
        mTop = top;
        setDirty();
    }

    void setBottom(f32 bottom)
    {
        mBottom = bottom;
        setDirty();
    }

    void setLeft(f32 left)
    {
        mLeft = left;
        setDirty();
    }

    void setRight(f32 right)
    {
        mRight = right;
        setDirty();
    }

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

    void setByViewport(const Viewport& vp);
    void setBoundBox(const BoundBox2f& box);

    void createDividedProjection(OrthoProjection* dst, s32 partno_x, s32 partno_y, s32 divnum_x, s32 divnum_y) const;

    f32 getNear() const { return mNear; }
    f32 getFar() const { return mFar; }
    f32 getTop() const { return mTop; }
    f32 getBottom() const { return mBottom; }
    f32 getLeft() const { return mLeft; }
    f32 getRight() const { return mRight; }

    virtual Type getProjectionType() const { return Projection::cType_Ortho; }
    virtual void doUpdateMatrix(Matrix44f* dst) const;
    virtual void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const;

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};
#ifdef cafe
static_assert(sizeof(OrthoProjection) == 0xAC, "sead::OrthoProjection size mismatch");
#endif // cafe

class FrustumProjection : public Projection
{
    SEAD_RTTI_OVERRIDE(FrustumProjection, Projection)

public:
    FrustumProjection();
    FrustumProjection(f32 _near, f32 _far, f32 top, f32 bottom, f32 left, f32 right);
    FrustumProjection(f32 _near, f32 _far, const BoundBox2f& box);
    virtual ~FrustumProjection();

    virtual Type getProjectionType() const { return Projection::cType_Perspective; }
    virtual void doUpdateMatrix(Matrix44f* dst) const;
    virtual void doScreenPosToCameraPosTo(Vector3f* dst, const Vector3f& screen_pos) const;

    void setNear(f32 near)
    {
        mNear = near;
        setDirty();
    }

    void setFar(f32 far)
    {
        mFar = far;
        setDirty();
    }

    void setTop(f32 top)
    {
        mTop = top;
        setDirty();
    }

    void setBottom(f32 bottom)
    {
        mBottom = bottom;
        setDirty();
    }

    void setLeft(f32 left)
    {
        mLeft = left;
        setDirty();
    }

    void setRight(f32 right)
    {
        mRight = right;
        setDirty();
    }

    void setTBLR(f32 top, f32 bottom, f32 left, f32 right);

    void setBoundBox(const BoundBox2f& box);

    void createDividedProjection(FrustumProjection* dst, s32 partno_x, s32 partno_y, s32 divnum_x, s32 divnum_y) const;

    f32 getNear() const { return mNear; }
    f32 getFar() const { return mFar; }
    f32 getTop() const { return mTop; }
    f32 getBottom() const { return mBottom; }
    f32 getLeft() const { return mLeft; }
    f32 getRight() const { return mRight; }

    f32 getFovy() const;
    f32 getAspect() const;
    void getOffset(Vector2f* dst) const;
    f32 getOffsetX() const;
    f32 getOffsetY() const;

    void setFovyAspectOffset(f32 fovy, f32 aspect, const Vector2f& offset);

private:
    f32 mNear;
    f32 mFar;
    f32 mTop;
    f32 mBottom;
    f32 mLeft;
    f32 mRight;
};
#ifdef cafe
static_assert(sizeof(FrustumProjection) == 0xAC, "sead::FrustumProjection size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_PROJECTION_H_
