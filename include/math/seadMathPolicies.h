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

} // namespace sead

#endif // SEAD_MATHPOLICIES_H_
