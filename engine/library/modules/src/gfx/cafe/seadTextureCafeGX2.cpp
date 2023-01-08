#include <gfx/cafe/seadTextureCafeGX2.h>

namespace sead {

u32 TextureCafeGX2::getWidth() const
{
    //SEAD_ASSERT(mGX2Texture);
    return mGX2Texture->surface.width;
}

u32 TextureCafeGX2::getHeight() const
{
    //SEAD_ASSERT(mGX2Texture);
    return mGX2Texture->surface.height;
}

} // namespace sead
