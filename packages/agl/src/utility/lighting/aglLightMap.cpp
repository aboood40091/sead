#include <detail/aglRootNode.h>
#include <detail/aglShaderHolder.h>
#include <environment/aglLight.h>
#include <gfx/seadViewport.h>
#include <lighting/aglLightMap.h>
#include <lighting/aglLightMapMgr.h>
#include <utility/aglPrimitiveShape.h>
#include <utility/aglPrimitiveTexture.h>
#include <utility/aglVertexAttributeHolder.h>

namespace agl { namespace lght {

LightMap::LightMap()
    : mpLightMapMgr(nullptr)
    , mpParent(nullptr)
    , mFlag(1 << 1 |
            1 << 3)
    , mChildIndex(-1)
    , mLightObjNum(0)
    , mParentIndex(-1)
    , mCopyIndex(-1)
    , mDirection(sead::Vector3f::ez)
    , mLightColor(sead::Color4f::cBlack)
{
    mRimLightRef.setType(env::DirectionalLight::getType());

    static_assert(offsetof(LightMap, mArrayNode) == 0x73C);
    static_assert(offsetof(LightMap, mLightingHint) == 0x740);
}

LightMap::~LightMap()
{
    mEnvObjEx.freeBuffer();
    mMipUniformBlock.freeBuffer();
    mViewInfo.freeBuffer();
}

void LightMap::initialize_(const LightMapMgr* p_mgr, const env::EnvObjBuffer::AllocateArg& alloc_arg, s32 view_max, sead::Heap* heap)
{
    mpLightMapMgr = p_mgr;

    env::EnvObjSet::allocBuffer(alloc_arg, heap);

    mViewInfo.allocBuffer(view_max, heap);
    for (
        sead::Buffer<ViewInfo>::iterator
            itr_view_info = mViewInfo.begin(),
            itr_view_info_end = mViewInfo.end()
        ;   itr_view_info != itr_view_info_end
        ; ++itr_view_info
    )
    {
        ViewInfo& view_info = *itr_view_info;
        if (itr_view_info.getIndex() == 0)
        {
            view_info.mUniformBlock.startDeclare(cLightViewMemberNum, heap);
            view_info.mUniformBlock.declare(UniformBlock::cType_vec4, 1);               // cRimColor
            view_info.mUniformBlock.declare(UniformBlock::cType_vec4, cLightObjMax);    // cLightDir
        }
        else
        {
            view_info.mUniformBlock.declare(mViewInfo[0].mUniformBlock);
        }
        view_info.mUniformBlock.create(heap);
    }

    mUniformBlock.startDeclare(cLightMemberNum, heap);
    mUniformBlock.declare(UniformBlock::cType_float, 1);            // cRimPow
    mUniformBlock.declare(UniformBlock::cType_float, 1);            // cRimWidth
    mUniformBlock.declare(UniformBlock::cType_float, cLightObjMax); // cParamLight
    mUniformBlock.create(heap);

    mMipUniformBlock.allocBuffer(cMipMax, heap);
    for (
        sead::Buffer<UniformBlock>::iterator
            itr_mip_uniform_block = mMipUniformBlock.begin(),
            itr_mip_uniform_block_end = mMipUniformBlock.end()
        ;   itr_mip_uniform_block != itr_mip_uniform_block_end
        ; ++itr_mip_uniform_block
    )
    {
        UniformBlock& mip_uniform_block = *itr_mip_uniform_block;
        if (itr_mip_uniform_block.getIndex() == 0)
        {
            mip_uniform_block.startDeclare(cLightMipMemberNum, heap);
            mip_uniform_block.declare(UniformBlock::cType_float, cLightObjMax);     // cSpecPow
            mip_uniform_block.declare(UniformBlock::cType_vec4, cLightObjMax * 2);  // cParamLightMip
        }
        else
        {
            mip_uniform_block.declare(mMipUniformBlock[0]);
        }
        mip_uniform_block.create(heap);
    }

    mLightingHint   .initialize(0,                                                          "lighting_hint",    "タイプ",               &mSetting);  // label is... "type"?
    mHdrEnable      .initialize(false,                                                      "hdr_enable",       "HDR",                  &mSetting);
    mRimLightRef    .initialize(sead::FixedSafeString<32>(sead::SafeString::cEmptyString),  "rim_light_ref",    "参照",                 &mSetting);
    mRimEnable      .initialize(false,                                                      "rim_enable",       "有効",                 &mSetting);
    mRimEffect      .initialize(1.0f,                                                       "rim_effect",       "ライトカラーの影響",   &mSetting);
    mRimWidth       .initialize(1.0f,                                                       "rim_width",        "深さ",                 &mSetting);
    mRimAngle       .initialize(1.0f,                                                       "rim_angle",        "指向性",               &mSetting);
    mRimPow         .initialize(2.0f,                                                       "rim_pow",          "絞り",                 &mSetting);
    mParentMap      .initialize(sead::FixedSafeString<32>(sead::SafeString::cEmptyString),  "parent_map",       "親ライトマップ",       &mSetting);
    mCopyMap        .initialize(sead::FixedSafeString<32>(sead::SafeString::cEmptyString),  "copy_map",         "コピーライトマップ",   &mSetting);
    mMappingType    .initialize(cMappingType_Sphere,                                        "mapping_type",     "マッピングタイプ",     &mSetting);

    mEnvObjEx.allocBuffer(mEnvObjPtrBuffer.size(), heap);
    for (
        sead::Buffer<EnvObjEx>::iterator
            itr_ex = mEnvObjEx.begin(),
            itr_ex_end = mEnvObjEx.end()
        ;   itr_ex != itr_ex_end
        ; ++itr_ex
    )
    {
        bool enable_mip_1 = searchType(itr_ex.getIndex()) == env::DirectionalLight::getType();
        Ref& ref = mObjRefArrayBuffer[itr_ex.getIndex()];

        itr_ex->mCalcType   .initialize(0,                                                          "calc_type",    "計算タイプ",           &ref);
        itr_ex->mLUTName    .initialize(sead::FixedSafeString<32>(sead::SafeString::cEmptyString),  "lut_name",     "LUT",                  &ref);
        itr_ex->mEffect     .initialize(1.0f,                                                       "effect",       "ライトカラーの影響",   &ref);
        itr_ex->mPow        .initialize(50.0f,                                                      "pow",          "絞り",                 &ref);
        itr_ex->mPowMipMax  .initialize(2.0f,                                                       "pow_mip_max",  "絞り(ラフネス最大)",   &ref);
        itr_ex->mEnableMip0 .initialize(true,                                                       "enable_mip0",  "通常時有効",           &ref);
        itr_ex->mEnableMip1 .initialize(enable_mip_1,                                               "enable_mip1",  "シャドウ時有効",       &ref);

        *itr_ex->mLUTName = *mpLightMapMgr->mLUT[0].mName;
        bindLUT_(itr_ex.getIndex());
    }

    mChild.allocBuffer(mpLightMapMgr->mLightMap.size(), heap);
    mChild.clear();

    *mName = "LightMap";

    detail::RootNode::setNodeMeta(this, "Icon=TEXTURE");
    detail::RootNode::setNodeMeta(&mArrayNode, "Icon=FOLDER_BLUE");
}

void LightMap::update_()
{
    s32 prev_light_obj_num = mLightObjNum;
    mLightObjNum = 0;
    mDirection.set(sead::Vector3f::ez);
    mFlag.resetBit(5);

    if (mFlag.isOnBit(1))
    {
        if (mFlag.isOnBit(2) || *mMappingType != cMappingType_Cube)
            mFlag.setBit(4);

        for (
            sead::Buffer<env::EnvObj*>::iterator
                itr_p_obj = mEnvObjPtrBuffer.begin(),
                itr_p_obj_end = mEnvObjPtrBuffer.end()
            ;   itr_p_obj != itr_p_obj_end
            ; ++itr_p_obj
        )
        {
            env::EnvObj* p_obj = *itr_p_obj;
            if (p_obj == nullptr || !p_obj->isEnable())
                continue;

            p_obj->getTypeID();
            EnvObjEx& ex = mEnvObjEx[itr_p_obj.getIndex()];
            LightObj& light_obj = mLightObj[mLightObjNum];

            if (getLightingHint() == 0 && mFlag.isOnBit(0))
            {
                light_obj.mFlag.changeBit(0, *ex.mEnableMip0);
                light_obj.mFlag.changeBit(1, *ex.mEnableMip1);
            }
            else
            {
                light_obj.mFlag.setBit(0);
                light_obj.mFlag.setBit(1);
            }

            s32 num = applyEnvObjData(p_obj, &ex, mLightObjNum);
            if (num == 0)
                continue;

            if (p_obj->getTypeID() == mRimLightRef.getType() && p_obj->getIndex() == mRimLightRef.getIndex())
            {
                mLightColor = light_obj.mLightColorMinMax[1];
                mDirection.set(light_obj.mDirection);
            }

            f32 effect = *ex.mEffect;
            {
                // Max
                sead::Color4f color = light_obj.mLightColorMinMax[1] * effect;
                light_obj.mLightColorMinMaxWithEffect[1] = color;
                // Min
                color = light_obj.mLightColorMinMax[0] * effect;
                light_obj.mLightColorMinMaxWithEffect[0] = color;
            }

            mLightObjNum += num;
            if (mLightObjNum >= cLightObjMax)
            {
                mLightObjNum = cLightObjMax;
                break;
            }
        }
    }

    if (mFlag.isOnBit(4) || prev_light_obj_num != mLightObjNum)
        mFlag.setBit(5);

    if (mLightObjNum > 0)
        mTextureSampler.applyTextureData(mTextureData);

    else
    {
        if (mpParent == nullptr && mChild.isEmpty() && mCopyIndex == -1)
        {
            switch (*mMappingType)
            {
            default:
                break;
            case cMappingType_Cube:
                if (mTextureData.getTextureType() == cTextureType_CubeMap)
                    mTextureSampler.applyTextureData(utl::PrimitiveTexture::instance()->getTextureSampler(utl::PrimitiveTexture::cSampler_BlackCube).getTextureData());
                else
                    mTextureSampler.applyTextureData(utl::PrimitiveTexture::instance()->getTextureSampler(utl::PrimitiveTexture::cSampler_BlackCubeArray).getTextureData());
                break;
            case cMappingType_Sphere:
                mTextureSampler.applyTextureData(utl::PrimitiveTexture::instance()->getTextureSampler(utl::PrimitiveTexture::cSampler_Black2D).getTextureData());
                break;
            case cMappingType_SphereBackside:
                mTextureSampler.applyTextureData(utl::PrimitiveTexture::instance()->getTextureSampler(utl::PrimitiveTexture::cSampler_Black2DArray).getTextureData());
                break;
            }

            mFlag.resetBit(5);
        }
        else
        {
            mTextureSampler.applyTextureData(mTextureData);
        }
    }

    mFlag.resetBit(2);
    mFlag.resetBit(3);
    mFlag.resetBit(4);
}

void LightMap::updateGPU_(bool invalidate_gpu) const
{
    if (mFlag.isOffBit(5))
        return;

    mUniformBlock.setFloat(cLightMemberIndex_RimPow, *mRimPow);
    mUniformBlock.setFloat(cLightMemberIndex_RimWidth, *mRimWidth);
    f32 inv_lut_num = 1.0f / mpLightMapMgr->mCPUTextureLUT.mTextureData.getHeight();
    for (
        sead::UnsafeArray<LightObj, cLightObjMax>::constIterator
            itr_light_obj = mLightObj.begin(),
            itr_light_obj_end = mLightObj.toIterator(mLightObjNum)
        ;   itr_light_obj != itr_light_obj_end
        ; ++itr_light_obj
    )
    {
        const LightObj& light_obj = *itr_light_obj;
        mUniformBlock.setFloat(
            cLightMemberIndex_ParamLight,
            (light_obj.mLUTType + 0.5f) * inv_lut_num,
            itr_light_obj.getIndex()
        );
    }
    mUniformBlock.flushNoSync(invalidate_gpu);

    u32 mip_num = mTextureData.getMipLevelNum();
    for (
        sead::Buffer<UniformBlock>::constIterator
            itr_mip_uniform_block = mMipUniformBlock.begin(),
            itr_mip_uniform_block_end = mMipUniformBlock.toIterator(mip_num)
        ;   itr_mip_uniform_block != itr_mip_uniform_block_end
        ; ++itr_mip_uniform_block
    )
    {
        const UniformBlock& mip_uniform_block = *itr_mip_uniform_block;

        f32 inv_mip_num = mip_num > 1 ? (1.0f / (mip_num - 1)) : 0.0f;

        static const sead::Color4f c_default_color_min_max[2] = {
            sead::Color4f::cBlack,
            sead::Color4f::cBlack
        };

        switch (getLightingHint())
        {
        default:
            break;
        case 0:
            for (
                sead::UnsafeArray<LightObj, cLightObjMax>::constIterator
                    itr_light_obj = mLightObj.begin(),
                    itr_light_obj_end = mLightObj.toIterator(mLightObjNum)
                ;   itr_light_obj != itr_light_obj_end
                ; ++itr_light_obj
            )
            {
                const LightObj& light_obj = *itr_light_obj;

                mip_uniform_block.setFloat(
                    cLightMipMemberIndex_SpecPow,
                    1.0f,
                    itr_light_obj.getIndex()
                );

                const sead::Color4f* light_color =
                    light_obj.mFlag.isOnBit(itr_mip_uniform_block.getIndex())
                        ? light_obj.mLightColorMinMaxWithEffect
                        : c_default_color_min_max;

                mip_uniform_block.setVector4f(
                    cLightMipMemberIndex_ParamLightMip,
                    reinterpret_cast<const sead::Vector4f*>(&(light_color[0].r)),
                    2,
                    itr_light_obj.getIndex() * 2
                );
            }
            break;
        case 1:
            for (
                sead::UnsafeArray<LightObj, cLightObjMax>::constIterator
                    itr_light_obj = mLightObj.begin(),
                    itr_light_obj_end = mLightObj.toIterator(mLightObjNum)
                ;   itr_light_obj != itr_light_obj_end
                ; ++itr_light_obj
            )
            {
                const LightObj& light_obj = *itr_light_obj;

                f32 pow_log = std::log(light_obj.mPow);
                f32 pow_mip_max_log = std::log(light_obj.mPowMipMax);

                mip_uniform_block.setFloat(
                    cLightMipMemberIndex_SpecPow,
                    std::exp(pow_log + (pow_mip_max_log - pow_log) * itr_mip_uniform_block.getIndex() * inv_mip_num),
                    itr_light_obj.getIndex()
                );
                mip_uniform_block.setVector4f(
                    cLightMipMemberIndex_ParamLightMip,
                    reinterpret_cast<const sead::Vector4f*>(&(light_obj.mLightColorMinMaxWithEffect[0].r)),
                    2,
                    itr_light_obj.getIndex() * 2
                );
            }
            break;
        case 2:
            for (
                sead::UnsafeArray<LightObj, cLightObjMax>::constIterator
                    itr_light_obj = mLightObj.begin(),
                    itr_light_obj_end = mLightObj.toIterator(mLightObjNum)
                ;   itr_light_obj != itr_light_obj_end
                ; ++itr_light_obj
            )
            {
                const LightObj& light_obj = *itr_light_obj;

                mip_uniform_block.setFloat(
                    cLightMipMemberIndex_SpecPow,
                    light_obj.mPow,
                    itr_light_obj.getIndex()
                );
                mip_uniform_block.setVector4f(
                    cLightMipMemberIndex_ParamLightMip,
                    reinterpret_cast<const sead::Vector4f*>(&(light_obj.mLightColorMinMaxWithEffect[0].r)),
                    2,
                    itr_light_obj.getIndex() * 2
                );
            }
            break;
        }

        mip_uniform_block.flush(invalidate_gpu);
    }
}

void LightMap::updateViewGPU_(s32 view_index, const sead::Matrix34f& view_mtx, bool invalidate_gpu) const
{
    if (mFlag.isOffBit(5))
        return;

    const ViewInfo& view_info = mViewInfo[view_index];

    MappingType mapping_type = getMappingType();
    for (
        sead::UnsafeArray<LightObj, cLightObjMax>::constIterator
            itr_light_obj = mLightObj.begin(),
            itr_light_obj_end = mLightObj.toIterator(mLightObjNum)
        ;   itr_light_obj != itr_light_obj_end
        ; ++itr_light_obj
    )
    {
        const LightObj& light_obj = *itr_light_obj;

        if (mapping_type == cMappingType_Cube)
        {
            sead::Vector3f dir = light_obj.mDirection;
            dir.normalize();

            view_info.mUniformBlock.setVector4f(
                cLightViewMemberIndex_LightDir,
                sead::Vector4f(dir.x, dir.y, dir.z, 0.0f),
                itr_light_obj.getIndex()
            );
        }
        else
        {
            sead::Vector3f dir;
            dir.setRotate(view_mtx, light_obj.mDirection);
            switch (light_obj.mCalcType)
            {
            default:
                break;
            case 0:
                dir.normalize();
                break;
            case 1:
                dir.z += -1.0f;
                dir.normalize();
                break;
            case 2:
                dir.set(0.0f, 0.0f, -1.0f);
                break;
            }

            view_info.mUniformBlock.setVector4f(
                cLightViewMemberIndex_LightDir,
                sead::Vector4f(dir.x, dir.y, dir.z, 0.0f),
                itr_light_obj.getIndex()
            );
        }
    }

    sead::Vector3f dir;
    dir.setRotate(view_mtx, mDirection);

    f32 rim_intensity = 0.0f;
    if (*mRimEnable)
    {
        f32 rim_angle = (*mRimAngle);
        rim_intensity = (sead::Mathf::clamp2(0.0f, dir.z, 1.0f) - 1.0f);
        rim_intensity *= rim_angle;
        rim_intensity += 1.0f;
        rim_intensity *= (*mRimEffect);
    }

    view_info.mUniformBlock.setVector4f(
        cLightViewMemberIndex_RimColor,
        sead::Vector4f(
            mLightColor.r * rim_intensity,
            mLightColor.g * rim_intensity,
            mLightColor.b * rim_intensity,
            mLightColor.a * rim_intensity
        )
    );

    view_info.mUniformBlock.flush(invalidate_gpu);
}

ShaderMode LightMap::draw_(s32 view_index, const DrawArg& arg, ShaderMode mode) const
{
    if (mFlag.isOffBit(5))
        return mode;

    if (mCopyIndex != -1 && mpParent == nullptr)
        mpLightMapMgr->getLightMap(mCopyIndex)->getTextureSampler().getTextureData().copyToAll(&mTextureData);

    const ViewInfo& view_info = mViewInfo[view_index];

    (*arg.mpGraphicsContext)[getMappingType()][mCopyIndex != -1].apply();

    const ShaderProgram* program;
    if (mLightObjNum > 0)
    {
        // This part is intentionally non-matching, but because it
        // is better than Nintendo's method which was on crack.
        s32 variation = ((getMappingType()                              // LM_TEXTURE_TYPE
                            * cLightObjMax + mLightObjNum - 1)          // LM_LIGHT_NUM
                                * cLightingHintMax + getLightingHint()) // LM_POW_TYPE
                                    * cBoolMax + !*mHdrEnable;          // LM_CLAMP

        program = detail::ShaderHolder::instance()->getShader(detail::ShaderHolder::cShader_Lightmap).getVariation(variation);
    }
    else
    {
        program = &detail::ShaderHolder::instance()->getShader(detail::ShaderHolder::cShader_LightmapClear);
    }
    mode = program->activate(mode);

  //SEAD_ASSERT( arg.mNormalMap[ getMappingType() ] != nullptr );
  //SEAD_ASSERT( arg.mpLUT != nullptr );
    arg.mNormalMap[getMappingType()]->activate(program->getSamplerLocation(cSampler_Normal));
    arg.mpLUT->activate(program->getSamplerLocation(cSampler_LUT));

    mUniformBlock.setUniform(program->getUniformBlockLocation(cUniformBlockID_Light));
    view_info.mUniformBlock.setUniform(program->getUniformBlockLocation(cUniformBlockID_LightView));

    utl::VertexAttributeHolder::instance()->getVertexAttribute(utl::VertexAttributeHolder::cAttribute_QuadTriangle).activate();

    for (u32 i = 0; i < mTextureData.getMipLevelNum(); i++)
    {
        const Context& context = view_info.mContext[i];

        context.mRenderBuffer.bind();

        sead::Viewport viewport(context.mRenderBuffer);
        viewport.apply(context.mRenderBuffer);

        mMipUniformBlock[i].setUniform(program->getUniformBlockLocation(cUniformBlockID_LightMip));

        utl::PrimitiveShape::instance()->getIdxStreamQuadTriangle().draw();
    }

    return mode;
}

void LightMap::bindLUT_(s32 obj_index)
{
    for (s32 i = 0; i < LightMapMgr::cLUT_Num; i++)
    {
        if (*(mpLightMapMgr->mLUT[i].mName) == *(mEnvObjEx[obj_index].mLUTName))
        {
            mEnvObjEx[obj_index].mLUTType = i;
            return;
        }
    }
    mEnvObjEx[obj_index].mLUTType = -1;
}

void LightMap::bindLUT_()
{
    for (
        sead::Buffer<EnvObjEx>::constIterator
            itr_ex = mEnvObjEx.constBegin(),
            itr_ex_end = mEnvObjEx.constEnd()
        ;   itr_ex != itr_ex_end
        ; ++itr_ex
    )
        bindLUT_(itr_ex.getIndex());
}

void LightMap::initializeRenderBuffer_(s32 view_index, u32 mip_level)
{
    ViewInfo& view_info = mViewInfo[view_index];

    f32 size = mTextureData.getWidth() >> mip_level;
    view_info.mContext[mip_level].mRenderBuffer.setVirtualSize(size, size);
    view_info.mContext[mip_level].mRenderBuffer.setPhysicalArea(0.0f, 0.0f, size, size);

    view_info.mContext[mip_level].mRenderBuffer.setRenderTargetColorNullAll();
    if (getMappingType() == cMappingType_Cube)
    {
        for (s32 slice = 0; slice < cSliceNum_Cube; slice++)
        {
            view_info.mContext[mip_level].mColorTarget[slice].setMipLevel(mip_level);
            view_info.mContext[mip_level].mRenderBuffer.setRenderTargetColor(&view_info.mContext[mip_level].mColorTarget[slice], slice);
        }
    }
    else
    {
        view_info.mContext[mip_level].mColorTarget[0].setMipLevel(mip_level);
        view_info.mContext[mip_level].mRenderBuffer.setRenderTargetColor(&view_info.mContext[mip_level].mColorTarget[0], 0);
        if (getMappingType() == cMappingType_SphereBackside)
        {
            view_info.mContext[mip_level].mColorTarget[1].setMipLevel(mip_level);
            view_info.mContext[mip_level].mRenderBuffer.setRenderTargetColor(&view_info.mContext[mip_level].mColorTarget[1], 1);
        }
    }
}

void LightMap::initializeTexture_(u32 size_sphere, u32 size_cube)
{
    if (mFlag.isOnBit(6))
    {
        u8* image_ptr = static_cast<u8*>(mTextureData.getImagePtr());
        if (image_ptr != nullptr)
            delete[] image_ptr;

        u8* mip_ptr = static_cast<u8*>(mTextureData.getMipPtr());
        if (mip_ptr != nullptr)
            delete[] mip_ptr;

        mFlag.resetBit(6);
    }

    if (mpParent != nullptr)
        return;

    bool is_array = mChild.size() > 0 || mFlag.isOnBit(7);
    s32 array_num = is_array ? mChild.size() + 1 : 1;
    switch (*mMappingType)
    {
    default:
      //SEAD_ASSERT(0);
        break;
    case cMappingType_Sphere:
        if (is_array)
            mTextureData.initializeArray(cTextureFormat_R11_G11_B10_float, size_sphere, size_sphere, array_num, cMipMax);
        else
            mTextureData.initialize(cTextureFormat_R11_G11_B10_float, size_sphere, size_sphere, cMipMax);
        break;
    case cMappingType_SphereBackside:
        mTextureData.initializeArray(cTextureFormat_R11_G11_B10_float, size_sphere, size_sphere, array_num * cSliceNum_SphereBackside, cMipMax);
        break;
    case cMappingType_Cube:
        if (is_array)
            mTextureData.initializeCubeMapArray(cTextureFormat_R11_G11_B10_float, size_cube, size_cube, array_num, cMipMax);
        else
            mTextureData.initializeCubeMap(cTextureFormat_R11_G11_B10_float, size_cube, size_cube, cMipMax);
        break;
    }
}

void LightMap::constructMemory_(sead::Heap* heap)
{
    if (mpParent == nullptr)
    {
      //SEAD_ASSERT(mTextureData.getImagePtr() == nullptr);
      //SEAD_ASSERT(mTextureData.getMipPtr() == nullptr);
        u8* image_ptr = new (heap, mTextureData.getAlignment()) u8[mTextureData.getImageByteSize()];
        u8* mip_ptr = new (heap, mTextureData.getAlignment()) u8[mTextureData.getMipByteSize()];
        DCFlushRangeNoSync(image_ptr, mTextureData.getImageByteSize());
        DCFlushRangeNoSync(mip_ptr, mTextureData.getMipByteSize());
        mTextureData.setImagePtr(image_ptr);
        mTextureData.setMipPtr(mip_ptr);
        mFlag.setBit(6);
    }
    else
    {
        mTextureData = mpParent->mTextureData;
    }

    u32 mip_level_num = 1;
    switch (getLightingHint())
    {
    default:
        break;
    case 0:
        if (mFlag.isOnBit(0))
            mip_level_num = 2;
        break;
    case 1:
        mip_level_num = cMipMax;
        break;
    }
    mTextureData.setMipLevelNum(mip_level_num);

    for (
        sead::Buffer<ViewInfo>::iterator
            itr_view_info = mViewInfo.begin(),
            itr_view_info_end = mViewInfo.end()
        ;   itr_view_info != itr_view_info_end
        ; ++itr_view_info
    )
    {
        s32 array_base_index = mChildIndex != -1 ? mChildIndex : 0;
        for (u32 mip_level = 0; mip_level < sead::Mathu::max(1, mip_level_num); mip_level++)
        {
            if (getMappingType() == cMappingType_Cube)
            {
                for (s32 slice = 0; slice < cSliceNum_Cube; slice++)
                {
                    itr_view_info->mContext[mip_level].mColorTarget[slice].setSlice(array_base_index * cSliceNum_Cube + slice);
                    itr_view_info->mContext[mip_level].mColorTarget[slice].applyTextureData(mTextureData);
                }
            }
            else
            {
                itr_view_info->mContext[mip_level].mColorTarget[0].applyTextureData(mTextureData);
                if (getMappingType() == cMappingType_SphereBackside)
                {
                    itr_view_info->mContext[mip_level].mColorTarget[0].setSlice(array_base_index * cSliceNum_SphereBackside + 0);
                    itr_view_info->mContext[mip_level].mColorTarget[1].setSlice(array_base_index * cSliceNum_SphereBackside + 1);

                    itr_view_info->mContext[mip_level].mColorTarget[1].applyTextureData(mTextureData);
                }
                else
                {
                    itr_view_info->mContext[mip_level].mColorTarget[0].setSlice(array_base_index);
                }
            }
            initializeRenderBuffer_(itr_view_info.getIndex(), mip_level);
        }
    }

    mTextureSampler.setFilter(
        cTextureFilterType_Linear,
        cTextureFilterType_Linear,
        mip_level_num > 1 ? cTextureMipFilterType_Linear : cTextureMipFilterType_None
    );

    mFlag.setBit(2);
}

bool LightMap::preRead_()
{
    if (env::EnvObjSet::preRead_())
    {
        for (
            sead::Buffer<env::EnvObj*>::constIterator
                itr_p_obj = mEnvObjPtrBuffer.constBegin(),
                itr_p_obj_end = mEnvObjPtrBuffer.constEnd()
            ;   itr_p_obj != itr_p_obj_end
            ; ++itr_p_obj
        )
        {
            s32 index = itr_p_obj.getIndex();
            *(mEnvObjEx[index].mEnableMip0) = true;
            if (searchType(index) == env::DirectionalLight::getType())
                *(mEnvObjEx[index].mEnableMip1) = false;
            else
                *(mEnvObjEx[index].mEnableMip1) = true;
        }
        return true;
    }
    return false;
}

void LightMap::postRead_()
{
    env::EnvObjSet::postRead_();
    mFlag.setBit(4);
    mpEnvObjMgr->reconstruct();
}

void LightMap::genMessageEachObj(sead::hostio::Context*, s32, const env::EnvObj*)
{
    // Deleted from NSMBU
}

bool LightMap::preWrite_() const
{
    for (
        sead::Buffer<EnvObjEx>::constIterator
            itr_ex = mEnvObjEx.begin(),
            itr_ex_end = mEnvObjEx.end()
        ;   itr_ex != itr_ex_end
        ; ++itr_ex
    )
    {
        EnvObjEx& ex = const_cast<EnvObjEx&>(*itr_ex);
        if (ex.mLUTType != -1)
            *(ex.mLUTName) = *(mpLightMapMgr->mLUT[ex.mLUTType].mName);
    }
    return true;
}

void LightMap::callbackNotAppliable_(utl::IParameterObj* p_param_obj, utl::ResParameter res_param)
{
    for (
        sead::Buffer<EnvObjEx>::iterator
            itr_ex = mEnvObjEx.begin(),
            itr_ex_end = mEnvObjEx.end()
        ;   itr_ex != itr_ex_end
        ; ++itr_ex
    )
    {
        EnvObjEx& ex = *itr_ex;
        if (&(mObjRefArrayBuffer[itr_ex.getIndex()]) == p_param_obj)
        {
            if (res_param.isApply(utl::ParameterBase::calcHash("lut_type")))
            {
                utl::Parameter<s32> lut_type;
                lut_type.applyResource(res_param);

                *(ex.mLUTName) = *(mpLightMapMgr->mLUT[*lut_type].mName);
            }
        }
    }
}

s32 LightMap::applyEnvObjData(const env::EnvObj* p_obj, const EnvObjEx* p_ex, s32 light_index)
{
  //SEAD_ASSERT(p_ex != nullptr);

    s32 type = p_obj->getTypeID();
    LightObj& light_obj = mLightObj[light_index];

    setCalcType(light_index, *(p_ex->mCalcType));
    setPow(light_index, *(p_ex->mPow));
    setPowMipMax(light_index, *(p_ex->mPowMipMax));

    if (type == env::AmbientLight::getType())
    {
        const agl::env::AmbientLight* p_env_obj = static_cast<const env::AmbientLight*>(p_obj);

        sead::Color4f color;
        {
            f32 intensity = p_env_obj->getIntensity();
            sead::Color4f color_intensity(intensity, intensity, intensity, intensity);
            color = p_env_obj->getColor() * color_intensity;
        }
        setColorMinMax(light_index, color, color);

        setDirection(light_index, sead::Vector3f::ez);

        setLUTType(light_index, p_ex->mLUTType);
        return 1;
    }
    else if (type == env::DirectionalLight::getType())
    {
        const agl::env::DirectionalLight* p_env_obj = static_cast<const env::DirectionalLight*>(p_obj);

        bool use_specular = true;
        if ((getLightingHint() != 2 || light_obj.mCalcType != 1) && getLightingHint() != 1)
            use_specular = false;

        sead::Color4f diffuse_color;
        sead::Color4f specular_color;
        sead::Color4f backside_color;
        {
            f32 intensity = p_env_obj->getIntensity();
            sead::Color4f color_intensity(intensity, intensity, intensity, intensity);
            diffuse_color = p_env_obj->getDiffuseColor() * color_intensity;
            specular_color = p_env_obj->getSpecularColor() * color_intensity;
            backside_color = p_env_obj->getBacksideColor() * color_intensity;
        }
        if (use_specular)
            setColorMinMax(light_index, sead::Color4f::cBlack, specular_color);
        else
            setColorMinMax(light_index, backside_color, diffuse_color);

        setDirection(light_index, p_env_obj->getDirection());

        setLUTType(light_index, p_ex->mLUTType);
        return 1;
    }
    else if (type == env::HemisphereLight::getType())
    {
        const agl::env::HemisphereLight* p_env_obj = static_cast<const env::HemisphereLight*>(p_obj);

        sead::Color4f sky_color;
        sead::Color4f ground_color;
        {
            f32 intensity = p_env_obj->getIntensity();
            sead::Color4f color_intensity(intensity, intensity, intensity, intensity);
            sky_color = p_env_obj->getSkyColor() * color_intensity;
            ground_color = p_env_obj->getGroundColor() * color_intensity;
        }
        setColorMinMax(light_index, ground_color, sky_color);

        setDirection(light_index, -p_env_obj->getDirection());

        setLUTType(light_index, LightMapMgr::cLUT_Hemisphere);
        return 1;
    }
    else
    {
        return 0;
    }
}

LightMap::LightObj::LightObj()
    : mCalcType(0)
    , mLUTType(0)
    , mFlag(0)
    , mDirection(-1.0f, -1.0f, -1.0f)
    , mPow(1.0f)
{
    mLightColorMinMax[0] = sead::Color4f::cBlack;
    mLightColorMinMax[1] = sead::Color4f::cBlack;
    mLightColorMinMaxWithEffect[0] = sead::Color4f::cBlack;
    mLightColorMinMaxWithEffect[1] = sead::Color4f::cBlack;
}

} }
