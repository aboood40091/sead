#ifndef SEAD_CORE_INFO_H_
#define SEAD_CORE_INFO_H_

#include <basis/seadTypes.h>

#include <cafe/os.h>

namespace sead {

class CoreId
{
public:
    enum ValueType
    {
        cDontUseThis_StartNumMinus1 = -1,
        cMain,
        cSub1,
        cSub2,
        cSub3,
        cSub4,
        cSub5,
        cSub6,
        cSub7,
        cSub8,
        cSub9,
        cSub10,
        cSub11,
        cSub12,
        cSub13,
        cSub14,
        cSub15,
        cSub16,
        cSub17,
        cSub18,
        cSub19,
        cSub20,
        cSub21,
        cSub22,
        cSub23,
        cSub24,
        cSub25,
        cSub26,
        cSub27,
        cSub28,
        cSub29,
        cSub30,
        cSub31,
        cSub32,
        cUndef,
        cDontUseThis_MaxNumPlus1,
        cDontUseThis_MemSize32bit = 2147483647,
        cEnumStart = 0,
        cEnumMax = 33
    };
    static_assert(sizeof(ValueType) == 4, "sead::CoreId::ValueType size mismatch");

public:
    CoreId()
        : mValue(cMain)
    {
    }

    CoreId(ValueType value)
        : mValue(value)
    {
    }

    CoreId(s32 value)
    {
        // SEAD_ASSERT_MSG(value >= cMain && value < cDontUseThis_MaxNumPlus1, "range over: %d, [%d - %d)", value, s32(cMain), s32(cDontUseThis_MaxNumPlus1));
        mValue = value;
    }

    ValueType value() const
    {
        return ValueType(mValue);
    }

    operator s32() const
    {
        return mValue;
    }

    bool operator==(ValueType value) const
    {
        return this->value() == value;
    }

protected:
    s32 mValue;
};
static_assert(sizeof(CoreId) == 4, "sead::CoreId size mismatch");

class CoreIdMask
{
public:
    CoreIdMask()
        : mMask(0)
    {
    }

    CoreIdMask(CoreId id)
        : mId(id)
    {
        mMask = getCoreMask_(mId);
    }

    CoreIdMask(CoreId id_1, CoreId id_2)
    {
        mMask = getCoreMask_(id_1) |
                getCoreMask_(id_2);
    }

    CoreIdMask(CoreId id_1, CoreId id_2, CoreId id_3)
    {
        mMask = getCoreMask_(id_1) |
                getCoreMask_(id_2) |
                getCoreMask_(id_3);
    }

    void set(u32 mask)
    {
        mMask = mask;
    }

    operator u32() const
    {
        return mMask;
    }

    CoreIdMask& OR(CoreId id)
    {
        mMask |= getCoreMask_(id);
        return *this;
    }

    bool isOn(const CoreId& id) const
    {
        return mMask & getCoreMask_(id);
    }

    void setOff(CoreId id)
    {
        mMask &= ~getCoreMask_(id);
    }

    void setOn(CoreId id)
    {
        mMask |= getCoreMask_(id);
    }

    void clear()
    {
        mMask = 0;
    }

    u32 countOnBits() const;

protected:
    u32 getCoreMask_(CoreId id) const
    {
        return 1u << s32(id);
    }

protected:
    u32 mMask;
    CoreId mId;
};
static_assert(sizeof(CoreIdMask) == 8, "sead::CoreIdMask size mismatch");

class CoreInfo
{
public:
    static void configure();
    static void dump();

    static u32 getNumCores()
    {
        return sNumCores;
    }

    static u32 getPlatformMainCoreId()
    {
        return sPlatformCoreId[CoreId::cMain];
    }

    static u32 getPlatformCoreId(CoreId id)
    {
        return sPlatformCoreId[s32(id)];
    }

    static CoreId getCurrentCoreId()
    {
        return getCoreIdFromPlatformCoreId(OSGetCoreId());
    }

    static u32 getPlatformMask(CoreId id);

    static CoreId getCoreIdFromPlatformCoreId(u32 id)
    {
        return sCoreIdFromPlatformCoreIdTable[id];
    }

    static u32 getPlatformMaskFromCoreIdMask(const CoreIdMask& mask);

    static CoreIdMask getMaskAll()
    {
        CoreIdMask mask;
        mask.set((1 << sNumCores) - 1);
        return mask;
    }

    static CoreIdMask getMaskSubAll()
    {
        CoreIdMask mask = getMaskAll();
        mask.set(u32(mask) - 1);
        return mask;
    }

protected:
    static u32 sNumCores;
    static u32 sPlatformCoreId[32];
    static CoreId sCoreIdFromPlatformCoreIdTable[32];
};

} // namespace sead

#endif // SEAD_CORE_INFO_H_
