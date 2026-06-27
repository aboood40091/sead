#ifndef SEAD_EXP_HEAP_H_
#define SEAD_EXP_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

// TODO

class ExpHeap : public Heap
{
public:
    ExpHeap(const SafeString& name, Heap* parent, void* start, u32 size, HeapDirection direction, bool enable_lock);
    virtual ~ExpHeap();

    virtual const void* getStartAddress() const;
    virtual const void* getEndAddress() const;
    virtual size_t getSize() const;

public:
    static ExpHeap* tryCreate(u32 size, const SafeString& name, Heap* parent = nullptr, HeapDirection direction = cHeapDirection_Forward, bool enable_lock = false);

    static ExpHeap* create(u32 size, const SafeString& name, Heap* parent = nullptr, HeapDirection direction = cHeapDirection_Forward, bool enable_lock = false)
    {
        ExpHeap* heap = tryCreate(size, name, parent, direction, enable_lock);
        if (!heap)
        {
            // SEAD_ASSERT_MSG(false, "heap create failed. [%s] size: %d, parent: %s(%p)", name.cstr(), size, parent ? parent->getName().cstr() : "--", parent);
        }
        return heap;
    }
};

} // namespace sead

#endif // SEAD_EXP_HEAP_H_
