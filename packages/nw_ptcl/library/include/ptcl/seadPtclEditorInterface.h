#ifndef SEAD_PTCL_EDITOR_INTERFACE_H_
#define SEAD_PTCL_EDITOR_INTERFACE_H_

#include <basis/seadTypes.h>

namespace sead { namespace ptcl {

class PtclEditorInterface
{
    u32 _0[0xE58 / sizeof(u32)];
};
static_assert(sizeof(PtclEditorInterface) == 0xE58, "sead::ptcl::PtclEditorInterface size mismatch");

} }

#endif // SEAD_PTCL_EDITOR_INTERFACE_H_
