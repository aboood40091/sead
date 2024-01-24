#include <common/aglTextureDataInitializer.h>
#include <detail/aglRootNode.h>
#include <heap/seadSeparateHeap.h>
#include <lighting/aglLightMapMgr.h>
#include <prim/seadEndian.h>

namespace agl { namespace lght {

LightMapMgr::LightMapMgr()
    : utl::IParameterIO("agllmap", 2)
    , _2b84(0.25f)
    , _2b88(0)
    , mpLightMapHeap(nullptr)
{
    detail::RootNode::setNodeMeta(this, "Icon=LIGHT");
}

LightMapMgr::~LightMapMgr()
{
    mLightMap.freeBuffer();
    mpLightMapHeap->destroy();
}

void LightMapMgr::initialize(const env::EnvObjBuffer::AllocateArg& alloc_arg, const CreateArg& create_arg, env::EnvObjMgr* p_env_mgr, sead::Heap* heap)
{
    s32 light_map_max = create_arg.getNum();
  //SEAD_ASSERT(0 < light_map_max && light_map_max <= LightMap::cLightMapMax);

    sead::FixedSafeString<32> type("lightmap_array");
    mType.initialize(type, "type", "タイプ", &mConfig);

    addObj(&mConfig, "config");
    addObj(&mLUTParam, "lut_param");

    // mCPUTextureLUT
    {
        agl::TextureData texture_data;
        texture_data.initialize(cTextureFormat_R8_uNorm, cLUTValueNum, cLUT_Num, 1);
        mCPUTextureLUT.mPtr = new (heap, texture_data.getAlignment()) u8[texture_data.getImageByteSize()];
        mCPUTextureLUT.mSize = texture_data.getImageByteSize();
    }
    s32 user_data_index = 0;
    for (
        sead::SafeArray<LUT, cLUT_Num>::iterator
            itr_lut = mLUT.begin(),
            itr_lut_end = mLUT.end()
        ;   itr_lut != itr_lut_end
        ; ++itr_lut
    )
    {
        LUT& lut = *itr_lut;

        sead::FixedSafeString<32> name(sead::SafeString::cEmptyString);

        s32 index = itr_lut.getIndex();

        lut.mName.initialize(
            name, sead::FormatFixedSafeString<1024>(
                "name%d", index
            ), "名前", "AcceptReturn = False", &mLUTParam
        );

        lut.mIntensity.initializeListNode(
            sead::FormatFixedSafeString<1024>(
                "intensity%d", index
            ), "インテンシティ", sead::SafeString::cEmptyString, &mLUTParam
        );

        switch (index)
        {
        default:
            lut.mName->format("UserData%d", user_data_index); user_data_index++;
            break;
        case cLUT_Lambert:
            *lut.mName = "Lambert";
            break;
        case cLUT_Half_Lambert:
            *lut.mName = "Half-Lambert";
            break;
        case cLUT_Hemisphere:
            *lut.mName = "Hemisphere";
            break;
        }
    }
    updateLUT_();

    // mCPUTextureSphereNormal
    {
        agl::TextureData texture_data;
        texture_data.initialize(cTextureFormat_R32_G32_B32_A32_float, create_arg.getSizeSphere(), create_arg.getSizeSphere(), 1);
        mCPUTextureSphereNormal.mPtr = new (heap, texture_data.getAlignment()) u8[texture_data.getImageByteSize()];
        mCPUTextureSphereNormal.mSize = texture_data.getImageByteSize();
    }
    // mCPUTextureCubeNormal
    {
        agl::TextureData texture_data;
        texture_data.initialize(cTextureFormat_R32_G32_B32_A32_float, create_arg.getSizeCube(), create_arg.getSizeCube(), 1);
        mCPUTextureCubeNormal.mPtr = new (heap, texture_data.getAlignment()) u8[texture_data.getImageByteSize()];
        mCPUTextureCubeNormal.mSize = texture_data.getImageByteSize();
    }
    createNormalMap_(create_arg.getSizeSphere(), create_arg.getSizeCube());

    // mpLightMapHeap
    {
        u32 size_cube = sead::Mathu::max(mCPUTextureCubeNormal.mTextureData.getWidth(), 64);

        agl::TextureData texture_data_sphere;
        agl::TextureData texture_data_cube;

        u32 size_sphere = mCPUTextureSphereNormal.mTextureData.getWidth();

        texture_data_sphere.initializeArray(cTextureFormat_R8_G8_B8_A8_uNorm, size_sphere, size_sphere, 2, 3);
        texture_data_cube.initializeCubeMap(cTextureFormat_R8_G8_B8_A8_uNorm, size_cube, size_cube, 3);

        u32 image_size = sead::Mathu::max(texture_data_sphere.getImageByteSize(), texture_data_cube.getImageByteSize());
        u32 mip_size = sead::Mathu::max(texture_data_sphere.getMipByteSize(), texture_data_cube.getMipByteSize());
        u32 alignment = sead::Mathu::max(texture_data_sphere.getAlignment(), texture_data_cube.getAlignment());

        mpLightMapHeap = sead::SeparateHeap::create("LightMap", 256 * light_map_max, (image_size + mip_size + 2 * alignment) * light_map_max, heap);
    }

    mLightMap.allocBuffer(light_map_max, heap);
    mLightMapCalc.allocBuffer(light_map_max, heap);

    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap* p_lmap = &(*itr_lmap);
        p_lmap->initialize_(this, alloc_arg, create_arg.getViewMax(), heap);
        p_lmap->bind(p_env_mgr);
        p_lmap->mRimLightRef.bind(p_env_mgr);
        s32 index = itr_lmap.getIndex();
        *p_lmap->mName = sead::FormatFixedSafeString<1024>("LightMap%d", index);
        addList(p_lmap, sead::FormatFixedSafeString<1024>("%d", index));
    }

    for (
        GraphicsContext::iterator
            itr_context_array = mGraphicsContext.begin(),
            itr_context_array_end = mGraphicsContext.end()
        ;   itr_context_array != itr_context_array_end
        ; ++itr_context_array
    )
    {
        sead::UnsafeArray<sead::GraphicsContextMRT, 2>& context_array = *itr_context_array;
        for (
            sead::UnsafeArray<sead::GraphicsContextMRT, 2>::iterator
                itr_context = context_array.begin(),
                itr_context_end = context_array.end()
            ;   itr_context != itr_context_end
            ; ++itr_context
        )
        {
            sead::GraphicsContextMRT& context = *itr_context;
            context.setDepthEnable(false, false);
            if (itr_context.getIndex() == 0)
                context.setBlendEnableMask(0);
            else
            {
                context.setBlendEnableMask((1 << sead::Graphics::cRenderTarget_Num) - 1);
                for (u32 i = 0; i < sead::Graphics::cRenderTarget_Num; i++)
                {
                    context.getBlendExpression(i).setBlendFactor(sead::Graphics::cBlendFactor_One, sead::Graphics::cBlendFactor_One);
                    context.getBlendExpression(i).setBlendEquation(sead::Graphics::cBlendEquation_Add);
                }
            }
            context.setColorMask(
                1 << 0 |    // target 0 r
                1 << 1 |    // target 0 g
                1 << 2 |    // target 0 b
                1 << 3      // target 0 a
            );
            switch (itr_context_array.getIndex())
            {
            default:
                break;
            case 1:
                context.setColorMask(1, true, true, true, true);
                break;
            case 2:
                context.setColorMask(1, true, true, true, true);
                context.setColorMask(2, true, true, true, true);
                context.setColorMask(3, true, true, true, true);
                context.setColorMask(4, true, true, true, true);
                context.setColorMask(5, true, true, true, true);
                break;
            }
        }
    }

    mDebugTexturePage.setUp(create_arg.getViewMax(), "LightMap", heap);

    constructList_();
}

void LightMapMgr::updateLUT_()
{
    for (
        sead::SafeArray<LUT, cLUT_Num>::constIterator
            itr_lut = mLUT.constBegin(),
            itr_lut_end = mLUT.constEnd()
        ;   itr_lut != itr_lut_end
        ; ++itr_lut
    )
    {
        const LUT& lut = *itr_lut;

        s32 y = itr_lut.getIndex();
        for (s32 x = 0; x < cLUTValueNum; x++)
        {
            const f32 t = x / (f32)(cLUTValueNum - 1);
            f32 value;
            switch (y)
            {
            default:
                value = lut.mIntensity.evaluateUnit(0, t);
                break;
            case cLUT_Lambert:
                value = 2 * t - 1;
                break;
            case cLUT_Half_Lambert:
                value = sead::Mathf::square(t);
                break;
            case cLUT_Hemisphere:
                value = t;
                break;
            }
            static_cast<u8*>(mCPUTextureLUT.mPtr)[y * cLUTValueNum + x] = sead::Mathf::clamp2(0.0f, value, 1.0f) * 255;
        }
    }
    TextureDataInitializerRAW::initialize(
        &mCPUTextureLUT.mTextureData,
        mCPUTextureLUT.mPtr, mCPUTextureLUT.mSize,
        cTextureFormat_R8_uNorm,
        cLUTValueNum, cLUT_Num
    );
    mCPUTextureLUT.mTextureSampler.applyTextureData(mCPUTextureLUT.mTextureData);
}

void LightMapMgr::createNormalMap_(s32 size_sphere, s32 size_cube)
{
    // Sphere
    {
        sead::Vector4f* p_normal_sphere = static_cast<sead::Vector4f*>(mCPUTextureSphereNormal.mPtr);
        f32 v0 = _2b84 / size_sphere + 1.0f;
        f32 v1 = 2.0f / (size_sphere - 1.0f);
        for (s32 y = 0; y < size_sphere; y++)
        {
            for (s32 x = 0; x < size_sphere; x++)
            {
                p_normal_sphere->w = 0.0f;
                p_normal_sphere->x = (x * v1 - 1.0f) *  v0;
                p_normal_sphere->y = (y * v1 - 1.0f) * -v0;
                p_normal_sphere->z = sead::Mathf::sqrt(1.0f - (sead::Mathf::square(p_normal_sphere->x) + sead::Mathf::square(p_normal_sphere->y)));
                p_normal_sphere->normalize();

                // CPU is big endian and GPU is little endian,
                // therefore, endian swap is required.
                p_normal_sphere->x = sead::Endian::swapF32(p_normal_sphere->x);
                p_normal_sphere->y = sead::Endian::swapF32(p_normal_sphere->y);
                p_normal_sphere->z = sead::Endian::swapF32(p_normal_sphere->z);
                p_normal_sphere->w = sead::Endian::swapF32(p_normal_sphere->w);

                p_normal_sphere++;
            }
        }
        p_normal_sphere = static_cast<sead::Vector4f*>(mCPUTextureSphereNormal.mPtr);
        TextureDataInitializerRAW::initialize(
            &mCPUTextureSphereNormal.mTextureData,
            p_normal_sphere, mCPUTextureSphereNormal.mSize,
            cTextureFormat_R32_G32_B32_A32_float,
            size_sphere, size_sphere
        );
        mCPUTextureSphereNormal.mTextureSampler.applyTextureData(mCPUTextureSphereNormal.mTextureData);
        mCPUTextureSphereNormal.mTextureSampler.setFilter(
            cTextureFilterType_Point,
            cTextureFilterType_Point,
            cTextureMipFilterType_None
        );
    }
    // Cube
    {
        sead::Vector4f* p_normal_cube = static_cast<sead::Vector4f*>(mCPUTextureCubeNormal.mPtr);
        for (s32 y = 0; y < size_cube; y++)
        {
            for (s32 x = 0; x < size_cube; x++)
            {
                p_normal_cube->w = 0.0f;
                p_normal_cube->x = size_cube / 2;
                p_normal_cube->y = size_cube / 2 - y - 0.5f;
                p_normal_cube->z = size_cube / 2 - x - 0.5f;
                p_normal_cube->normalize();

                // CPU is big endian and GPU is little endian,
                // therefore, endian swap is required.
                p_normal_cube->x = sead::Endian::swapF32(p_normal_cube->x);
                p_normal_cube->y = sead::Endian::swapF32(p_normal_cube->y);
                p_normal_cube->z = sead::Endian::swapF32(p_normal_cube->z);
                p_normal_cube->w = sead::Endian::swapF32(p_normal_cube->w);

                p_normal_cube++;
            }
        }
        p_normal_cube = static_cast<sead::Vector4f*>(mCPUTextureCubeNormal.mPtr);
        TextureDataInitializerRAW::initialize(
            &mCPUTextureCubeNormal.mTextureData,
            p_normal_cube, mCPUTextureCubeNormal.mSize,
            cTextureFormat_R32_G32_B32_A32_float,
            size_cube, size_cube
        );
        mCPUTextureCubeNormal.mTextureSampler.applyTextureData(mCPUTextureCubeNormal.mTextureData);
        mCPUTextureCubeNormal.mTextureSampler.setFilter(
            cTextureFilterType_Point,
            cTextureFilterType_Point,
            cTextureMipFilterType_None
        );
    }
}

void LightMapMgr::constructList_()
{
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        lmap.mParentIndex = searchIndex(*lmap.mParentMap);
        lmap.mpParent = getLightMap(lmap.mParentIndex);
        lmap.mCopyIndex = searchIndex(*lmap.mCopyMap);
        lmap.mChildIndex = -1;
        lmap.mChild.clear();
    }
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        if (lmap.mpParent == nullptr)
            continue;
        lmap.mpParent->mChildIndex = 0;
        lmap.mpParent->mChild.pushBack(&lmap);
        lmap.mChildIndex = lmap.mpParent->mChild.size();
    }
    sead::Buffer<LightMap>::iterator
        itr_lmap_begin = mLightMap.begin(),
        itr_lmap_end = mLightMap.end();
    {
        for (
            sead::Buffer<LightMap>::iterator
                itr = itr_lmap_begin,
                itr_end = itr_lmap_end
            ;   itr != itr_end
            ; ++itr
        )
        {
            // Empty loop 1
        }
        for (
            sead::Buffer<LightMap>::iterator
                itr = itr_lmap_begin,
                itr_end = itr_lmap_end
            ;   itr != itr_end
            ; ++itr
        )
        {
            // Empty loop 2
        }
    }
    constructMemory_();
}

void LightMapMgr::constructMemory_()
{
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        lmap.initializeTexture_(mCPUTextureSphereNormal.mTextureData.getWidth(), mCPUTextureCubeNormal.mTextureData.getWidth());
        if (lmap.mpParent == nullptr)
            lmap.constructMemory_(mpLightMapHeap);
    }
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        if (lmap.mpParent != nullptr)
            lmap.constructMemory_(mpLightMapHeap);
    }
}

void LightMapMgr::update()
{
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        if (lmap.mFlag.isOnBit(3))
        {
            constructList_();
            break;
        }
        if (lmap.mFlag.isOnBit(2))
        {
            constructMemory_();
            break;
        }
    }

    mLightMapCalc.clear();
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        lmap.update_();
        if (lmap.mFlag.isOnBit(5) && lmap.mCopyIndex == -1)
            mLightMapCalc.pushBack(&lmap);
    }
    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
    {
        LightMap& lmap = *itr_lmap;
        if (lmap.mFlag.isOnBit(5) && lmap.mCopyIndex != -1)
            mLightMapCalc.pushBack(&lmap);
    }
}

void LightMapMgr::updateGPU(bool invalidate_gpu) const
{
    for (
        sead::PtrArray<LightMap>::constIterator
            itr_lmap = mLightMapCalc.constBegin(),
            itr_lmap_end = mLightMapCalc.constEnd()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
        itr_lmap->updateGPU_(invalidate_gpu);
}

void LightMapMgr::updateViewGPU(s32 view_index, bool ignore_mapping_type, const sead::Matrix34f& view_mtx, bool invalidate_gpu) const
{
    sead::PtrArray<LightMap>::constIterator
        itr_lmap = mLightMapCalc.constBegin(),
        itr_lmap_end = mLightMapCalc.constEnd();

    if (ignore_mapping_type)
    {
        for (; itr_lmap != itr_lmap_end; ++itr_lmap)
        {
            const LightMap& lmap = *itr_lmap;
            lmap.updateViewGPU_(view_index, view_mtx, invalidate_gpu);
        }
    }
    else
    {
        for (; itr_lmap != itr_lmap_end; ++itr_lmap)
        {
            const LightMap& lmap = *itr_lmap;
            if (*lmap.mMappingType != 2)
                lmap.updateViewGPU_(view_index, view_mtx, invalidate_gpu);
        }
    }
}

ShaderMode LightMapMgr::draw(s32 view_index, bool ignore_mapping_type, ShaderMode mode) const
{
    LightMap::DrawArg arg;
    arg.mpGraphicsContext = &mGraphicsContext;
    arg.mNormalMap[LightMap::cMappingType_Sphere] = &mCPUTextureSphereNormal.mTextureSampler;
    arg.mNormalMap[LightMap::cMappingType_SphereBackside] = &mCPUTextureSphereNormal.mTextureSampler;
    arg.mNormalMap[LightMap::cMappingType_Cube] = &mCPUTextureCubeNormal.mTextureSampler;
    arg.mpLUT = &mCPUTextureLUT.mTextureSampler;

    sead::PtrArray<LightMap>::constIterator
        itr_lmap = mLightMapCalc.constBegin(),
        itr_lmap_end = mLightMapCalc.constEnd();

    if (ignore_mapping_type)
    {
        for (; itr_lmap != itr_lmap_end; ++itr_lmap)
        {
            const LightMap& lmap = *itr_lmap;
            mode = lmap.draw_(view_index, arg, mode);
        }
    }
    else
    {
        for (; itr_lmap != itr_lmap_end; ++itr_lmap)
        {
            const LightMap& lmap = *itr_lmap;
            if (*lmap.mMappingType != 2)
                mode = lmap.draw_(view_index, arg, mode);
        }
    }

    return mode;
}

s32 LightMapMgr::searchIndex(const sead::SafeString& name) const
{
    for (
        sead::Buffer<LightMap>::constIterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
        if (name.isEqual(itr_lmap->getName()))
            return itr_lmap.getIndex();

    return -1;
}

void LightMapMgr::postRead_()
{
    constructList_();

    for (
        sead::Buffer<LightMap>::iterator
            itr_lmap = mLightMap.begin(),
            itr_lmap_end = mLightMap.end()
        ;   itr_lmap != itr_lmap_end
        ; ++itr_lmap
    )
        itr_lmap->bindLUT_();

    updateLUT_();
}

LightMapMgr::CreateArg::CreateArg()
    : mViewMax(1)
    , mSizeSphere(48)
    , mSizeCube(32)
    , mNum(0)
    , _10(1)
{
}

LightMapMgr::CPUTexture::CPUTexture()
    : mPtr(nullptr)
    , mSize(0)
{
}

LightMapMgr::CPUTexture::~CPUTexture()
{
    delete[] static_cast<u8*>(mPtr);
}

} }
