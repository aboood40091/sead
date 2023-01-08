#ifndef SEAD_TEXTURE_H_
#define SEAD_TEXTURE_H_

#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class Texture
{
    SEAD_RTTI_BASE(Texture)

public:
    Texture() { }
    virtual ~Texture() { }

    virtual u32 getWidth() const = 0;
    virtual u32 getHeight() const = 0;
};
#ifdef cafe
static_assert(sizeof(Texture) == 4, "sead::Texture size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_TEXTURE_H_
