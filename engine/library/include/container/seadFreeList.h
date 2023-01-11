#ifndef SEAD_FREE_LIST_H_
#define SEAD_FREE_LIST_H_

#include <basis/seadTypes.h>

namespace sead {

// TODO

class FreeList
{
public:
    FreeList();

    void init(void* work, s32 elem_size, s32 num)
    {
        // SEAD_ASSERT(work);
        // SEAD_ASSERT(elem_size > 0 && elem_size % cPtrSize == 0);
        // SEAD_ASSERT(num > 0);

        mFree = work;

        const s32 idx_multiplier = elem_size / cPtrSize;

        for (s32 i = 0; i < num - 1; ++i)
            static_cast<void**>(work)[i * idx_multiplier] = &static_cast<void**>(work)[(i + 1) * idx_multiplier];

        static_cast<void**>(work)[(num - 1) * idx_multiplier] = nullptr;

        mWork = work;
    }

    void* get()
    {
        if (mFree == nullptr)
            return nullptr;

        void* ptr = mFree;
        mFree = *static_cast<void**>(mFree);
        return ptr;
    }

    void cleanup()
    {
        mFree = nullptr;
        mWork = nullptr;
    }

    void* work() { return mWork; }
    const void* work() const { return mWork; }

    void* free() { return mFree; }
    const void* free() const { return mFree; }

    void free(void* ptr)
    {
        *static_cast<void**>(ptr) = mFree;
        mFree = ptr;
    }

private:
    void* mFree;
    void* mWork;

    static const size_t cPtrSize = sizeof(void*);
};

} // namespace sead

#endif // SEAD_FREE_LIST_H_
