#include <common/aglDisplayList.h>
#include <detail/aglPrivateResource.h>

namespace agl {

DisplayList::DisplayList()
{
    clear();
}

DisplayList::~DisplayList()
{
}

void DisplayList::setBuffer(u8* buffer, size_t size)
{
    if (buffer)
        mSize = size;
    else
        mSize = 0;

    mpBuffer = buffer;
}

void DisplayList::clear()
{
    mpBuffer = nullptr;
    mSize = 0;
    mValidSize = 0;
}

bool DisplayList::beginDisplayList()
{
    mValidSize = 0;

#ifdef cafe
    GX2BeginDisplayListEx(mpBuffer, mSize, GX2_DISABLE);
    return true;
#else
    return false;
#endif
}

size_t DisplayList::endDisplayList()
{
#ifdef cafe
    void* dl;
    size_t size;
    GX2GetCurrentDisplayList(&dl, &size);
    size_t dl_size = GX2EndDisplayList(dl);
    mValidSize = dl_size + ((uintptr_t)dl - (uintptr_t)mpBuffer);
    return mValidSize;
#else
    return 0;
#endif
}

bool DisplayList::beginDisplayListBuffer(u8* buffer, size_t size)
{
    // SEAD_ASSERT(0 < size);
#ifdef cafe
    DCInvalidateRange(buffer, size);
#endif // cafe
    setBuffer(buffer, size);
    return beginDisplayList();
}

size_t DisplayList::endDisplayListBuffer(sead::Heap* heap)
{
    u8* buffer = nullptr;
    size_t size = endDisplayList();
    if (size != 0)
    {
        buffer = new (heap, cDisplayListAlignment) u8[size];
        sead::MemUtil::copy(buffer, mpBuffer, size);
#ifdef cafe
        DCFlushRange(buffer, size);
#endif // cafe
    }
    setBuffer(buffer, size);
    return size;
}

bool DisplayList::beginDisplayListTemporary(size_t size)
{
    u8* buffer = new (detail::PrivateResource::instance()->getWorkHeap(), cDisplayListAlignment) u8[size];
    return beginDisplayListBuffer(buffer, size);
}

size_t DisplayList::endDisplayListTemporary(sead::Heap* heap)
{
    u8* buffer = mpBuffer;
    size_t size = endDisplayListBuffer(heap);
    delete[] buffer;
    return size;
}

size_t DisplayList::suspend(void** p_dl)
{
    // SEAD_ASSERT(p_dl != nullptr);

    *p_dl = nullptr;
    size_t size = 0;

#ifdef cafe
    if (GX2GetCurrentDisplayList(p_dl, &size) == GX2_TRUE)
    {
        size_t dl_size = GX2EndDisplayList(*p_dl);

        size -= dl_size;
        *p_dl = (void*)((uintptr_t)*p_dl + dl_size);
    }
#endif //cafe

    return size;
}
void DisplayList::resume(void* dl, size_t size)
{
#ifdef cafe
    if (dl != nullptr)
        GX2BeginDisplayListEx(dl, size, GX2_DISABLE);
#endif // cafe
}

}
