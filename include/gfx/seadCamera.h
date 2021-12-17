#ifndef SEAD_CAMERA_H_
#define SEAD_CAMERA_H_

#include <math/seadMatrix.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class Camera
{
    SEAD_RTTI_BASE(Camera)

public:
    Camera()
        : mMatrix(Matrix34f::ident)
    {
    }

    virtual ~Camera();

    virtual void doUpdateMatrix(Matrix34f* dst) const = 0;

    Matrix34f& getMatrix() { return mMatrix; }
    const Matrix34f& getMatrix() const { return mMatrix; }

protected:
    Matrix34f mMatrix;
};
#ifdef cafe
static_assert(sizeof(Camera) == 0x34, "sead::Camera size mismatch");
#endif // cafe

class LookAtCamera : public Camera
{
    SEAD_RTTI_OVERRIDE(LookAtCamera, Camera)

public:
    LookAtCamera()
        : Camera()
        , mPos(0.0f, 0.0f, 10.0f)
        , mAt(0.0f, 0.0f, 0.0f)
        , mUp(0.0f, 1.0f, 0.0f)
    {
    }

    LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up);
    virtual ~LookAtCamera();

    virtual void doUpdateMatrix(Matrix34f* dst) const;

    Vector3f& getPos() { return mPos; }
    const Vector3f& getPos() const { return mPos; }

    Vector3f& getAt() { return mAt; }
    const Vector3f& getAt() const { return mAt; }

    Vector3f& getUp() { return mUp; }
    const Vector3f& getUp() const { return mUp; }

private:
    Vector3f mPos;
    Vector3f mAt;
    Vector3f mUp;
};
#ifdef cafe
static_assert(sizeof(LookAtCamera) == 0x58, "sead::LookAtCamera size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_CAMERA_H_
