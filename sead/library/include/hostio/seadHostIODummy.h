#ifndef SEAD_HOST_IO_DUMMY_H_
#define SEAD_HOST_IO_DUMMY_H_

#include <basis/seadTypes.h>

namespace sead { namespace hostio {

struct LifeCheckable
{
};

struct PropertyEventListener : public LifeCheckable
{
};

struct NodeEventListener : public PropertyEventListener
{
};

struct Reflexible : public NodeEventListener
{
};

struct Node : public Reflexible
{
};

struct Context
{
    struct ContextBufferAccessor
    {
    };
};

} } // namespace sead::hostio

#endif // SEAD_HOST_IO_DUMMY_H_
