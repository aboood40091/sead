#pragma once

#include <prim/seadSafeString.h>

namespace agl { namespace utl {

class INamedObj
{
public:
    INamedObj();
    virtual ~INamedObj();

    virtual const sead::SafeString& getObjName() const;
    virtual const sead::SafeString& getGroupName() const;
    virtual s32 getObjType() const;
    virtual bool isHostIOEnabled() const;
};
static_assert(sizeof(INamedObj) == 4, "agl::utl::INamedObj size mismatch");

} }
