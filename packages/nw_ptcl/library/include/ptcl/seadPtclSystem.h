#ifndef SEAD_PTCL_SYSTEM_H_
#define SEAD_PTCL_SYSTEM_H_

#include <gfx/seadColor.h>
#include <gfx/seadGraphics.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <math/seadMatrix.h>
#include <time/seadTickTime.h>

#include <nw/effect.h>

namespace nw { namespace eftvw { // TODO: Implement this properly

class ToolConnecter
{
public:
    static bool IsConnected()
    {
        return s_isConnected;
    }

private:
    static volatile bool   s_isConnected;
};

class ViewerSystem
{
public:
    ViewerSystem(nw::eft::Heap* heap, nw::eft::Heap* connectionHeap, nw::eft::System* system);
    
    void UnbindResource(u32 resourceId);

    bool ProcCalc(f32 frameRate);

    void ExecuteCommand();

    bool IsPause()
    {
        return m_IsPaused;
    }

    void SetViewerCenter(nw::math::Matrix34 viewerCenter)
    {
        m_ViewerCenter = viewerCenter;
    }

    void SetViewerColor(f32 r, f32 g, f32 b, f32 a)
    {
        m_ViewerColor.Set(r, g, b, a);
    }

private:
    u32 _0[0xA14 / sizeof(u32)];
    bool                            m_DoLoop;
    bool                            m_IsPaused;
    u32 _a18[(0xA28 - 0xA18) / sizeof(u32)];
    nw::math::Matrix34              m_ViewerCenter;
    nw::ut::FloatColor              m_ViewerColor;
    u32 _a68[(0x13CC - 0xA68) / sizeof(u32)];
};
static_assert(sizeof(ViewerSystem) == 0x13CC);

} }

namespace sead { namespace ptcl {

enum
{
    cEftMaxGroup = nw::eft::EFT_GROUP_MAX,
    cEftViewerGroupID = cEftMaxGroup - 1
};

class Config : public nw::eft::Config
{
public:
    Config()
        : nw::eft::Config()
        , mpHeap()
        , mpViewerSysHeap(nullptr)
        , _28()
        , _29()
        , _2a(7)
        , _2b(1)
        , _2c()
        , _2d(0x12)
    {
    }

//private:
    ::sead::Heap* mpHeap;
    ::sead::Heap* mpViewerSysHeap;
    u8 _28;
    u8 _29;
    u8 _2a;
    u8 _2b;
    u8 _2c;
    u8 _2d;
};
static_assert(sizeof(Config) == 0x30);

class Heap : public nw::eft::Heap
{
public:
    Heap()
    {
    }

    explicit Heap(::sead::Heap* heap)
        : mpHeap(heap)
    {
    }

    virtual void* Alloc(u32 size, s32 alignment)
    {
        return mpHeap->alloc(size, alignment);
    }

    virtual void Free(void* ptr)
    {
        mpHeap->free(ptr);
    }

//private:
    ::sead::Heap* mpHeap;
};
static_assert(sizeof(Heap) == 8, "sead::ptcl::Heap size mismatch");

class PtclWorldScale
{
public:
    PtclWorldScale()
    {
        unkParam1Max = 100.0f;
        unkParam2Max = 10.0f;
        unkParam3Max = 1.0f;
        unkParam4Max = 10.0f;
        unkParam5Max = 1000;
        update();
    }

    void update()
    {
        unkParam1Meta                    .format("Min=%5.1f,Max=%5.1f",                            -unkParam1Max, unkParam1Max);
        unkParam1PositiveMeta            .format("Min=0,Max=%5.1f",                                unkParam1Max);
        unkParam3Meta                    .format("Min=%5.1f,Max=%5.1f",                            -unkParam3Max, unkParam3Max);
        unkParam3PositiveMeta            .format("Min=0,Max=%5.1f",                                unkParam3Max);
        unkParam3PositiveFractionMeta    .format("Min=0,Max=%5.1f",                                unkParam3Max / 10.0f);
        unkParam2Meta                    .format("Min=%5.1f,Max=%5.1f,Menu=True,MenuDefault=Auto", -unkParam2Max, unkParam2Max);
        unkParam2PositiveMeta            .format("Min=0,Max=%5.1f,Menu=True,MenuDefault=Auto",     unkParam2Max);
        unkParam4Meta                    .format("Min=%5.1f,Max=%5.1f,Menu=True,MenuDefault=Auto", -unkParam4Max, unkParam4Max);
        unkParam4PositiveMeta            .format("Min=0,Max=%5.1f,Menu=True,MenuDefault=Auto",     unkParam4Max);
        unkParam5Meta                    .format("Min=0,Max=%d",                                   unkParam5Max);
        unkParam5NonZeroMeta             .format("Min=1,Max=%d",                                   unkParam5Max);
        unkParam1DisableMeta             .format("%s,IsEnable=False",                              unkParam1Meta.cstr());
        unkParam1PositiveDisableMeta     .format("%s,IsEnable=False",                              unkParam1PositiveMeta.cstr());
        unkParam3PositiveDisableMeta     .format("%s,IsEnable=False",                              unkParam3PositiveMeta.cstr());
        unkParam2DisableMeta             .format("%s,IsEnable=False",                              unkParam2Meta.cstr());
        unkParam4DisableMeta             .format("%s,IsEnable=False",                              unkParam4Meta.cstr());
        unkParam4PositiveDisableMeta     .format("%s,IsEnable=False",                              unkParam4PositiveMeta.cstr());
        unkParam5DisableMeta             .format("%s,IsEnable=False",                              unkParam5Meta.cstr());
        unkParam2PositiveDisableMeta     .format("%s,IsEnable=False",                              unkParam2PositiveMeta.cstr());
        unkParam5NonZeroDisableMeta      .format("%s,IsEnable=False",                              unkParam5NonZeroMeta.cstr());
        unkParam3PositiveWithMenuMeta    .format("%s,Menu=True,MenuDefault=Auto",                  unkParam3PositiveMeta.cstr());
        unkParam5ScaledMeta              .format("Min=0,Max=%d",                                   unkParam5Max * 10);
        unkParam5ScaledNonZeroMeta       .format("Min=1,Max=%d",                                   unkParam5Max * 10);
        unkParam5ScaledDisableMeta       .format("%s,IsEnable=False",                              unkParam5ScaledMeta.cstr());
        unkParam5ScaledNonZeroDisableMeta.format("%s,IsEnable=False",                              unkParam5ScaledNonZeroMeta.cstr());
    }

private:
    f32 unkParam1Max; // Min = Negation of this
    f32 unkParam2Max; // ^^^
    f32 unkParam3Max; // ^^^
    f32 unkParam4Max; // ^^^
    s32 unkParam5Max; // ^^^
    FixedSafeString<64> unkParam1Meta;
    FixedSafeString<64> unkParam1PositiveMeta;
    FixedSafeString<64> unkParam3Meta;
    FixedSafeString<64> unkParam3PositiveMeta;
    FixedSafeString<64> unkParam3PositiveFractionMeta;
    FixedSafeString<96> unkParam3PositiveWithMenuMeta;
    FixedSafeString<96> unkParam2Meta;
    FixedSafeString<96> unkParam2PositiveMeta;
    FixedSafeString<96> unkParam4Meta;
    FixedSafeString<96> unkParam4PositiveMeta;
    FixedSafeString<64> unkParam5Meta;
    FixedSafeString<64> unkParam5NonZeroMeta;
    FixedSafeString<64> unkParam1DisableMeta;
    FixedSafeString<64> unkParam1PositiveDisableMeta;
    FixedSafeString<64> unkParam3PositiveDisableMeta;
    FixedSafeString<96> unkParam2DisableMeta;
    FixedSafeString<96> unkParam2PositiveDisableMeta;
    FixedSafeString<64> unkParam4DisableMeta;
    FixedSafeString<96> unkParam4PositiveDisableMeta;
    FixedSafeString<64> unkParam5DisableMeta;
    FixedSafeString<64> unkParam5NonZeroDisableMeta;
    FixedSafeString<64> unkParam5ScaledMeta;
    FixedSafeString<64> unkParam5ScaledNonZeroMeta;
    FixedSafeString<64> unkParam5ScaledDisableMeta;
    FixedSafeString<64> unkParam5ScaledNonZeroDisableMeta;
};
static_assert(sizeof(PtclWorldScale) == 0x880);

class PtclEditorInterface
{
public:
    PtclEditorInterface()
        : _8c8(0)
    {
        mCenter = Matrix34f::ident;
        _8cc = Matrix34f::ident;
        _4 = 0;
        _8fc = 1.0f;

        for (s32 i = 0; i < 16; i++)
            _900[i].format("[%d]", i);

        _dc0[0] = "”’l‚P";
        _dc0[1] = "”’l‚Q";

        mColor.setf(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void applyViewerCenter(nw::eftvw::ViewerSystem* p_system)
    {
        setViewerCenter(p_system, mCenter);
    }

    void applyViewerColor(nw::eftvw::ViewerSystem* p_system)
    {
        setViewerColor(p_system, mColor.r, mColor.g, mColor.b, mColor.a);
    }

    static void setViewerCenter(nw::eftvw::ViewerSystem* p_system, const Matrix34f& center)
    {
        p_system->SetViewerCenter(reinterpret_cast<const nw::math::Matrix34&>(center.a[0]));
    }

    static void setViewerColor(nw::eftvw::ViewerSystem* p_system, f32 r, f32 g, f32 b, f32 a)
    {
        if (p_system != nullptr)
            p_system->SetViewerColor(r, g, b, a);
    }

private:
    u32 _0[4 / sizeof(u32)];
    u32 _4;
    PtclWorldScale mWorldScale;
    Matrix34f mCenter;
    Color4f mColor;
    u32 _8c8;
    Matrix34f _8cc;
    f32 _8fc;
    FixedSafeString<64> _900[16];
    FixedSafeString<64> _dc0[2];
};
static_assert(sizeof(PtclEditorInterface) == 0xE58, "sead::ptcl::PtclEditorInterface size mismatch");

class PtclSystem : public nw::eft::System
{
public:
    PtclSystem(const Config& config)
        : nw::eft::System(config)
    {
        mHeap.mpHeap = config.mpHeap;
        Initialize(&mHeap, config);

        if (config.mpViewerSysHeap != nullptr)
        {
            mViewerSysHeap.mpHeap = config.mpViewerSysHeap;
            mViewerSysHeap.mpHeap->setEnableLock(true);
            mpViewerSys = new nw::eftvw::ViewerSystem(&mViewerSysHeap, &mViewerSysHeap, this);
        }
        else
        {
            mpViewerSys = nullptr;
        }

        mSeadHeapArray = static_cast< ::sead::Heap** >(config.mpHeap->alloc(sizeof(::sead::Heap*) * config.GetResourceNum(), 4));
        std::memset(mSeadHeapArray, 0, sizeof(::sead::Heap*) * config.GetResourceNum());
    }

    void entryResource(::sead::Heap* heap, void* resource, s32 resId)
    {
        Graphics::instance()->lockDrawContext();
        {
            mSeadHeapArray[resId] = heap;
            Heap heap_(mSeadHeapArray[resId]);
            EntryResource(&heap_, resource, resId);
        }
        Graphics::instance()->unlockDrawContext();
    }

    void clearResource(s32 resId)
    {
        if (mpViewerSys != nullptr)
            mpViewerSys->UnbindResource(resId);

        Heap heap(mSeadHeapArray[resId]);
        // SOME_MYSTERIOUS_ASSERT_FUNCTION(mSeadHeapArray[resId]);

        ClearResource(&heap, resId);

        mSeadHeapArray[resId] = nullptr;
    }

    bool createEmitterSetID(nw::eft::Handle* handle, const Vector3f& pos, s32 emitterSetID, s32 resourceID = 0, s32 groupID = 0, u32 emitterMask = 0xffffffff)
    {
        return CreateEmitterSetID(handle, reinterpret_cast<const nw::math::VEC3&>(pos.x), emitterSetID, resourceID, groupID, emitterMask);
    }

    bool createEmitterSetID(nw::eft::Handle* handle, const Matrix34f& mtx, s32 emitterSetID, s32 resourceID = 0, s32 groupID = 0, u32 emitterMask = 0xffffffff)
    {
        return CreateEmitterSetID(handle, reinterpret_cast<const nw::math::MTX34&>(mtx.a[0]), emitterSetID, resourceID, groupID, emitterMask);
    }

    void calcEmitter(s32 groupID, f32 frameRate = 1.0f)
    {
        if (groupID != cEftViewerGroupID)
            CalcEmitter(groupID, frameRate);

        else
        {
            if (mpViewerSys != nullptr && !mpViewerSys->IsPause())
                CalcEmitter(cEftViewerGroupID, frameRate);
        }
    }

    void beginFrame()
    {
        if (mpViewerSys != nullptr && nw::eftvw::ToolConnecter::IsConnected())
        {
            Graphics::instance()->lockDrawContext();
            {
                mEditorInterface.applyViewerCenter(mpViewerSys);
                mEditorInterface.applyViewerColor(mpViewerSys);

                CurrentHeapSetter chs(mViewerSysHeap.mpHeap);
                mpViewerSys->ExecuteCommand();
            }
            Graphics::instance()->unlockDrawContext();

            mpViewerSys->ProcCalc(1.0f);
        }

        BeginFrame();
    }

private:
    TickTime mTime;
    Heap mHeap;
    Heap mViewerSysHeap;
    Heap _a40;
    u8 _a48[4];
    ::sead::Heap** mSeadHeapArray;
    nw::eftvw::ViewerSystem* mpViewerSys;
    PtclEditorInterface mEditorInterface;
    u8 _18ac[4];
};
static_assert(sizeof(PtclSystem) == 0x18B0, "sead::ptcl::PtclSystem size mismatch");

} }

#endif // SEAD_PTCL_SYSTEM_H_
