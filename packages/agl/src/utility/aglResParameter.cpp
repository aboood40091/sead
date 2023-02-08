#include <utility/aglResParameter.h>

namespace agl { namespace utl {

void ResParameterList::modifyEndianList(bool is_le)
{
    ModifyEndianU32(is_le, ptr(), sizeof(ResParameterListData));

    listIterator itr_list = listBegin();
    for (; itr_list != listEnd(); ++itr_list)
        ResParameterList(&(*itr_list)).modifyEndianList(is_le);

    objIterator itr_obj = objIterator(0, (ResParameterObjData*)(&(*itr_list))); // Optimization, instead of using objBegin()
    for (; itr_obj != objEnd(); ++itr_obj)
        ResParameterObj(&(*itr_obj)).modifyEndianObj(is_le);
}

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
