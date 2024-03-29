#include <common/aglTextureData.h>
#include <common/aglTextureFormatInfo.h>
#include <detail/aglGX2.h>
#include <detail/aglTextureDataUtil.h>
#include <prim/seadMemUtil.h>

namespace agl {

TextureData::TextureData()
    : mMinWidth(1)
    , mMinHeight(1)
    , mMinSlice(1)
    , mMaxMipLevel(1)
    , mCompR(cTextureCompSel_0)
    , mCompG(cTextureCompSel_0)
    , mCompB(cTextureCompSel_0)
    , mCompA(cTextureCompSel_0)
{
    sead::MemUtil::fillZero(&mSurface, sizeof(GX2Surface));
}

void TextureData::setMipLevelNum(u32 mip_level_num)
{
    mSurface.numMips = sead::Mathi::clamp2(1, mip_level_num, mMaxMipLevel);
    detail::TextureDataUtil::calcSizeAndAlignment(&mSurface);
}

void TextureData::invalidateGPUCache() const
{
    if (getImagePtr())
        GX2Invalidate(GX2_INVALIDATE_TEXTURE, getImagePtr(), getImageByteSize());

    if (getMipPtr())
        GX2Invalidate(GX2_INVALIDATE_TEXTURE, getMipPtr(), getMipByteSize());
}

void TextureData::copyTo_(const TextureData* dst, s32 dst_slice, s32 dst_mip_level, s32 src_slice, s32 src_mip_level, bool restore_state) const
{
  //SEAD_ASSERT( dst != nullptr );
  //SEAD_ASSERT( 0 < dst_mip_level || dst->getImagePtr() != nullptr );

  //SEAD_ASSERT( dst_slice < static_cast< int >( dst->getSliceNum( dst_mip_level ) ) );
  //SEAD_ASSERT( src_slice < static_cast< int >( getSliceNum( src_mip_level ) ) );

  //SEAD_ASSERT( dst_mip_level < static_cast< int >( dst->getMipLevelNum() ) );
  //SEAD_ASSERT( src_mip_level < static_cast< int >( getMipLevelNum() ) );

  //SEAD_ASSERT( getWidth( src_mip_level ) == dst->getWidth( dst_mip_level ) );
  //SEAD_ASSERT( getHeight( src_mip_level ) == dst->getHeight( dst_mip_level ) );

    GX2CopySurface(
        &mSurface,
        src_mip_level,
        src_slice,
        &dst->mSurface,
        dst_mip_level,
        dst_slice
    );

    if (restore_state)
        driver::GX2Resource::instance()->restoreContextState();
}

void TextureData::copyToAll(const TextureData* dst) const
{
    u32 slice_num = sead::Mathu::min(getSliceNum(), dst->getSliceNum());
    u32 mip_level_num = sead::Mathu::min(getMipLevelNum(), dst->getMipLevelNum());

    for (u32 slice = 0; slice < slice_num; slice++)
        for (u32 mip_level = 0; mip_level < mip_level_num; mip_level++)
            copyTo(dst, slice, mip_level);

    driver::GX2Resource::instance()->restoreContextState();
}

void TextureData::initialize_(TextureType type, TextureFormat format, u32 width, u32 height, u32 slice_num, u32 mip_level_num, MultiSampleType multi_sample_type)
{
    mSurface.dim = GX2SurfaceDim(type);
    mFormat = format;
    mSurface.format = detail::TextureDataUtil::convFormatAGLToGX2(format);
    mSurface.aa = GX2AAMode(multi_sample_type);

    if (format >= cTextureFormat_Depth_16 && format <= cTextureFormat_Depth_24_uNorm_Stencil_8)
        mSurface.use = GX2_SURFACE_USE_DEPTH_BUFFER_TEXTURE;
    else
        mSurface.use = GX2_SURFACE_USE_TEXTURE;

    mSurface.tileMode = GX2_TILE_MODE_DEFAULT;
    mSurface.swizzle = 0;

    initializeSize_(width, height, slice_num);
    setMipLevelNum(mip_level_num);

    mCompR = TextureFormatInfo::getDefaultCompSel(format, 0);
    mCompG = TextureFormatInfo::getDefaultCompSel(format, 1);
    mCompB = TextureFormatInfo::getDefaultCompSel(format, 2);
    mCompA = TextureFormatInfo::getDefaultCompSel(format, 3);

    detail::TextureDataUtil::calcSizeAndAlignment(&mSurface);

    mSurface.imagePtr = nullptr;
    mSurface.mipPtr = nullptr;
}

void TextureData::initializeSize_(u32 width, u32 height, u32 slice_num)
{
    mSurface.width = width;
    mSurface.height = height;
    mSurface.depth = slice_num;

    mMinWidth = 1;

    if (mSurface.dim == GX2_SURFACE_DIM_1D_ARRAY)
        mMinHeight = height;
    else
        mMinHeight = 1;

    if (mSurface.dim == GX2_SURFACE_DIM_CUBE || mSurface.dim == GX2_SURFACE_DIM_2D_ARRAY)
        mMinSlice = slice_num;
    else
        mMinSlice = 1;

    if (mSurface.dim == GX2_SURFACE_DIM_2D_MSAA)
    {
        mMaxMipLevel = 1;
        return;
    }

    for (s32 mip_level = 1; ; mip_level++)
    {
        s32 mip_level_prev = mip_level - 1;
        if (getWidth(mip_level) == getWidth(mip_level_prev) &&
            getHeight(mip_level) == getHeight(mip_level_prev) &&
            getSliceNum(mip_level) == getSliceNum(mip_level_prev))
        {
            mMaxMipLevel = mip_level;
            return;
        }
    }
}

void TextureData::initializeFromSurface(const GX2Surface& surface)
{
    mSurface = surface;
    mFormat = detail::TextureDataUtil::convFormatGX2ToAGL(mSurface.format);

    initializeSize_(mSurface.width, mSurface.height, mSurface.depth);
    setMipLevelNum(surface.numMips);

    mCompR = TextureFormatInfo::getDefaultCompSel(mFormat, 0);
    mCompG = TextureFormatInfo::getDefaultCompSel(mFormat, 1);
    mCompB = TextureFormatInfo::getDefaultCompSel(mFormat, 2);
    mCompA = TextureFormatInfo::getDefaultCompSel(mFormat, 3);
}

}
