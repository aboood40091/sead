#include <utility/aglParameter.h>
#include <utility/aglResParameter.h>

namespace agl { namespace utl {

ResParameter ResParameterObj::getResParameter(u32 index) const
{
    // SEAD_ASSERT(index < getNum());

    constIterator itr = begin();
    constIterator itr_end = constIterator(index, nullptr);

    while (itr != itr_end)
        ++itr;

    return &(*itr);
}

void ResParameterObj::modifyEndianObj(bool is_le)
{
    ModifyEndianU32(is_le, ptr(), sizeof(ResParameterObjData));

    for (iterator itr = begin(), itr_end = end(); itr != itr_end; ++itr)
    {
        ResParameter parameter(&(*itr));
        ModifyEndianU32(is_le, parameter.ptr(), sizeof(ResParameterData));

        u32 type = parameter.ptr()->mType;

        switch (type)
        {
        case ParameterBase::cType_bool:
        case ParameterBase::cType_string32:
        case ParameterBase::cType_string64:
            break;
        case ParameterBase::cType_f32:
        case ParameterBase::cType_int:
        case ParameterBase::cType_vec2:
        case ParameterBase::cType_vec3:
        case ParameterBase::cType_vec4:
        case ParameterBase::cType_color:
        case ParameterBase::cType_curve1:
        case ParameterBase::cType_curve2:
        case ParameterBase::cType_curve3:
        case ParameterBase::cType_curve4:
            ModifyEndianU32(is_le, const_cast<void*>(parameter.getValue()), parameter.ptr()->mSize - sizeof(ResParameterData));
            break;
        default:
            // SEAD_ASSERT_MSG(false, "illigal type:%d", type);
        }
    }
}

ResParameterList ResParameterList::getResParameterList(u32 index) const
{
    // SEAD_ASSERT(index < getResParameterListNum());

    listConstIterator itr = listConstBegin();
    while (static_cast<u32>(itr.getIndex()) < index)
        ++itr;

    return &(*itr);
}

ResParameterObj ResParameterList::getResParameterObj(u32 index) const
{
    // SEAD_ASSERT(index < getResParameterObjNum());

    objConstIterator itr = objConstBegin();
    while (static_cast<u32>(itr.getIndex()) < index)
        ++itr;

    return &(*itr);
}

void ResParameterList::modifyEndianList(bool is_le)
{
    ModifyEndianU32(is_le, ptr(), sizeof(ResParameterListData));

    listIterator itr_list = listBegin();
    for (; static_cast<u32>(itr_list.getIndex()) < getResParameterListNum(); ++itr_list)
        ResParameterList(&(*itr_list)).modifyEndianList(is_le);

    objIterator itr_obj = objIterator(0, (ResParameterObjData*)(&(*itr_list))); // Optimization, instead of using objBegin()
    for (; static_cast<u32>(itr_obj.getIndex()) < getResParameterObjNum(); ++itr_obj)
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
