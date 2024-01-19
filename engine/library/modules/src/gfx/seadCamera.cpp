#include <gfx/seadCamera.h>

namespace sead {

void Camera::getWorldPosByMatrix(Vector3f* dst) const
{
    const Matrix34f& view_mtx = getViewMatrix();

    f32 z = (-view_mtx(0, 2) * view_mtx(0, 3) - view_mtx(1, 2) * view_mtx(1, 3)) - view_mtx(2, 2) * view_mtx(2, 3);
    f32 y = (-view_mtx(0, 1) * view_mtx(0, 3) - view_mtx(1, 1) * view_mtx(1, 3)) - view_mtx(2, 1) * view_mtx(2, 3);
    f32 x = (-view_mtx(0, 0) * view_mtx(0, 3) - view_mtx(1, 0) * view_mtx(1, 3)) - view_mtx(2, 0) * view_mtx(2, 3);

    dst->set(x, y, z);
}

LookAtCamera::LookAtCamera(const Vector3f& pos, const Vector3f& at, const Vector3f& up)
    : mPos(pos)
    , mAt(at)
    , mUp(up)
{
    // SEAD_ASSERT(mPos != mAt);
    mUp.normalize();
}

LookAtCamera::~LookAtCamera()
{
}

void LookAtCamera::doUpdateMatrix(Matrix34f* dst) const
{
    if (mPos == mAt)
    {
        // SEAD_WARNING(false, "Warning! LookAtCamera::mPos == LookAtCamera::mAt\n");
        return;
    }

    sead::Vector3f dir = mPos;
    dir -= mAt;
    dir.normalize();

    sead::Vector3f right;
    right.setCross(mUp, dir);
    right.normalize();

    sead::Vector3f up;
    up.setCross(dir, right);

    f32 x = -right.dot(mPos);
    f32 y = -up.dot(mPos);
    f32 z = -dir.dot(mPos);

    (*dst)(0, 0) = right.x;
    (*dst)(0, 1) = right.y;
    (*dst)(0, 2) = right.z;
    (*dst)(0, 3) = x;

    (*dst)(1, 0) = up.x;
    (*dst)(1, 1) = up.y;
    (*dst)(1, 2) = up.z;
    (*dst)(1, 3) = y;

    (*dst)(2, 0) = dir.x;
    (*dst)(2, 1) = dir.y;
    (*dst)(2, 2) = dir.z;
    (*dst)(2, 3) = z;
}

}
