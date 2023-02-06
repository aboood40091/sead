#ifndef SEAD_PTCL_SYSTEM_H_
#define SEAD_PTCL_SYSTEM_H_

#include <heap/seadHeap.h>
#include <time/seadTickTime.h>

#include <nw/effect.h>

namespace nw { namespace eftvw { // TODO: Implement this properly

class ViewerSystem
{
public:
    void UnbindResource(u32 resourceId);
};

} }

namespace sead { namespace ptcl {

enum
{
    cEftMaxGroup = nw::eft::EFT_GROUP_MAX
};

class Heap : public nw::eft::Heap
{
public:
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

private:
    ::sead::Heap* mpHeap;
};
static_assert(sizeof(Heap) == 8, "sead::ptcl::Heap size mismatch");

class PtclEditorInterface
{
    u32 _0[0xE58 / sizeof(u32)];
};
static_assert(sizeof(PtclEditorInterface) == 0xE58, "sead::ptcl::PtclEditorInterface size mismatch");

class PtclSystem : public nw::eft::System
{
public:
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
