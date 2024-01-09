#include <random/seadRandom.h>
#include <time/seadTickTime.h>

namespace sead {

void Random::init()
{
    init(static_cast<u32>(TickTime().toU64()));
}

void Random::init(u32 seed)
{
    static const u32 a = 1812433253;
    mX = (seed ^ (seed >> 30)) * a + 1;
    mY = (mX ^ (mX >> 30)) * a + 2;
    mZ = (mY ^ (mY >> 30)) * a + 3;
    mW = (mZ ^ (mZ >> 30)) * a + 4;
}

void Random::init(u32 seed_0, u32 seed_1, u32 seed_2, u32 seed_3)
{
    if (seed_0 == 0 && seed_1 == 0 && seed_2 == 0 && seed_3 == 0)
    {
      //SEAD_ASSERT_MSG(false, "seeds must not be all zero.");
        init(0);
    }
    else
    {
        mX = seed_0;
        mY = seed_1;
        mZ = seed_2;
        mW = seed_3;
    }
}

u32 Random::getU32()
{
    u32 t = (mX^(mX<<11));
    mX = mY;
    mY = mZ;
    mZ = mW;
    mW = (mW^(mW>>19))^(t^(t>>8));
    return mW;
}

void Random::getContext(u32* num_0, u32* num_1, u32* num_2, u32* num_3) const
{
    *num_0 = mX;
    *num_1 = mY;
    *num_2 = mZ;
    *num_3 = mW;
}

} // namespace sead
