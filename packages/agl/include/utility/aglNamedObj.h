#pragma once

#include <prim/seadSafeString.h>

namespace agl { namespace utl {

class INamedObj
{
public:
    INamedObj();
    virtual ~INamedObj();

    virtual const sead::SafeString& getObjName() const { return sead::SafeString::cEmptyString; }
    virtual const sead::SafeString& getGroupName() const { return sead::SafeString::cEmptyString; }
    virtual s32 getObjType() const { return 0; }
    virtual bool isHostIOEnabled() const { return true; }
};
static_assert(sizeof(INamedObj) == 4, "agl::utl::INamedObj size mismatch");

} }
