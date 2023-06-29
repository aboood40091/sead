#include <detail/aglRootNode.h>
#include <detail/aglShaderHolder.h>
#include <postfx/aglDepthOfField.h>
#include <resource/seadArchiveRes.h>
#include <utility/aglImageFilter2D.h>

namespace agl { namespace detail {

SEAD_SINGLETON_DISPOSER_IMPL(ShaderHolder)

ShaderHolder::ShaderHolder()
    : mCreateDisplayLists(true)
{
}

ShaderHolder::~ShaderHolder()
{
}

void ShaderHolder::initialize(sead::ArchiveRes* p_arc, sead::Heap* heap)
{
    // SEAD_ASSERT(p_arc != nullptr);

    static const char* sProgramArchiveNames[cArchiveType_Num] = {
        "agl_common",
        "agl_technique",
        "simple_model"
    };

    const sead::UnsafeArray<ShaderProgramArchive, cArchiveType_Num>::iterator itr_archive_begin = mProgramArchive.begin();
    const sead::UnsafeArray<ShaderProgramArchive, cArchiveType_Num>::iterator itr_archive_end = mProgramArchive.begin();

    for (sead::UnsafeArray<ShaderProgramArchive, cArchiveType_Num>::iterator itr_archive = itr_archive_begin; itr_archive != itr_archive_end; ++itr_archive)
    {
        void* sharc = p_arc->getFile(
            sead::FormatFixedSafeString<1024>(
                "%s.%s",
                sProgramArchiveNames[itr_archive.getIndex()],
                ResShaderArchiveData::getExtension()
            )
        );

        void* sharcfb = p_arc->getFile(
            sead::FormatFixedSafeString<1024>(
                "%s.%s",
                sProgramArchiveNames[itr_archive.getIndex()],
                ResBinaryShaderArchiveData::getExtension()
            )
        );

        if (sharc || sharcfb)
        {
            itr_archive->createWithOption(
                ResBinaryShaderArchive(sharcfb),
                ResShaderArchive(sharc),
                (!mCreateDisplayLists) |
                (1 << 1),
                heap
            );
        }
    }

    static const struct {
        ArchiveType archive;
        const char* name;
    } sShaders[cShader_Num] = {
        { cArchiveType_Common,      "dev_util"                                 },
        { cArchiveType_Common,      "depth_visualize"                          },
        { cArchiveType_Common,      "texture"                                  },
        { cArchiveType_Common,      "reduce_2"                                 },
        { cArchiveType_Common,      "reduce_4"                                 },
        { cArchiveType_Common,      "reduce_8"                                 },
        { cArchiveType_Common,      "reduce_16"                                },
        { cArchiveType_Technique,   "color_correction"                         },
        { cArchiveType_Technique,   "color_correction_map"                     },
        { cArchiveType_Common,      "xlu_snap"                                 },
        { cArchiveType_Common,      "image_filter_gaussian"                    },
        { cArchiveType_Common,      "image_filter_cubic"                       },
        { cArchiveType_Common,      "image_filter_blur"                        },
        { cArchiveType_Common,      "texture_mult_color"                       },
        { cArchiveType_Common,      "red"                                      },
        { cArchiveType_Common,      "green"                                    },
        { cArchiveType_Common,      "blue"                                     },
        { cArchiveType_Common,      "alpha"                                    },
        { cArchiveType_Common,      "depth_raw"                                },
        { cArchiveType_Common,      "depth_linear"                             },
        { cArchiveType_Common,      "texture_2d_array"                         },
        { cArchiveType_Common,      "texture_3d"                               },
        { cArchiveType_Common,      "texture_cube_map"                         },
        { cArchiveType_Common,      "texture_cube_map_array"                   },
        { cArchiveType_Common,      "texture_texcoord"                         },
        { cArchiveType_Common,      "texture_multi_sample_1x"                  },
        { cArchiveType_Common,      "texture_multi_sample_2x"                  },
        { cArchiveType_Common,      "texture_multi_sample_4x"                  },
        { cArchiveType_Common,      "texture_multi_sample_8x"                  },
        { cArchiveType_Common,      "alpha_modify"                             },
        { cArchiveType_Common,      "texture_color_drift"                      },
        { cArchiveType_Common,      "color_quad"                               },
        { cArchiveType_Common,      "top_bottom_color"                         },
        { cArchiveType_Common,      "draw_imm"                                 },
        { cArchiveType_Common,      "draw_fan"                                 },
        { cArchiveType_Common,      "frame_buffer_flip_y"                      },
        { cArchiveType_Common,      "frame_buffer_no_flip"                     },
        { cArchiveType_Technique,   "bloom_mask"                               },
        { cArchiveType_Technique,   "bloom_gaussian"                           },
        { cArchiveType_Technique,   "bloom_compose"                            },
        { cArchiveType_Technique,   "hdr_compose"                              },
        { cArchiveType_Technique,   "dof_near_mask"                            },
        { cArchiveType_Technique,   "dof_mipmap"                               },
        { cArchiveType_Technique,   "dof_depth_mask"                           },
        { cArchiveType_Technique,   "dof_final"                                },
        { cArchiveType_Technique,   "dof_vignetting"                           },
        { cArchiveType_Technique,   "fxaa"                                     },
        { cArchiveType_Technique,   "fxaa_coloronly"                           },
        { cArchiveType_Technique,   "fxaa_luma"                                },
        { cArchiveType_Technique,   "fxaa_reprojection"                        },
        { cArchiveType_Technique,   "filter_aa_reprojection"                   },
        { cArchiveType_Technique,   "reduce_aa"                                },
        { cArchiveType_Technique,   "lightmap"                                 },
        { cArchiveType_Technique,   "lightmap_clear"                           }, // <-- Not in Nintendo Land
        { cArchiveType_Technique,   "light_pre_pass_point_light"               },
        { cArchiveType_Technique,   "light_pre_pass_spot_light"                },
        { cArchiveType_Technique,   "light_pre_pass_proj_light"                },
        { cArchiveType_Technique,   "light_pre_pass_double_same_pos_with_back" },
        { cArchiveType_Technique,   "light_pre_pass_tbr"                       },
        { cArchiveType_Technique,   "light_pre_pass_tbr_sp"                    },
        { cArchiveType_Technique,   "light_pre_pass_quad"                      },
        { cArchiveType_Technique,   "light_pre_pass_dev"                       },
        { cArchiveType_Technique,   "multi_filter_reduce"                      },
        { cArchiveType_Technique,   "multi_filter_expand"                      },
        { cArchiveType_Technique,   "static_depth_shadow"                      },
        { cArchiveType_Common,      "cubemap_gaussian"                         },
        { cArchiveType_Common,      "cubemap_color_correction"                 },
        { cArchiveType_Common,      "cubemap_head_extract"                     },
        { cArchiveType_Common,      "cubemap_head_convolute_first"             },
        { cArchiveType_Common,      "cubemap_head_convolute_other"             },
        { cArchiveType_Common,      "cubemap_hdr_compose"                      },
        { cArchiveType_Common,      "cubemap_draw_illuminant"                  },
        { cArchiveType_Common,      "nv12decode"                               },
        { cArchiveType_Technique,   "screen_pick"                              },
        { cArchiveType_Common,      "debug_cubemap"                            },
        { cArchiveType_Technique,   "depth_shadow_debug"                       },
        { cArchiveType_Technique,   "vsm"                                      },
        { cArchiveType_Technique,   "ssao_ao_buffer"                           },
        { cArchiveType_Technique,   "ssao_blur"                                },
        { cArchiveType_Technique,   "ssao_reduce"                              },
        { cArchiveType_Technique,   "ssao_mask"                                },
        { cArchiveType_Technique,   "occlusion_query"                          },
        { cArchiveType_Technique,   "occlusion_renderer_clear_buf"             },
        { cArchiveType_Technique,   "occlusion_renderer"                       },
        { cArchiveType_Technique,   "occluded_effect_lensflare"                },
        { cArchiveType_Common,      "texture_compress"                         },
        { cArchiveType_Technique,   "shadow_mask"                              },
        { cArchiveType_Technique,   "radial_blur"                              },
        { cArchiveType_Technique,   "radial_blur_compose"                      },
        { cArchiveType_Common,      "debug_primitive"                          },
        { cArchiveType_Common,      "debug_shape_instanced"                    },
        { cArchiveType_Common,      "debug_point_instanced"                    },
        { cArchiveType_Common,      "debug_line_instanced"                     },
        { cArchiveType_Common,      "debug_triangle_instanced"                 }
    };

    for (s32 i = 0; i < cShader_Num; i++)
    {
        const ArchiveType archive = sShaders[i].archive;
        const ShaderProgramArchive& program_archive = mProgramArchive[archive];
        if (!program_archive.isValid())
        {
            mProgram.pushBack(nullptr);
            continue;
        }

        const char* const name = sShaders[i].name;
        s32 program_index = program_archive.searchShaderProgramIndex(name);
        // SEAD_ASSERT_MSG(program_index >= 0, "Cannot find a shader program (name = %s) in the shader archieve (ArchiveType = %d).\n", name, s32(archive));

        ShaderProgram* p = const_cast<ShaderProgram*>(program_archive.searchShaderProgram(program_index));
        // SEAD_ASSERT(p != nullptr);
        mProgram.pushBack(p);

        switch (ShaderType(i))
        {
        default:
            break;
        case cShader_DevUtil:
            p->createUniform(6, heap);
            {
                p->setUniformName(1, "cWorldViewMatrix[0]");
                p->setUniformName(0, "cWorldViewProjectionMatrix[0]");
                p->setUniformName(2, "cDiffuseColor0");
                p->setUniformName(3, "cDiffuseColor1");
                p->setUniformName(4, "cSpecularColor");
                p->setUniformName(5, "cViewLightDir");
            }
            break;
        case cShader_DepthVisualize:
            p->createUniform(5, heap);
            {
                p->setUniformName(0, "cNear");
                p->setUniformName(1, "cFar");
                p->setUniformName(2, "cDepth[0]");
                p->setUniformName(3, "cColor[0]");
                p->setUniformName(4, "cLineNum");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cTexDepth");
            }
            break;
        case cShader_Texture:
        case cShader_Reduce2:
        case cShader_Reduce4:
        case cShader_Reduce8:
        case cShader_Reduce16:
        //
        case cShader_ImageFilterGaussian:
        case cShader_ImageFilterCubic:
        case cShader_ImageFilterBlur:
        case cShader_TextureMultColor:
        case cShader_Red:
        case cShader_Green:
        case cShader_Blue:
        case cShader_Alpha:
        case cShader_DepthRaw:
        case cShader_DepthLinear:
        case cShader_Texture2DArray:
        case cShader_Texture3D:
        case cShader_TextureCubeMap:
        case cShader_TextureCubeMapArray:
        case cShader_TextureTexCoord:
        case cShader_TextureMultiSample1x:
        case cShader_TextureMultiSample2x:
        case cShader_TextureMultiSample4x:
        case cShader_TextureMultiSample8x:
        case cShader_AlphaModify:
        case cShader_TextureColorDrift:
        case cShader_ColorQuad:
        case cShader_TopBottomColor:
        //
        case cShader_NV12Decode:
            p->createAttribute(utl::ImageFilter2D::cAttribute_Num, heap);
            {
                p->setAttributeName(utl::ImageFilter2D::cAttribute_Position,        "aPosition");
            }
            p->createUniform(utl::ImageFilter2D::cUniform_Num, heap);
            {
                p->setUniformName(utl::ImageFilter2D::cUniform_ProjViewWorld,       "uProjViewWorld[0]");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexSize,             "cTexSize");
                p->setUniformName(utl::ImageFilter2D::cUniform_MipLevel,            "uMipLevel");
                p->setUniformName(utl::ImageFilter2D::cUniform_Slice,               "uSlice");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexScale,            "uTexScale");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexRotate,           "uTexRotate");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexTrans,            "uTexTrans");
                p->setUniformName(utl::ImageFilter2D::cUniform_MultiSampleNum,      "uMultiSampleNum");
                p->setUniformName(utl::ImageFilter2D::cUniform_Color,               "uColor");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexFlipY,            "uTexFlipY");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexFetchAdjust,      "uTexFetchAdjust");
                p->setUniformName(utl::ImageFilter2D::cUniform_ColorDriftR,         "cColorDriftR");
                p->setUniformName(utl::ImageFilter2D::cUniform_ColorDriftG,         "cColorDriftG");
                p->setUniformName(utl::ImageFilter2D::cUniform_ColorDriftB,         "cColorDriftB");
                p->setUniformName(utl::ImageFilter2D::cUniform_ColorT,              "uColorT");
                p->setUniformName(utl::ImageFilter2D::cUniform_ColorB,              "uColorB");
                p->setUniformName(utl::ImageFilter2D::cUniform_CubeMapFace,         "uCubeMapFace");
                p->setUniformName(utl::ImageFilter2D::cUniform_BlurOffset,          "uBlurOffset");
                p->setUniformName(utl::ImageFilter2D::cUniform_DepthNear,           "uDepthNear");
                p->setUniformName(utl::ImageFilter2D::cUniform_InvDepthRange,       "uInvDepthRange");
                p->setUniformName(utl::ImageFilter2D::cUniform_OneMinusNearDivFar,  "uOneMinusNearDivFar");
                p->setUniformName(utl::ImageFilter2D::cUniform_NV12DecodeParam,     "cNV12DecodeParam");
            }
            p->createSamplerLocation(utl::ImageFilter2D::cSampler_Num, heap);
            {
                p->setSamplerLocationName(utl::ImageFilter2D::cSampler_Texture,     "uTexture");
                p->setSamplerLocationName(utl::ImageFilter2D::cSampler_Texture_1,   "uTexture_1");
            }
            break;
        case cShader_ColorCorrection:
        case cShader_ColorCorrectionMap:
            p->createUniform(3, heap);
            {
                p->setUniformName(0, "c3DTexCoordOffset");
                p->setUniformName(1, "cHSBG");
                p->setUniformName(2, "cCurve[0]");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "cTexColor");
                p->setSamplerLocationName(1, "cTexCorrectionMap");
            }
            break;
        case cShader_XluSnap:
            p->createUniform(utl::ImageFilter2D::cXluSnapUniform_Num, heap);
            {
                p->setUniformName(utl::ImageFilter2D::cUniform_ProjViewWorld,               "uProjViewWorld[0]");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexSize,                     "cTexSize");
                p->setUniformName(utl::ImageFilter2D::cUniform_TexScale,                    "uTexCoordScale");
                p->setUniformName(utl::ImageFilter2D::cUniform_MipLevel,                    "uMipLevel");
                p->setUniformName(utl::ImageFilter2D::cXluSnapUniform_Alpha,                "uAlpha");
            }
            p->createSamplerLocation(utl::ImageFilter2D::cXluSnapSampler_Num, heap);
            {
                p->setSamplerLocationName(utl::ImageFilter2D::cSampler_Texture,             "uTexture");
                p->setSamplerLocationName(utl::ImageFilter2D::cXluSnapSampler_SnapColor,    "uSnapColor");
                p->setSamplerLocationName(utl::ImageFilter2D::cXluSnapSampler_SnapDepth,    "uSnapDepth");
                p->setSamplerLocationName(utl::ImageFilter2D::cXluSnapSampler_TargetDepth,  "uTargetDepth");
            }
            break;
        case cShader_DrawImm:
            p->createUniform(5, heap);
            {
                p->setUniformName(0, "cProjViewWorld[0]");
                p->setUniformName(1, "cColor");
                p->setUniformName(2, "cPosition[0]");
                p->setUniformName(3, "cPosition[1]");
                p->setUniformName(4, "cPosition[2]");
            }
            break;
        case cShader_DrawFan:
            p->createUniform(3, heap);
            {
                p->setUniformName(0, "cProjViewWorld[0]");
                p->setUniformName(1, "cArcInfo");
                p->setUniformName(2, "cColor");
            }
            break;
        case cShader_FrameBufferFlipY:
        case cShader_FrameBufferNoFlip:
            p->createAttribute(utl::ImageFilter2D::cAttribute_Num, heap);
            {
                p->setAttributeName(utl::ImageFilter2D::cAttribute_Position,                            "aPosition");
            }
            p->createUniform(utl::ImageFilter2D::cUniform_Num, heap);
            {
                p->setUniformName(utl::ImageFilter2D::cUniform_ProjViewWorld,                           "uProjViewWorld[0]");
                p->setUniformName(utl::ImageFilter2D::cUniform_MipLevel,                                "uMipLevel");
            }
            p->createSamplerLocation(utl::ImageFilter2D::cFrameBufferCopySampler_Num, heap);
            {
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_0,   "uTextureColor_0");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_2,   "uTextureColor_1");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_2,   "uTextureColor_2");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_3,   "uTextureColor_3");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_4,   "uTextureColor_4");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_5,   "uTextureColor_5");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_6,   "uTextureColor_6");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureColor_7,   "uTextureColor_7");
                p->setSamplerLocationName(utl::ImageFilter2D::cFrameBufferCopySampler_TextureDepth,     "uTextureDepth");
            }
            break;
        case cShader_BloomMask:
        case cShader_BloomGaussian:
        case cShader_BloomCompose:
            p->createUniform(8, heap);
            {
                p->setUniformName(0, "cParam0");
                p->setUniformName(1, "cParam1");
                p->setUniformName(2, "cParam2");
                p->setUniformName(3, "cParam3");
                p->setUniformName(4, "cParam4");
                p->setUniformName(5, "cParam5");
                p->setUniformName(6, "cParam6");
                p->setUniformName(7, "cParam7");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "cTextureColor");
                p->setSamplerLocationName(1, "cTextureDepth");
            }
            break;
        case cShader_HDRCompose:
            p->createUniform(3, heap);
            {
                p->setUniformName(1, "cExposure");
                p->setUniformName(2, "cColorCorrectionCoeff");
            }
            p->createSamplerLocation(3, heap);
            {
                p->setSamplerLocationName(0, "cHDRImage");
                p->setSamplerLocationName(1, "cBloom");
                p->setSamplerLocationName(2, "cColorCorrectionTable");
            }
            break;
        case cShader_DOFNearMask:
        case cShader_DOFMipmap:
        case cShader_DOFDepthMask:
        case cShader_DOFFinal:
        case cShader_DOFVignetting:
            p->createUniform(pfx::DepthOfField::cUniform_Num, heap);
            {
                p->setUniformName(pfx::DepthOfField::cUniform_Param0,                   "cParam0");
                p->setUniformName(pfx::DepthOfField::cUniform_IndirectTexParam,         "cIndirectTexParam");
                p->setUniformName(pfx::DepthOfField::cUniform_IndirectTexMtx0,          "cIndirectTexMtx0");
                p->setUniformName(pfx::DepthOfField::cUniform_IndirectTexMtx1,          "cIndirectTexMtx1");
                p->setUniformName(pfx::DepthOfField::cUniform_MulParam,                 "cMulParam");
                p->setUniformName(pfx::DepthOfField::cUniform_AddParam,                 "cAddParam");
                p->setUniformName(pfx::DepthOfField::cUniform_NearFarParam,             "cNearFarParam");
                p->setUniformName(pfx::DepthOfField::cUniform_TexParam,                 "cTexParam");
                p->setUniformName(pfx::DepthOfField::cUniform_VignettingRadius,         "cVignettingRadius");
                p->setUniformName(pfx::DepthOfField::cUniform_VignettingParam,          "cVignettingParam");
                p->setUniformName(pfx::DepthOfField::cUniform_VignettingTrans,          "cVignettingTrans");
                p->setUniformName(pfx::DepthOfField::cUniform_VignettingColor,          "cVignettingColor");
                p->setUniformName(pfx::DepthOfField::cUniform_FarMulColor,              "cFarMulColor");
            }
            p->createSamplerLocation(pfx::DepthOfField::cSampler_Num, heap);
            {
                p->setSamplerLocationName(pfx::DepthOfField::cSampler_TexColor,         "cTexColor");
                p->setSamplerLocationName(pfx::DepthOfField::cSampler_TexDepth,         "cTexDepth");
                p->setSamplerLocationName(pfx::DepthOfField::cSampler_TexMipMap,        "cTexMipMap");
                p->setSamplerLocationName(pfx::DepthOfField::cSampler_TexMipMapDepth,   "cTexMipMapDepth");
                p->setSamplerLocationName(pfx::DepthOfField::cSampler_TexIndirect,      "cTexIndirect");
            }
            break;
        case cShader_FXAA:
        case cShader_FXAAColorOnly:
        case cShader_FXAALuma:
        case cShader_FXAAReprojection:
        case cShader_FilterAAReprojection:
            p->createUniform(9, heap);
            {
                p->setUniformName(0, "cFrameSize");
                p->setUniformName(1, "cFrameRCP1H");
                p->setUniformName(2, "cFrameRCPOffset");
                p->setUniformName(3, "cLumaCoeff");
                p->setUniformName(4, "cEdgeThreshold");
                p->setUniformName(5, "cAlphaOut");
                p->setUniformName(6, "cPrevScreenCoordTrx[0]");
                p->setUniformName(7, "cNearFar");
                p->setUniformName(8, "cNearFarCache");
            }
            p->createSamplerLocation(5, heap);
            {
                p->setSamplerLocationName(0, "cTexBase");
                p->setSamplerLocationName(1, "cTexLuma");
                p->setSamplerLocationName(2, "cTexReduce");
                p->setSamplerLocationName(3, "cTexDepth");
                p->setSamplerLocationName(4, "cTexPrev");
            }
            break;
        case cShader_ReduceAA:
            p->createUniform(1, heap);
            {
                p->setUniformName(0, "cFrameRCP");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "cTexNormal");
                p->setSamplerLocationName(1, "cTexReduce");
            }
            break;
        case cShader_Lightmap:
        case cShader_LightmapClear:
            p->createUniformBlock(3, heap);
            {
                p->setUniformBlockName(0, "Light");
                p->setUniformBlockName(1, "LightView");
                p->setUniformBlockName(2, "LightMip");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "cLUT");
                p->setSamplerLocationName(1, "cNormal");
            }
            break;
        case cShader_LightPrePassPointLight:
        case cShader_LightPrePassSpotLight:
        case cShader_LightPrePassProjLight:
        //
        case cShader_LightPrePassQuad:
            p->createUniformBlock(4, heap);
            {
                p->setUniformBlockName(0, "Context");
                p->setUniformBlockName(1, "PointLightView");
                p->setUniformBlockName(2, "SpotLightView");
                p->setUniformBlockName(3, "ProjLightView");
            }
            p->createSamplerLocation(6, heap);
            {
                p->setSamplerLocationName(0, "cDepth");
                p->setSamplerLocationName(1, "cNormal");
                p->setSamplerLocationName(2, "cSpecPowTable");
                p->setSamplerLocationName(3, "cProjTex");
                p->setSamplerLocationName(4, "cDepthShadow");
                p->setSamplerLocationName(5, "cAlbedo");
            }
            break;
        case cShader_LightPrePassDoubleSamePosWithBack:
            p->createUniformBlock(2, heap);
            {
                p->setUniformBlockName(0, "Context");
                p->setUniformBlockName(1, "DoubleSamePosWithBack");
            }
            p->createSamplerLocation(6); // They forgot to pass the heap here
            {
                p->setSamplerLocationName(0, "cDepth");
                p->setSamplerLocationName(1, "cNormal");
                p->setSamplerLocationName(2, "cSpecPowTable");
                p->setSamplerLocationName(3, "cProjTex");
                p->setSamplerLocationName(4, "cDepthShadow");
                p->setSamplerLocationName(5, "cAlbedo");
            }
            break;
        case cShader_LightPrePassTBR:
        case cShader_LightPrePassTBRSp:
            p->createUniformBlock(1, heap);
            {
                p->setUniformBlockName(0, "PointLightView");
            }
            p->createSamplerLocation(3, heap);
            {
                p->setSamplerLocationName(0, "cDepth");
                p->setSamplerLocationName(1, "cNormal");
                p->setSamplerLocationName(2, "cSpecPowTable");
            }
            break;
        case cShader_LightPrePassDev:
            p->createUniform(3, heap);
            {
                p->setUniformName(0, "cWVP[0]");
                p->setUniformName(1, "cColor");
                p->setUniformName(2, "cSpotCone_CenterScale");
            }
            break;
        case cShader_MultiFilterReduce:
        case cShader_MultiFilterExpand:
            p->createAttribute(1, heap);
            {
                p->setAttributeName(0, "aPosition");
            }
            p->createUniform(4, heap);
            {
                p->setUniformName(0, "cTexOffsetScale");
                p->setUniformName(1, "cTexWidthInv");
                p->setUniformName(2, "cTexOffsetAdjust");
                p->setUniformName(3, "c3DTexCoordOffset");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "cTexture");
            }
            break;
        case cShader_StaticDepthShadow:
            p->createUniform(1, heap);
            {
                p->setUniformName(0, "uParam0");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "uTexture");
            }
            break;
        case cShader_CubeMapGaussian:
            p->createUniform(7, heap);
            {
                p->setUniformName(0, "cTexFlipY");
                p->setUniformName(1, "cCubeMapFace");
                p->setUniformName(2, "cCubeMapArrayIndex");
                p->setUniformName(3, "cMipLevel");
                p->setUniformName(4, "cTextureWidth");
                p->setUniformName(5, "cAngleScale");
                p->setUniformName(6, "cOffsetScale");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "sTexCube");
            }
            break;
        case cShader_CubeMapColorCorrection:
            p->createUniform(7, heap);
            {
                p->setUniformName(0, "cTexFlipY");
                p->setUniformName(2, "cCubeMapFace");
                p->setUniformName(3, "cCubeMapArrayIndex");
                p->setUniformName(1, "cMipLevel");
                p->setUniformName(4, "cTextureWidth");
                p->setUniformName(5, "cColorPower");
                p->setUniformName(6, "cColorScale");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "sTexCube");
            }
            break;
        case cShader_CubeMapHeadExtract:
            p->createUniform(5, heap);
            {
                p->setUniformName(0, "cTexFlipY");
                p->setUniformName(2, "cCubeMapFace");
                p->setUniformName(3, "cCubeMapArrayIndex");
                p->setUniformName(1, "cMipLevel");
                p->setUniformName(4, "cTextureWidth");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "sTexCube");
                p->setSamplerLocationName(1, "sTexCorrectValue");
            }
            break;
        case cShader_CubeMapConvoluteFirst:
            p->createUniform(2, heap);
            {
                p->setUniformName(0, "cTexFlipY");
                p->setUniformName(1, "cTextureWidth");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "sTexHeadArray");
            }
            break;
        case cShader_CubeMapConvoluteOther:
            p->createUniform(2, heap);
            {
                p->setUniformName(0, "cTexFlipY");
                p->setUniformName(1, "cTextureWidth");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "sTexHead");
            }
            break;
        case cShader_CubeMapHDRCompose:
            p->createUniform(5, heap);
            {
                p->setUniformName(0, "cTexFlipY");
                p->setUniformName(1, "cMipLevel");
                p->setUniformName(2, "cCubeMapFace");
                p->setUniformName(3, "cCubeMapArrayIndex");
                p->setUniformName(4, "cTextureWidth");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "sTexCube");
                p->setSamplerLocationName(1, "sTexCorrectValue");
            }
            break;
        case cShader_CubeMapDrawIlluminant:
            p->createUniform(6, heap);
            {
                p->setUniformName(0, "cCenterPosH");
                p->setUniformName(1, "cAspect");
                p->setUniformName(2, "cScale");
                p->setUniformName(3, "cColor");
                p->setUniformName(4, "cIntensity");
                p->setUniformName(5, "cTexFlipY");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "sTexture");
            }
            break;
        case cShader_ScreenPick:
            p->createAttribute(4, heap);
            {
                p->setAttributeName(0, "aPosition");
                p->setAttributeName(1, "aBlendWeight");
                p->setAttributeName(2, "aBlendIndex");
                p->setAttributeName(3, "aTexCoord0");
            }
            p->createUniformBlock(4, heap);
            {
                p->setUniformBlockName(0, "View");
                p->setUniformBlockName(1, "Skeleton");
                p->setUniformBlockName(2, "Material");
                p->setUniformBlockName(3, "Shape");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cAlbedo");
            }
            break;
        case cShader_DebugCubeMap:
            p->createUniform(9, heap);
            {
                p->setUniformName(0, "cProjViewMtx[0]");
                p->setUniformName(1, "cWorldMtx[0]");
                p->setUniformName(2, "cCamWorld");
                p->setUniformName(3, "cUseReflection");
                p->setUniformName(4, "cUseMipLevel");
                p->setUniformName(5, "cUseHDRCoeff");
                p->setUniformName(6, "cAlphaAdd");
                p->setUniformName(7, "cMipLevel");
                p->setUniformName(8, "cCubeMapArrayIndex");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "sTexCube");
                p->setSamplerLocationName(1, "sTexCorrectValue");
            }
            break;
        case cShader_DepthShadowDebug:
            p->createUniform(5, heap);
            {
                p->setUniformName(0, "cPVW[0]");
                p->setUniformName(1, "cNearFarParam");
                p->setUniformName(2, "cInvTextureSize");
                p->setUniformName(3, "cTextureIndex");
                p->setUniformName(4, "cTextureIndexY");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cTexture");
            }
            break;
        case cShader_VSM:
            p->createUniform(2, heap);
            {
                p->setUniformName(0, "cInvTexSize");
                p->setUniformName(1, "cSlice");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cTexture");
            }
            break;
        case cShader_SSAOBuffer:
        case cShader_SSAOBlur:
        case cShader_SSAOReduce:
        case cShader_SSAOMask:
            p->createUniform(8, heap);
            {
                p->setUniformName(0, "cAOParam0");
                p->setUniformName(1, "cAOParam1");
                p->setUniformName(2, "cNearFar");
                p->setUniformName(3, "cInvTexSize");
                p->setUniformName(4, "cSamplePoint[0]");
                p->setUniformName(5, "cRotate[0]");
                p->setUniformName(6, "cReprojectionMtx[0]");
                p->setUniformName(7, "cReprojectionMtxZ");
            }
            p->createSamplerLocation(5, heap);
            {
                p->setSamplerLocationName(0, "cTexture");
                p->setSamplerLocationName(1, "cTextureRotate");
                p->setSamplerLocationName(2, "cTextureHistory");
                p->setSamplerLocationName(3, "cTextureDepth");
                p->setSamplerLocationName(4, "cTextureMask");
            }
            p->createUniformBlock(1, heap);
            {
                p->setUniformBlockName(0, "cAO_UB");
            }
            break;
        case cShader_OcclusionQuery:
            p->createUniformBlock(3, heap);
            {
                p->setUniformBlockName(0, "View");
                p->setUniformBlockName(1, "AABB");
                p->setUniformBlockName(2, "Sphere");
            }
            break;
        case cShader_OcclusionRendererClearBuf:
            // Empty
            break;
        case cShader_OcclusionRenderer:
            p->createUniformBlock(1, heap);
            {
                p->setUniformBlockName(0, "Context");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cDepth");
            }
            break;
        case cShader_OccludedEffectLensFlare:
            p->createUniformBlock(1, heap);
            {
                p->setUniformBlockName(0, "Element");
            }
            p->createSamplerLocation(3, heap);
            {
                p->setSamplerLocationName(0, "cOcclusion");
                p->setSamplerLocationName(1, "cTexture");
                p->setSamplerLocationName(2, "cTexture2");
            }
            break;
        case cShader_TextureCompress:
            p->createUniform(4, heap);
            {
                p->setUniformName(0, "cTexSizeRCP");
                p->setUniformName(1, "cReferLevel");
                p->setUniformName(2, "cReferSlice");
                p->setUniformName(3, "cReferFace");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cTexture");
            }
            break;
        case cShader_ShadowMask:
            p->createUniformBlock(3, heap);
            {
                p->setUniformBlockName(0, "Common");
                p->setUniformBlockName(1, "SphereBlock");
                p->setUniformBlockName(2, "CylinderBlock");
            }
            p->createSamplerLocation(2, heap);
            {
                p->setSamplerLocationName(0, "cLinearDepth");
                p->setSamplerLocationName(1, "cTexColor");
            }
            break;
        case cShader_RadialBlur:
        case cShader_RadialBlurCompose:
            p->createUniform(3, heap);
            {
                p->setUniformName(0, "cRadius");
                p->setUniformName(1, "cBlurPosition");
                p->setUniformName(2, "cVtxColor[0]");
            }
            p->createSamplerLocation(1, heap);
            {
                p->setSamplerLocationName(0, "cTexture");
            }
            break;
        case cShader_DebugPrimitive:
            p->createUniform(3, heap);
            {
                p->setUniformName(0, "cPVW[0]");
                p->setUniformName(1, "cWorld[0]");
                p->setUniformName(2, "cColor");
            }
            break;
        case cShader_DebugShapeInstanced:
            p->createUniformBlock(5, heap);
            {
                p->setUniformBlockName(0, "View");
                p->setUniformBlockName(1, "ShapeInfoId");
                p->setUniformBlockName(2, "ShapeInfo");
            }
            break;
        case cShader_DebugPointInstanced:
            p->createUniformBlock(5, heap);
            {
                p->setUniformBlockName(0, "View");
                p->setUniformBlockName(3, "PointInfoId");
                p->setUniformBlockName(4, "PointInfo");
            }
            break;
        case cShader_DebugLineInstanced:
            p->createUniformBlock(5, heap);
            {
                p->setUniformBlockName(0, "View");
                p->setUniformBlockName(3, "PointInfoId");
                p->setUniformBlockName(4, "PointInfo");
            }
            break;
        case cShader_DebugTriangleInstanced:
            p->createUniformBlock(5, heap);
            {
                p->setUniformBlockName(0, "View");
                p->setUniformBlockName(3, "PointInfoId");
                p->setUniformBlockName(4, "PointInfo");
            }
            break;
        }
    }

    for (sead::UnsafeArray<ShaderProgramArchive, cArchiveType_Num>::iterator itr_archive = itr_archive_begin; itr_archive != itr_archive_end; ++itr_archive)
        if (itr_archive->isValid())
            itr_archive->setUp();

    RootNode::setNodeMeta(this, "Icon = CIRCLE_BLUE");
}

} }
