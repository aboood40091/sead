#ifndef SEAD_HASH_CRC32_H_
#define SEAD_HASH_CRC32_H_

#include <basis/seadTypes.h>

namespace sead {

class HashCRC32
{
public:
    static void initialize();
    static u32 calcHash(const void* dataptr, u32 datasize);

private:
    static u32 sTable[256];
    static bool sInitialized;
};

} // namespace sead

#endif // SEAD_HASH_CRC32_H_
