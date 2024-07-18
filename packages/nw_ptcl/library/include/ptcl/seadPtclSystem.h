#ifndef SEAD_PTCL_SYSTEM_H_
#define SEAD_PTCL_SYSTEM_H_

#include <heap/seadHeap.h>
#include <time/seadTickTime.h>

#include <nw/effect.h>

namespace nw { namespace eftvw { // TODO: Implement this properly

class ViewerSystem
{
public:
    ViewerSystem(nw::eft::Heap* heap, nw::eft::Heap* connectionHeap, nw::eft::System* system);
    
    void UnbindResource(u32 resourceId);
};

} }

namespace sead { namespace ptcl {

enum
{
    cEftMaxGroup = nw::eft::EFT_GROUP_MAX
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

private:
    u32 _0[0xE58 / sizeof(u32)];
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

        mSeadHeapArray = new (config.mpHeap) ::sead::Heap*[config.GetResourceNum()];
        std::memset(mSeadHeapArray, 0, sizeof(::sead::Heap*) * config.GetResourceNum());
    }

    void entryResource(::sead::Heap* heap, void* resource, s32 resId)
    {
        mSeadHeapArray[resId] = heap;
        Heap heap_(mSeadHeapArray[resId]);
        EntryResource(&heap_, resource, resId);
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
