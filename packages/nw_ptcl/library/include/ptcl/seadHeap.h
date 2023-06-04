#ifndef SEAD_PTCL_HEAP_H_
#define SEAD_PTCL_HEAP_H_

#include <heap/seadHeap.h>

#include <nw/eft/eft_Heap.h>

namespace sead { namespace ptcl {

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

} }

#endif // SEAD_PTCL_HEAP_H_
