#ifndef SEAD_MATHPOLICIES_H_
#define SEAD_MATHPOLICIES_H_

namespace sead {

template <typename T>
class Policies
{
public:
    struct Vec2Base
    {
        T x;
        T y;
    };

    struct Vec3Base
    {
        T x;
        T y;
        T z;
    };

    struct Vec4Base
    {
        T x;
        T y;
        T z;
        T w;
    };

    struct QuatBase
    {
        T x;
        T y;
        T z;
        T w;
    };

    struct Mtx22Base
    {
        union
        {
            T m[2][2];
            T a[4];
        };
    };

    struct Mtx33Base
    {
        union
        {
            T m[3][3];
            T a[9];
        };
    };

    struct Mtx34Base
    {
        union
        {
            T m[3][4];
            T a[12];
        };
    };

    struct Mtx44Base
    {
        union
        {
            T m[4][4];
            T a[16];
        };
    };
};

#ifdef cafe
static_assert(sizeof(Policies<float>::Vec2Base) == 8, "sead::Policies<T>::Vec2Base size mismatch");
static_assert(sizeof(Policies<float>::Vec3Base) == 0xC, "sead::Policies<T>::Vec3Base size mismatch");
static_assert(sizeof(Policies<float>::Vec4Base) == 0x10, "sead::Policies<T>::Vec4Base size mismatch");
static_assert(sizeof(Policies<float>::QuatBase) == 0x10, "sead::Policies<T>::QuatBase size mismatch");
static_assert(sizeof(Policies<float>::Mtx22Base) == 0x10, "sead::Policies<T>::Mtx22Base size mismatch");
static_assert(sizeof(Policies<float>::Mtx33Base) == 0x24, "sead::Policies<T>::Mtx33Base size mismatch");
static_assert(sizeof(Policies<float>::Mtx34Base) == 0x30, "sead::Policies<T>::Mtx34Base size mismatch");
static_assert(sizeof(Policies<float>::Mtx44Base) == 0x40, "sead::Policies<T>::Mtx44Base size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_MATHPOLICIES_H_
