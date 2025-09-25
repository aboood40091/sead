#ifndef SEAD_BIT_FLAG_H_
#define SEAD_BIT_FLAG_H_

#include <basis/seadTypes.h>

namespace sead {

class BitFlagUtil
{
public:
    static s32 countOnBit(u32 x);

    static s32 countContinuousOffBitFromRight(u32 x)
    {
        return countOnBit((x & -x) - 1);
    }

    static s32 countRightOnBit(u32 x, s32 bit);
    static s32 findOnBitFromRight(u32 x, s32 num);

    static s32 countOnBit64(u64 x)
    {
        return countOnBit(static_cast<u32>(x)) + countOnBit(static_cast<u32>(x >> 32));
    }

    static s32 countContinuousOffBitFromRight64(u64 x)
    {
        return countOnBit64((x & -x) - 1);
    }

    static s32 countRightOnBit64(u64 x, s32 bit);
    static s32 findOnBitFromRight64(u64 x, s32 num);
};

template <typename T>
class BitFlag
{
public:
    BitFlag()
        : mBits(0)
    {
    }

    explicit BitFlag(T t)
        : mBits(t)
    {
    }

    operator T() const { return mBits; }

    void makeAllZero() { mBits = 0; }
    void makeAllOne() { mBits = T(-1); }

    void setDirect(T bits) { mBits = bits; }
    T getDirect() const { return mBits; }
    T* getPtr() { return &mBits; }
    const T* getPtr() const { return &mBits; }
    size_t getByteSize() const { return sizeof(T); }

    void set(T mask) { mBits |= mask; }
    void reset(T mask) { mBits &= ~mask; }
    void toggle(T mask) { mBits ^= mask; }
    void change(T mask, bool b) { b ? set(mask) : reset(mask); }

    bool isZero() const { return mBits == 0; }
    bool isOn(T mask) const { return (mBits & mask) != 0; }
    bool isOnAll(T mask) const { return (mBits & mask) == mask; }
    bool isOff(T mask) const { return !isOn(mask); }

    T getMask(T mask) const { return mBits & mask; }

    static T makeMask(s32 bit)
    {
        //SEAD_ASSERT(static_cast<u32>(bit) < sizeof(T)*8);
        return T(1) << bit;
    }

    void setBit(s32 bit) { set(makeMask(bit)); }
    void resetBit(s32 bit) { reset(makeMask(bit)); }
    void changeBit(s32 bit, bool b) { change(makeMask(bit), b); }
    void toggleBit(s32 bit) { toggle(makeMask(bit)); }

    bool isOnBit(s32 bit) const { return isOn(makeMask(bit)); }
    bool isOffBit(s32 bit) const { return isOff(makeMask(bit)); }

    s32 countOnBit() const;
    inline s32 countContinuousOffBitFromRight() const;
    s32 countRightOnBit(s32) const;
    s32 findOnBitFromRight(s32) const;

protected:
    T mBits;
};

typedef BitFlag<u8> BitFlag8;
typedef BitFlag<u16> BitFlag16;
typedef BitFlag<u32> BitFlag32;

#ifdef cafe
static_assert(sizeof(BitFlag8) == 1, "sead::BitFlag8 size mismatch");
static_assert(sizeof(BitFlag16) == 2, "sead::BitFlag16 size mismatch");
static_assert(sizeof(BitFlag32) == 4, "sead::BitFlag32 size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_BIT_FLAG_H_
