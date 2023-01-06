#pragma once

#include <heap/seadDisposer.h>
#include <hostio/seadHostIODummy.h>
#include <prim/seadSafeString.h>
#include <thread/seadCriticalSection.h>

namespace agl { namespace detail {

class RootNode
{
    SEAD_SINGLETON_DISPOSER(RootNode)

public:
    RootNode();
    virtual ~RootNode();

    static void setNodeMeta(sead::hostio::Node* p_node, const sead::SafeString& meta);

private:
    sead::FixedSafeString<256> _10;
    sead::CriticalSection mCriticalSection;
};
static_assert(sizeof(RootNode) == 0x15C, "agl::detail::RootNode size mismatch");

} }
