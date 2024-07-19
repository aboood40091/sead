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

class PtclEditorInterface
{
public:
    PtclEditorInterface();

    void applyViewerCenter(nw::eftvw::ViewerSystem* p_system)
    {
        setViewerCenter(p_system, mCenter);
    }

    void applyViewerColor(nw::eftvw::ViewerSystem* p_system)
    {
        setViewerColor(p_system, mColor.r, mColor.g, mColor.b, mColor.a);
    }

    static void setViewerCenter(nw::eftvw::ViewerSystem* p_system, const sead::Matrix34f& center)
    {
        p_system->SetViewerCenter(reinterpret_cast<const nw::math::Matrix34&>(center.a[0]));
    }

    static void setViewerColor(nw::eftvw::ViewerSystem* p_system, f32 r, f32 g, f32 b, f32 a)
    {
        if (p_system != nullptr)
            p_system->SetViewerColor(r, g, b, a);
    }

private:
    u32 _0[0x888 / sizeof(u32)];
    sead::Matrix34f mCenter;
    sead::Color4f mColor;
    u32 _8c8[(0xE58 - 0x8C8) / sizeof(u32)];
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
        sead::Graphics::instance()->lockDrawContext();
        {
            mSeadHeapArray[resId] = heap;
            Heap heap_(mSeadHeapArray[resId]);
            EntryResource(&heap_, resource, resId);
        }
        sead::Graphics::instance()->unlockDrawContext();
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

    bool createEmitterSetID(nw::eft::Handle* handle, const sead::Vector3f& pos, s32 emitterSetID, s32 resourceID = 0, s32 groupID = 0, u32 emitterMask = 0xffffffff)
    {
        return CreateEmitterSetID(handle, reinterpret_cast<const nw::math::VEC3&>(pos.x), emitterSetID, resourceID, groupID, emitterMask);
    }

    bool createEmitterSetID(nw::eft::Handle* handle, const sead::Matrix34f& mtx, s32 emitterSetID, s32 resourceID = 0, s32 groupID = 0, u32 emitterMask = 0xffffffff)
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
            sead::Graphics::instance()->lockDrawContext();
            {
                mEditorInterface.applyViewerCenter(mpViewerSys);
                mEditorInterface.applyViewerColor(mpViewerSys);

                sead::CurrentHeapSetter chs(mViewerSysHeap.mpHeap);
                mpViewerSys->ExecuteCommand();
            }
            sead::Graphics::instance()->unlockDrawContext();

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
