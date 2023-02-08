#include <common/aglResCommon.h>
#include <prim/seadEndian.h>

namespace agl {

void ModifyEndianU32(bool is_le, void* p_data, size_t size)
{
    // SEAD_ASSERT(p_data != nullptr);
    // SEAD_ASSERT(size % 4 == 0);

    u32* p_data_u32 = static_cast<u32*>(p_data);
    u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
    {
        *p_data_u32 = sead::Endian::toHost(sead::Endian::Types(is_le), *p_data_u32);
        p_data_u32++;
    }
}

}
