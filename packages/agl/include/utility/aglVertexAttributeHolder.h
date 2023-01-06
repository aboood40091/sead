#pragma once

#include <common/aglVertexAttribute.h>
#include <container/seadSafeArray.h>
#include <heap/seadDisposer.h>

namespace agl { namespace utl {

class VertexAttributeHolder
{
    SEAD_SINGLETON_DISPOSER(VertexAttributeHolder)

public:
    enum VertexAttributeType
    {
        cAttribute_QuadTriangle = 7,
        cAttribute_Num = 9
    };

public:
    VertexAttributeHolder();
    virtual ~VertexAttributeHolder();

    const VertexAttribute& getVertexAttribute(VertexAttributeType type) const
    {
        return mVertexAttribute[type];
    }

private:
    sead::SafeArray<VertexAttribute, cAttribute_Num> mVertexAttribute;
};
static_assert(sizeof(VertexAttributeHolder) == 0x8A8, "agl::utl::VertexAttributeHolder size mismatch");

} }
