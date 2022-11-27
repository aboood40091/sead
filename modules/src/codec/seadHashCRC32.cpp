#include <codec/seadHashCRC32.h>

namespace sead {

u32 HashCRC32::sTable[256];
bool HashCRC32::sInitialized = false;

void HashCRC32::initialize()
{
    u32 x = 0;
    for (u32 i = 0; i < 256; i++)
    {
        x = i;
        for (u32 j = 0; j < 8; j++)
        {
            if (x & 1)
                x = x >> 1 ^ 0xedb88320;
            else
                x >>= 1;
        }
        sTable[i] = x;
    }
    sInitialized = true;
}

u32 HashCRC32::calcHash(const void* dataptr, u32 datasize)
{
    if (!sInitialized)
        initialize();

    u32 hash = 0xFFFFFFFF;
    const u8* dataptr8 = (const u8*)dataptr;

    for (u32 i = 0; i < datasize; i++)
    {
        u32 x = sTable[(hash ^ *dataptr8++) & 0xff];
        hash = hash >> 8 ^ x;
    }

    return ~hash;
}

} // namespace sead
