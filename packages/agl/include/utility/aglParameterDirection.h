#pragma once

#include <utility/aglParameter.h>

namespace agl { namespace utl {

class ParameterDirection3f : public Parameter<sead::Vector3f>
{
public:
    ParameterDirection3f(const sead::Vector3f& value, IParameterObj* p_obj)
        : Parameter<sead::Vector3f>(value, "Direction", "•ûŒü", p_obj)
    {
    }
};
static_assert(sizeof(ParameterDirection3f) == 0x1C);

} }
