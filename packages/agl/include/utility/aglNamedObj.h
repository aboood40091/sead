#pragma once

#include <hostio/seadHostIODummy.h>
#include <prim/seadSafeString.h>

namespace agl { namespace utl {

class INamedObj : public sead::hostio::Node
{
public:
    INamedObj();
    virtual ~INamedObj();

    static const sead::SafeString& getDefaultGroupName();

    virtual const sead::SafeString& getObjName() const { return sead::SafeString::cEmptyString; }
    virtual const sead::SafeString& getGroupName() const { return sead::SafeString::cEmptyString; }
    virtual s32 getObjType() const { return 0; }
    virtual bool isHostIOEnabled() const { return true; }
};
static_assert(sizeof(INamedObj) == 4, "agl::utl::INamedObj size mismatch");

} }
