#include <common/aglResCommon.h>
#include <prim/seadEndian.h>

namespace agl {

void ModifyEndianU32(bool is_le, void* ptr, size_t size)
{
    u32* ptr_u32 = static_cast<u32*>(ptr);
    u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
    {
        *ptr_u32 = sead::Endian::toHost(sead::Endian::Types(is_le), *ptr_u32);
        ptr_u32++;
    }
}

}
