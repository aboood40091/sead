#ifndef SEAD_FREE_LIST_H_
#define SEAD_FREE_LIST_H_

#include <basis/seadTypes.h>

namespace sead {

// TODO

class FreeList
{
public:
    FreeList();

    void init(void* work, s32 elem_size, s32 num);
    void* get();
    void cleanup();

private:
    void* mFree;
    void* mWork;
};

} // namespace sead

#endif // SEAD_FREE_LIST_H_
