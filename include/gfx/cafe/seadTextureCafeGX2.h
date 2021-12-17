#ifndef SEAD_TEXTURE_CAFE_GX2_H_
#define SEAD_TEXTURE_CAFE_GX2_H_

#include <cafe.h>

#include <gfx/seadTexture.h>
#include <prim/seadRuntimeTypeInfo.h>

namespace sead {

class TextureCafeGX2 : public Texture
{
    SEAD_RTTI_OVERRIDE(TextureCafeGX2, Texture)

public:
    TextureCafeGX2()
        : Texture()
        , mGX2Texture(NULL)
    {
    }

    virtual ~TextureCafeGX2()
    {
    }

    virtual u32 getWidth() const;
    virtual u32 getHeight() const;

    const GX2Texture* getGX2Texture() const
    {
        return mGX2Texture;
    }

    GX2Texture* getGX2Texture()
    {
        return mGX2Texture;
    }

    void setGX2Texture(GX2Texture* texture)
    {
        mGX2Texture = texture;
    }

private:
    GX2Texture* mGX2Texture;
};
static_assert(sizeof(TextureCafeGX2) == 8, "sead::TextureCafeGX2 size mismatch");

} // namespace sead

#endif // SEAD_TEXTURE_CAFE_GX2_H_
