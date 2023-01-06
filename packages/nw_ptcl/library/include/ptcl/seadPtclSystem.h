#ifndef SEAD_PTCL_SYSTEM_H_
#define SEAD_PTCL_SYSTEM_H_

#include <heap/seadHeap.h>
#include <time/seadTickTime.h>

#include <nw/effect.h>

namespace sead { namespace ptcl {

class Heap : public nw::eft::Heap
{
public:
    virtual void* Alloc(u32 size, s32 alignment)
    {
        return mpHeap->alloc(size, alignment);
    }

    virtual void Free(void* ptr)
    {
        mpHeap->free(ptr);
    }

private:
    sead::Heap* mpHeap;
};
static_assert(sizeof(Heap) == 8, "sead::ptcl::Heap size mismatch");

class PtclEditorInterface
{
    u32 _0[0xE58 / sizeof(u32)];
};
static_assert(sizeof(PtclEditorInterface) == 0xE58, "sead::ptcl::PtclEditorInterface size mismatch");

class PtclSystem : public nw::eft::System
{
    TickTime mTime;
    Heap mHeap;
    Heap mViewerSysHeap;
    Heap _a40;
    u8 _a48[4];
    sead::Heap** mpResHeap;
    void* mpViewerSys; // nw::eftvw::ViewerSystem*
    PtclEditorInterface mEditorInterface;
    u8 _18ac[4];
};
static_assert(sizeof(PtclSystem) == 0x18B0, "sead::ptcl::PtclSystem size mismatch");

} }

#endif // SEAD_PTCL_SYSTEM_H_
