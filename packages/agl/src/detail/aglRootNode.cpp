#pragma once

#include <detail/aglPrivateResource.h>
#include <detail/aglRootNode.h>

namespace agl { namespace detail {

SEAD_SINGLETON_DISPOSER_IMPL(RootNode)

RootNode::RootNode()
    : _10(sead::SafeString::cEmptyString)
    , mCriticalSection()
{
}

RootNode::~RootNode()
{
}

void RootNode::setNodeMeta(sead::hostio::Node* p_node, const sead::SafeString& meta)
{
    // SEAD_ASSERT(p_node != nullptr);

    if (!instance() || !PrivateResource::instance()->getDebugHeap())
        return;

    sead::CriticalSection& cs = PrivateResource::instance()->getCriticalSection();
    cs.lock();
    {
        sead::FormatFixedSafeString<1024> s("%s%s", meta.cstr(), instance()->_10.cstr());
    }
    cs.unlock();
}

} }
