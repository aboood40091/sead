#ifndef SEAD_SEPARATE_HEAP_H_
#define SEAD_SEPARATE_HEAP_H_

#include <heap/seadHeap.h>

namespace sead {

// TODO

class SeparateHeap : public Heap
{
public:
    static SeparateHeap* create(const SafeString& name, u32 management_area_size, u32 allocate_area_size, Heap* parent = nullptr, bool enable_lock = false);
};

}

#endif // SEAD_SEPARATE_HEAP_H_
