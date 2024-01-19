#ifndef SEAD_HOST_IO_DEFINE_H_
#define SEAD_HOST_IO_DEFINE_H_

#include <prim/seadSafeString.h>

namespace sead { namespace hostio {

struct FileInfo
{
    SafeString path;
    u64 size;
};

} } // namespace sead::hostio

#endif // SEAD_HOST_IO_DEFINE_H_
