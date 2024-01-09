#ifndef SEAD_RANDOM_H_
#define SEAD_RANDOM_H_

#include <basis/seadTypes.h>

namespace sead {

class Random
{
public:
    Random()
    {
        init();
    }

    explicit Random(u32 seed)
    {
        init(seed);
    }

    Random(u32 seed_0, u32 seed_1, u32 seed_2, u32 seed_3)
    {
        init(seed_0, seed_1, seed_2, seed_3);
    }

    void init();
    void init(u32 seed);
    void init(u32 seed_0, u32 seed_1, u32 seed_2, u32 seed_3);

    u32 getU32();

    u32 getU32(u32 ceil)
    {
        return static_cast<u32>(((u64)getU32() * ceil) >> 32);
    }

    s32 getS32Range(s32 a, s32 b)
    {
        return getU32(b - a) + a;
    }

    f32 getF32()
    {
        return getU32() * (1.0f / 4294967296.0f);
    }

    f32 getF32(f32 ceil)
    {
        return getF32() * ceil;
    }

    f32 getF32Range(f32 a, f32 b)
    {
        return getF32(b - a) + a;
    }

    f64 getF64()
    {
        return getU32() * (1.0 / 4294967296.0);
    }

    f64 getF64(f64 ceil)
    {
        return getF64() * ceil;
    }

    f64 getF64Range(f64 a, f64 b)
    {
        return getF64(b - a) + a;
    }

    s32 getSign()
    {
        return (getU32() & 2) - 1;
    }

    bool getBool()
    {
        return static_cast<bool>(getU32() & 1);
    }

    void getContext(u32* num_0, u32* num_1, u32* num_2, u32* num_3) const;

private:
    u32 mX;
    u32 mY;
    u32 mZ;
    u32 mW;
};
#ifdef cafe
static_assert(sizeof(Random) == 0x10, "sead::Random size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_RANDOM_H_
