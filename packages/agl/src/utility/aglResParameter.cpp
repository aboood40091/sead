#include <utility/aglParameter.h>
#include <utility/aglResParameter.h>

namespace agl { namespace utl {

void ResParameterObj::modifyEndianObj(bool is_le)
{
    ModifyEndianU32(is_le, ptr(), sizeof(ResParameterObjData));

    for (iterator itr = begin(), itr_end = end(); itr != itr_end; ++itr)
    {
        ResParameter parameter(&(*itr));
        ModifyEndianU32(is_le, parameter.ptr(), sizeof(ResParameterData));

        u32 type = parameter.ref().mType;

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
            ModifyEndianU32(is_le, const_cast<void*>(parameter.getValue()), parameter.ref().mSize - sizeof(ResParameterData));
            break;
        default:
            // SEAD_ASSERT_MSG(false, "illigal type:%d", type);
        }
    }
}

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
