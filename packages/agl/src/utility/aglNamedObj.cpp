#include <utility/aglNamedObj.h>

namespace agl { namespace utl {

INamedObj::INamedObj()
{
}

INamedObj::~INamedObj()
{
}

const sead::SafeString& INamedObj::getDefaultGroupName()
{
    static sead::SafeString s_default_group_name = "default";
    return s_default_group_name;
}

} }
