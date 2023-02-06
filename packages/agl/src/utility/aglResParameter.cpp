#include <utility/aglResParameter.h>

namespace agl { namespace utl {

ResParameterArchive::ResParameterArchive(const void* p_data)
    : ResCommon<ResParameterArchiveData>(p_data)
{
    if (!isValid())
        return;

    // SEAD_ASSERT(sead::PtrUtil::isAlignedN( p_data, 4 ));

    if (!isEndianResolved())
    {
        ModifyEndianU32(modifyEndian(), ptr(), sizeof(ResParameterArchiveData));

        verify();

        getResParameterList().modifyEndianList(modifyEndian());

        setEndianResolved();
    }
}

} }
