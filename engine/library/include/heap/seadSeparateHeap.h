#ifndef SEAD_SEPARATE_HEAP_H_
#define SEAD_SEPARATE_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

// TODO

class SeparateHeap : public Heap
{
public:
    // TODO: These should be inline
    virtual const void* getStartAddress() const;
    virtual const void* getEndAddress() const;
    virtual size_t getSize() const;

public:
    static SeparateHeap* create(const SafeString& name, u32 management_area_size, u32 allocate_area_size, Heap* parent = nullptr, bool enable_lock = false);
};

}

#endif // SEAD_SEPARATE_HEAP_H_
