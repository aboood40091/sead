#ifndef SEAD_PTCL_DATA_H_
#define SEAD_PTCL_DATA_H_

#include <basis/seadTypes.h>

#include <nw/eft/eft_Data.h>

namespace sead { namespace ptcl {

enum SystemConstants
{
    cEftBinaryVersion       = nw::eft::EFT_BINARY_VERSION,
    cEftNumEmitterInset     = nw::eft::EFT_EMITTER_INSET_NUM,
    cEftNumTexturePattern   = nw::eft::EFT_TEXTURE_PATTERN_NUM,
    cEftInfinitLife         = nw::eft::EFT_INFINIT_LIFE,
    cEftMaxGroup            = nw::eft::EFT_GROUP_MAX,
    cEftMaxCallback         = nw::eft::EFT_CALLBACK_MAX,
    cEftPtclBinaryAlignment = nw::eft::EFT_PTCL_BINARY_ALIGNMENT,
    cEftDefaultDrawPriority = nw::eft::EFT_DEFAULT_DRAW_PRIORITY
};

enum CpuCore
{
    cCpuCore_0      = nw::eft::EFT_CPU_CORE_0,
    cCpuCore_1      = nw::eft::EFT_CPU_CORE_1,
    cCpuCore_2      = nw::eft::EFT_CPU_CORE_2,
    cCpuCore_Max    = nw::eft::EFT_CPU_CORE_MAX
};

enum EmitterType
{
    cEmitterType_Simple     = nw::eft::EFT_EMITTER_TYPE_SIMPLE,
    cEmitterType_Complex    = nw::eft::EFT_EMITTER_TYPE_COMPLEX,
    cEmitterType_Max        = nw::eft::EFT_EMITTER_TYPE_MAX
};

enum PtclType
{
    cPtclType_Simple    = nw::eft::EFT_PTCL_TYPE_SIMPLE,
    cPtclType_Complex   = nw::eft::EFT_PTCL_TYPE_COMPLEX,
    cPtclType_Child     = nw::eft::EFT_PTCL_TYPE_CHILD,
    cPtclType_Max       = nw::eft::EFT_PTCL_TYPE_MAX
};

enum PtclFollowType
{
    cFollowType_All     = nw::eft::EFT_FOLLOW_TYPE_ALL,
    cFollowType_None    = nw::eft::EFT_FOLLOW_TYPE_NONE,
    cFollowType_PosOnly = nw::eft::EFT_FOLLOW_TYPE_POS_ONLY,
    cFollowType_Max     = nw::eft::EFT_FOLLOW_TYPE_MAX
};

enum UserDataCallBackID
{
    cUserDataCallBackID_None    = nw::eft::EFT_USER_DATA_CALLBACK_ID_NONE,
    cUserDataCallBackID_0       = nw::eft::EFT_USER_DATA_CALLBACK_ID_0,
    cUserDataCallBackID_1       = nw::eft::EFT_USER_DATA_CALLBACK_ID_1,
    cUserDataCallBackID_2       = nw::eft::EFT_USER_DATA_CALLBACK_ID_2,
    cUserDataCallBackID_3       = nw::eft::EFT_USER_DATA_CALLBACK_ID_3,
    cUserDataCallBackID_4       = nw::eft::EFT_USER_DATA_CALLBACK_ID_4,
    cUserDataCallBackID_5       = nw::eft::EFT_USER_DATA_CALLBACK_ID_5,
    cUserDataCallBackID_6       = nw::eft::EFT_USER_DATA_CALLBACK_ID_6,
    cUserDataCallBackID_7       = nw::eft::EFT_USER_DATA_CALLBACK_ID_7,
    cUserDataCallBackID_Max     = nw::eft::EFT_USER_DATA_CALLBACK_ID_MAX
};

typedef GroupFlag nw::eft::GroupFlag;
#define SEAD_PTCL_GROUP_FLAG( groupID ) EFT_GROUP_FLAG( groupID )

enum DrawPathFlag
{
    cDrawPathFlag_0     = nw::eft::EFT_DRAW_PATH_FLAG_0,
    cDrawPathFlag_1     = nw::eft::EFT_DRAW_PATH_FLAG_1,
    cDrawPathFlag_2     = nw::eft::EFT_DRAW_PATH_FLAG_2,
    cDrawPathFlag_3     = nw::eft::EFT_DRAW_PATH_FLAG_3,
    cDrawPathFlag_4     = nw::eft::EFT_DRAW_PATH_FLAG_4,
    cDrawPathFlag_5     = nw::eft::EFT_DRAW_PATH_FLAG_5,
    cDrawPathFlag_6     = nw::eft::EFT_DRAW_PATH_FLAG_6,
    cDrawPathFlag_7     = nw::eft::EFT_DRAW_PATH_FLAG_7,
    cDrawPathFlag_8     = nw::eft::EFT_DRAW_PATH_FLAG_8,
    cDrawPathFlag_9     = nw::eft::EFT_DRAW_PATH_FLAG_9,
    cDrawPathFlag_10    = nw::eft::EFT_DRAW_PATH_FLAG_10,
    cDrawPathFlag_11    = nw::eft::EFT_DRAW_PATH_FLAG_11,
    cDrawPathFlag_12    = nw::eft::EFT_DRAW_PATH_FLAG_12,
    cDrawPathFlag_13    = nw::eft::EFT_DRAW_PATH_FLAG_13,
    cDrawPathFlag_14    = nw::eft::EFT_DRAW_PATH_FLAG_14,
    cDrawPathFlag_15    = nw::eft::EFT_DRAW_PATH_FLAG_15,
    cDrawPathFlag_16    = nw::eft::EFT_DRAW_PATH_FLAG_16,
    cDrawPathFlag_17    = nw::eft::EFT_DRAW_PATH_FLAG_17,
    cDrawPathFlag_18    = nw::eft::EFT_DRAW_PATH_FLAG_18,
    cDrawPathFlag_19    = nw::eft::EFT_DRAW_PATH_FLAG_19,
    cDrawPathFlag_20    = nw::eft::EFT_DRAW_PATH_FLAG_20,
    cDrawPathFlag_21    = nw::eft::EFT_DRAW_PATH_FLAG_21,
    cDrawPathFlag_22    = nw::eft::EFT_DRAW_PATH_FLAG_22,
    cDrawPathFlag_23    = nw::eft::EFT_DRAW_PATH_FLAG_23,
    cDrawPathFlag_24    = nw::eft::EFT_DRAW_PATH_FLAG_24,
    cDrawPathFlag_25    = nw::eft::EFT_DRAW_PATH_FLAG_25,
    cDrawPathFlag_26    = nw::eft::EFT_DRAW_PATH_FLAG_26,
    cDrawPathFlag_27    = nw::eft::EFT_DRAW_PATH_FLAG_27,
    cDrawPathFlag_28    = nw::eft::EFT_DRAW_PATH_FLAG_28,
    cDrawPathFlag_29    = nw::eft::EFT_DRAW_PATH_FLAG_29,
    cDrawPathFlag_30    = nw::eft::EFT_DRAW_PATH_FLAG_30,
    cDrawPathFlag_31    = nw::eft::EFT_DRAW_PATH_FLAG_31
};

//#define cEftInvalidEmitterSetID EFT_INVALID_EMITTER_SET_ID
//#define cEftInvalidEmitterID    EFT_INVALID_EMITTER_ID
//#define cEftInvalidAttribute    EFT_INVALID_ATTRIBUTE
//#define cEftInvalidLocation     EFT_INVALID_LOCATION
//#define cEftInvalidSampler      EFT_INVALID_SAMPLER

enum UserShaderCallBackID
{
    cUserShaderCallBackID_None  = nw::eft::EFT_USER_SHADER_CALLBACK_NONE,
    cUserShaderCallBackID_1     = nw::eft::EFT_USER_SHADER_CALLBACK_1,
    cUserShaderCallBackID_2     = nw::eft::EFT_USER_SHADER_CALLBACK_2,
    cUserShaderCallBackID_3     = nw::eft::EFT_USER_SHADER_CALLBACK_3,
    cUserShaderCallBackID_4     = nw::eft::EFT_USER_SHADER_CALLBACK_4,
    cUserShaderCallBackID_5     = nw::eft::EFT_USER_SHADER_CALLBACK_5,
    cUserShaderCallBackID_6     = nw::eft::EFT_USER_SHADER_CALLBACK_6,
    cUserShaderCallBackID_7     = nw::eft::EFT_USER_SHADER_CALLBACK_7,
    cUserShaderCallBackID_8     = nw::eft::EFT_USER_SHADER_CALLBACK_8,
    cUserShaderCallBackID_Max   = nw::eft::EFT_USER_SHADER_CALLBACK_MAX
};

enum TextureSlot
{
    cTextureSlot_0              = nw::eft::EFT_TEXTURE_SLOT_0,
    cTextureSlot_1              = nw::eft::EFT_TEXTURE_SLOT_1,
    cTextureSlot_MaxBin         = nw::eft::EFT_TEXTURE_SLOT_BIN_MAX,
    cTextureSlot_DepthBuffer    = nw::eft::EFT_TEXTURE_SLOT_DEPTH_BUFFER,
    cTextureSlot_FrameBuffer    = nw::eft::EFT_TEXTURE_SLOT_FRAME_BUFFER,
    cTextureSlot_Max            = nw::eft::EFT_TEXTURE_SLOT_MAX
};

enum TextureWrapType
{
    cTextureWrapType_Mirror     = nw::eft::EFT_TEXTURE_WRAP_TYPE_MIRROR,
    cTextureWrapType_Repeat     = nw::eft::EFT_TEXTURE_WRAP_TYPE_REPEAT,
    cTextureWrapType_Clamp      = nw::eft::EFT_TEXTURE_WRAP_TYPE_CLAMP,
    cTextureWrapType_MirrorOnce = nw::eft::EFT_TEXTURE_WRAP_TYPE_MIROOR_ONCE,
    cTextureWrapType_Max        = nw::eft::EFT_TEXTURE_WRAP_TYPE_MAX
};

enum TextureFilterType
{
    cTextureFilterType_Linear   = nw::eft::EFT_TEXTURE_FILTER_TYPE_LINEAR,
    cTextureFilterType_Near     = nw::eft::EFT_TEXTURE_FILTER_TYPE_NEAR
};

enum DrawViewFlag
{
    cDrawViewFlag_0     = nw::eft::EFT_DRAW_VIEW_FLAG_0,
    cDrawViewFlag_1     = nw::eft::EFT_DRAW_VIEW_FLAG_1,
    cDrawViewFlag_2     = nw::eft::EFT_DRAW_VIEW_FLAG_2,
    cDrawViewFlag_3     = nw::eft::EFT_DRAW_VIEW_FLAG_3,
    cDrawViewFlag_4     = nw::eft::EFT_DRAW_VIEW_FLAG_4,
    cDrawViewFlag_5     = nw::eft::EFT_DRAW_VIEW_FLAG_5,
    cDrawViewFlag_All   = nw::eft::EFT_DRAW_VIEW_FLAG_ALL,
    cDrawViewFlag_None  = nw::eft::EFT_DRAW_VIEW_FLAG_NONE
};

enum DisplaySideType
{
    cDisplaySideType_Both   = nw::eft::EFT_DISPLAYSIDETYPE_BOTH,
    cDisplaySideType_Front  = nw::eft::EFT_DISPLAYSIDETYPE_FRONT,
    cDisplaySideType_Back   = nw::eft::EFT_DISPLAYSIDETYPE_BACK
};

enum TextureAddressing
{
    cTextureAddressing_Normal       = nw::eft::EFT_TEX_ADDRESSING_NORMAL,
    cTextureAddressing_Mirror_U2    = nw::eft::EFT_TEX_ADDRESSING_MIRROR_U2,
    cTextureAddressing_Mirror_V2    = nw::eft::EFT_TEX_ADDRESSING_MIRROR_V2,
    cTextureAddressing_Mirror_U2_V2 = nw::eft::EFT_TEX_ADDRESSING_MIRROR_U2_V2,
    cTextureAddressing_Max          = nw::eft::EFT_TEX_ADDRESSING_MAX
};

enum EmitterFlg
{
    cEmitterFlg_ColorRandom             = nw::eft::EFT_EMITTER_FLAG_COLOR_RANDOM,
    cEmitterFlg_DispEmitterMatrix       = nw::eft::EFT_EMITTER_FLAG_DISP_EMITTER_MATRIX,
    cEmitterFlg_DispVolume              = nw::eft::EFT_EMITTER_FLAG_DISP_VOLUME,
    cEmitterFlg_DispFieldMagnet         = nw::eft::EFT_EMITTER_FLAG_DISP_FIELD_MAGNET,
    cEmitterFlg_DispFieldSpin           = nw::eft::EFT_EMITTER_FLAG_DISP_FIELD_SPIN,
    cEmitterFlg_DispFieldCollision      = nw::eft::EFT_EMITTER_FLAG_DISP_FIELD_COLLISION,
    cEmitterFlg_DispFieldConvergence    = nw::eft::EFT_EMITTER_FLAG_DISP_FIELD_CONVERGENCE,
    cEmitterFlg_EnableFog               = nw::eft::EFT_EMITTER_FLAG_ENABLE_FOG,
    cEmitterFlg_EnableSoftEdge          = nw::eft::EFT_EMITTER_FLAG_ENABLE_SOFTEDGE,
    cEmitterFlg_EnableSortParticle      = nw::eft::EFT_EMITTER_FLAG_ENABLE_SORTPARTICLE,
    cEmitterFlg_ReverseOrderParticle    = nw::eft::EFT_EMITTER_FLAG_REVERSE_ORDER_PARTICLE,

    cEmitterFlg_Texture0ColorOne        = nw::eft::EFT_EMITTER_FLAG_TEXTURE0_COLOR_ONE,
    cEmitterFlg_Texture1ColorOne        = nw::eft::EFT_EMITTER_FLAG_TEXTURE1_COLOR_ONE,
    cEmitterFlg_PrimitiveColorOne       = nw::eft::EFT_EMITTER_FLAG_PRIMITIVE_COLOR_ONE,
    cEmitterFlg_Texture0AlphaOne        = nw::eft::EFT_EMITTER_FLAG_TEXTURE0_ALPHA_ONE,
    cEmitterFlg_Texture1AlphaOne        = nw::eft::EFT_EMITTER_FLAG_TEXTURE1_ALPHA_ONE,
    cEmitterFlg_PrimitiveAlphaOne       = nw::eft::EFT_EMITTER_FLAG_PRIMITIVE_ALPHA_ONE,

    cEmitterFlg_DispFieldMask           = nw::eft::EFT_EMITTER_FLAG_DISP_FIELD_MASK,

    cEmitterFlg_All                     = nw::eft::EFT_EMITTER_FLAG_ALL
};

enum BehaviorFlg
{
    cBehaviorFlg_AirRegist          = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_AIR_REGIST,
    cBehaviorFlg_Gravity            = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_GRAVITY,
    cBehaviorFlg_Rotate             = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_ROTATE,
    cBehaviorFlg_RotRegist          = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_ROT_REGIST,
    cBehaviorFlg_WldPosDif          = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_WLD_POSDIF,
    cBehaviorFlg_LclPosDif          = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_LCL_POSDIF,
    cBehaviorFlg_ScaleAnim          = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_SCALE_ANIM,
    cBehaviorFlg_AlphaAnim          = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_ALPHA_ANIM,
    cBehaviorFlg_Color0Anim         = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_COLOR0_ANIM,
    cBehaviorFlg_Color1Anim         = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_COLOR1_ANIM,
    cBehaviorFlg_Texture0UvAnim     = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_TEXTURE0_UV_ANIM,
    cBehaviorFlg_Texture1UvAnim     = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_TEXTURE1_UV_ANIM,
    cBehaviorFlg_Texture0PtnAnim    = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_TEXTURE0_PTN_ANIM,
    cBehaviorFlg_Texture1PtnAnim    = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_TEXTURE1_PTN_ANIM,
    cBehaviorFlg_Texture1Calc       = nw::eft::EFT_EMITTER_BEHAVIOR_FLAG_TEXTURE1_CALC
};

enum ShaderEnableAttrFlg
{
    cShaderEnableAttrFlg_HasScale           = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_SCALE,
    cShaderEnableAttrFlg_HasTexAnim         = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_TEX_ANIM,
    cShaderEnableAttrFlg_HasSubTexAnim      = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_SUB_TEX_ANIM,
    cShaderEnableAttrFlg_HasWorldPos        = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_WORLD_POS,
    cShaderEnableAttrFlg_HasWorldPosDiff    = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_WORLD_POS_DIFF,
    cShaderEnableAttrFlg_HasColor0          = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_COLOR0,
    cShaderEnableAttrFlg_HasColor1          = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_COLOR1,
    cShaderEnableAttrFlg_HasRotate          = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_ROTATE,
    cShaderEnableAttrFlg_HasEmitterMatrix   = nw::eft::EFT_SHADER_ATTRIBUTE_HAS_EMITTER_MATRIX
};

enum VolumeType
{
    cVolumeType_Point               = nw::eft::EFT_VOLUME_TYPE_POINT,
    cVolumeType_Circle              = nw::eft::EFT_VOLUME_TYPE_CIRCLE,
    cVolumeType_CircleSameDivide    = nw::eft::EFT_VOLUME_TYPE_CIRCLE_SAME_DIVIDE,
    cVolumeType_FillCircle          = nw::eft::EFT_VOLUME_TYPE_CIRCLE_FILL,
  //cVolumeType_FillCircleSameDivide,
    cVolumeType_Sphere              = nw::eft::EFT_VOLUME_TYPE_SPHERE,
    cVolumeType_SphereSameDivide    = nw::eft::EFT_VOLUME_TYPE_SPHERE_SAME_DIVIDE,
    cVolumeType_SphereSameDivide64  = nw::eft::EFT_VOLUME_TYPE_SPHERE_SAME_DIVIDE64,
    cVolumeType_FillSphere          = nw::eft::EFT_VOLUME_TYPE_SPHERE_FILL,
  //cVolumeType_FillSphereSameDivide,
    cVolumeType_Cylinder            = nw::eft::EFT_VOLUME_TYPE_CYLINDER,
    cVolumeType_FillCylinder        = nw::eft::EFT_VOLUME_TYPE_CYLINDER_FILL,
    cVolumeType_Box                 = nw::eft::EFT_VOLUME_TYPE_BOX,
    cVolumeType_FillBox             = nw::eft::EFT_VOLUME_TYPE_BOX_FILL,
    cVolumeType_Line                = nw::eft::EFT_VOLUME_TYPE_LINE,
    cVolumeType_LineSameDivide      = nw::eft::EFT_VOLUME_TYPE_LINE_SAME_DIVIDE,
    cVolumeType_Rectangle           = nw::eft::EFT_VOLUME_TYPE_RECTANGLE,

    cVolumeType_Max                 = nw::eft::EFT_VOLUME_TYPE_MAX
};

enum ShaderType
{
    cShaderType_Normal      = nw::eft::EFT_SHADER_TYPE_NORMAL,
    cShaderType_UserDef1    = nw::eft::EFT_SHADER_TYPE_USER_DEF1,
    cShaderType_UserDef2    = nw::eft::EFT_SHADER_TYPE_USER_DEF2,
    cShaderType_Max         = nw::eft::EFT_SHADER_TYPE_MAX
};

enum BlendType
{
    cBlendType_Normal   = nw::eft::EFT_BLEND_TYPE_NORMAL,
    cBlendType_Add      = nw::eft::EFT_BLEND_TYPE_ADD,
    cBlendType_Sub      = nw::eft::EFT_BLEND_TYPE_SUB,
    cBlendType_Screen   = nw::eft::EFT_BLEND_TYPE_SCREEN,
    cBlendType_Mult     = nw::eft::EFT_BLEND_TYPE_MULT,
    cBlendType_None     = nw::eft::EFT_BLEND_TYPE_NONE,
    cBlendType_Max      = nw::eft::EFT_BLEND_TYPE_MAX
};

enum ZBufATestType
{
    cZBufATestType_Normal   = nw::eft::EFT_ZBUFF_ATEST_TYPE_NORMAL,
    cZBufATestType_ZIgnore  = nw::eft::EFT_ZBUFF_ATEST_TYPE_ZIGNORE,
    cZBufATestType_Entity   = nw::eft::EFT_ZBUFF_ATEST_TYPE_ENTITY,
    cZBufATestType_Max      = nw::eft::EFT_ZBUFF_ATEST_TYPE_MAX
};

enum PtclRotType
{
    cRotType_NoWork = nw::eft::EFT_ROT_TYPE_NO_WORK,
    cRotType_RotX   = nw::eft::EFT_ROT_TYPE_ROTX,
    cRotType_RotY   = nw::eft::EFT_ROT_TYPE_ROTY,
    cRotType_RotZ   = nw::eft::EFT_ROT_TYPE_ROTZ,
    cRotType_RotXYZ = nw::eft::EFT_ROT_TYPE_ROTXYZ,
    cRotType_Max    = nw::eft::EFT_ROT_TYPE_MAX
};

enum MeshType
{
    cMeshType_Particle  = nw::eft::EFT_MESH_TYPE_PARTICLE,
    cMeshType_Primitive = nw::eft::EFT_MESH_TYPE_PRIMITIVE,
    cMeshType_Stripe    = nw::eft::EFT_MESH_TYPE_STRIPE
};

enum BillboardType
{
    cBillboardType_Billboard        = nw::eft::EFT_BILLBOARD_TYPE_BILLBOARD,
    cBillboardType_PolygonXY        = nw::eft::EFT_BILLBOARD_TYPE_POLYGON_XY,
    cBillboardType_PolygonXZ        = nw::eft::EFT_BILLBOARD_TYPE_POLYGON_XZ,
    cBillboardType_VelLook          = nw::eft::EFT_BILLBOARD_TYPE_VEL_LOOK,
    cBillboardType_VelLookPolygon   = nw::eft::EFT_BILLBOARD_TYPE_VEL_LOOK_POLYGON,
    cBillboardType_Stripe           = nw::eft::EFT_BILLBOARD_TYPE_STRIPE,
    cBillboardType_ComplexStripe    = nw::eft::EFT_BILLBOARD_TYPE_COMPLEX_STRIPE,
    cBillboardType_Primitive        = nw::eft::EFT_BILLBOARD_TYPE_PRIMITIVE,
    cBillboardType_YBillboard       = nw::eft::EFT_BILLBOARD_TYPE_Y_BILLBOARD,

    cBillboardType_MAX              = nw::eft::EFT_BILLBOARD_TYPE_MAX
};

enum CombinerType
{
    cCombinerType_Color                 = nw::eft::EFT_COMBINER_TYPE_COLOR,
    cCombinerType_Texture               = nw::eft::EFT_COMBINER_TYPE_TEXTURE,
    cCombinerType_TextureInterpolate    = nw::eft::EFT_COMBINER_TYPE_TEXTURE_INTERPOLATE,
    cCombinerType_TextureAdd            = nw::eft::EFT_COMBINER_TYPE_TEXTURE_ADD,
    cCombinerType_Max                   = nw::eft::EFT_COMBINER_TYPE
};

enum AlphaCombinerType
{
    cAlphaCombinerType_Mod  = nw::eft::EFT_ALPHA_COMBINER_TYPE_MOD,
    cAlphaCombinerType_Sub  = nw::eft::EFT_ALPHA_COMBINER_TYPE_SUB,
    cAlphaCombinerType_ModR = nw::eft::EFT_ALPHA_COMBINER_TYPE_MOD_R,
    cAlphaCombinerType_SubR = nw::eft::EFT_ALPHA_COMBINER_TYPE_SUB_R,
    cAlphaCombinerType_Max  = nw::eft::EFT_ALPHA_COMBINER_TYPE
};

enum ColorBlendType
{
    cColorBlendType_Mod = nw::eft::EFT_COLOR_BLEND_TYPE_MOD,
    cColorBlendType_Add = nw::eft::EFT_COLOR_BLEND_TYPE_ADD,
    cColorBlendType_Sub = nw::eft::EFT_COLOR_BLEND_TYPE_SUB
};

enum ColorCalcType
{
    cColorCalcType_None                 = nw::eft::EFT_COLOR_CALC_TYPE_NONE,
    cColorCalcType_Random               = nw::eft::EFT_COLOR_CALC_TYPE_RANDOM,
    cColorCalcType_RandomLinear3Color   = nw::eft::EFT_COLOR_CALC_TYPE_RANDOM_LINEAR3COLOR
};

enum ColorKind
{
    cColorKind_0    = nw::eft::EFT_COLOR_KIND_0,
    cColorKind_1    = nw::eft::EFT_COLOR_KIND_1,
    cColorKind_Max  = nw::eft::EFT_COLOR_KIND_MAX
};

enum TextureFormat
{
    cTextureFormat_None         = nw::eft::EFT_TEXTURE_FORMAT_NONE,
    cTextureFormat_24Bit_Color  = nw::eft::EFT_TEXTURE_FORMAT_24BIT_COLOR,
    cTextureFormat_32Bit_Color  = nw::eft::EFT_TEXTURE_FORMAT_32BIT_COLOR,
    cTextureFormat_uNorm_BC1    = nw::eft::EFT_TEXTURE_FORMAT_UNORM_BC1,
    cTextureFormat_SRGB_BC1     = nw::eft::EFT_TEXTURE_FORMAT_SRGB_BC1,
    cTextureFormat_uNorm_BC2    = nw::eft::EFT_TEXTURE_FORMAT_UNORM_BC2,
    cTextureFormat_SRGB_BC2     = nw::eft::EFT_TEXTURE_FORMAT_SRGB_BC2,
    cTextureFormat_uNorm_BC3    = nw::eft::EFT_TEXTURE_FORMAT_UNORM_BC3,
    cTextureFormat_SRGB_BC3     = nw::eft::EFT_TEXTURE_FORMAT_SRGB_BC3,
    cTextureFormat_uNorm_BC4    = nw::eft::EFT_TEXTURE_FORMAT_UNORM_BC4,
    cTextureFormat_sNorm_BC4    = nw::eft::EFT_TEXTURE_FORMAT_SNORM_BC4,
    cTextureFormat_uNorm_BC5    = nw::eft::EFT_TEXTURE_FORMAT_UNORM_BC5,
    cTextureFormat_sNorm_BC5    = nw::eft::EFT_TEXTURE_FORMAT_SNORM_BC5,
    cTextureFormat_uNnorm_8     = nw::eft::EFT_TEXTURE_FORMAT_UNORM_8,
    cTextureFormat_uNorm_8_8    = nw::eft::EFT_TEXTURE_FORMAT_UNORM_8_8,
    cTextureFormat_SRGB_8_8_8_8 = nw::eft::EFT_TEXTURE_FORMAT_SRGB_8_8_8_8
};

enum UvShiftAnimMode
{
    cUvShiftAnimMode_None   = nw::eft::EFT_UV_SHIFT_ANIM_NONE,
    cUvShiftAnimMode_Scroll = nw::eft::EFT_UV_SHIFT_ANIM_SCROLL,
    cUvShiftAnimMode_Scale  = nw::eft::EFT_UV_SHIFT_ANIM_SCALE,
    cUvShiftAnimMode_Rot    = nw::eft::EFT_UV_SHIFT_ANIM_ROT,
    cUvShiftAnimMode_All    = nw::eft::EFT_UV_SHIFT_ANIM_ALL
};

enum ChildFlg
{
    cChildFlg_Enable                = nw::eft::EFT_CHILD_FLAG_ENABLE,
    cChildFlg_Color0Inherit         = nw::eft::EFT_CHILD_FLAG_COLOR0_INHERIT,
    cChildFlg_AlphaInherit          = nw::eft::EFT_CHILD_FLAG_ALPHA_INHERIT,
    cChildFlg_ScaleInherit          = nw::eft::EFT_CHILD_FLAG_SCALE_INHERIT,
    cChildFlg_RotateInherit         = nw::eft::EFT_CHILD_FLAG_ROTATE_INHERIT,
    cChildFlg_VelInherit            = nw::eft::EFT_CHILD_FLAG_VEL_INHERIT,
    cChildFlg_EmitterFollow         = nw::eft::EFT_CHILD_FLAG_EMITTER_FOLLOW,
    cChildFlg_DispParent            = nw::eft::EFT_CHILD_FLAG_DISP_PARENT,
    cChildFlg_WorldField            = nw::eft::EFT_CHILD_FLAG_WORLD_FIELD,
    cChildFlg_IsPolygon             = nw::eft::EFT_CHILD_FLAG_IS_POLYGON,
    cChildFlg_IsEmitterBillboardMtx = nw::eft::EFT_CHILD_FLAG_IS_EMITTER_BILLBOARD_MTX,
    cChildFlg_ParentField           = nw::eft::EFT_CHILD_FLAG_PARENT_FIELD,
    cChildFlg_PreChildDraw          = nw::eft::EFT_CHILD_FLAG_PRE_CHILD_DRAW,
    cChildFlg_IsTexturePatAnim      = nw::eft::EFT_CHILD_FLAG_IS_TEXTURE_PAT_ANIM,
    cChildFlg_IsTexturePatAnimRand  = nw::eft::EFT_CHILD_FLAG_IS_TEXTURE_PAT_ANIM_RAND,
    cChildFlg_Color1Inherit         = nw::eft::EFT_CHILD_FLAG_COLOR1_INHERIT,
    cChildFlg_ColorScaleInherit     = nw::eft::EFT_CHILD_FLAG_COLOR_SCALE_INHERIT,

    cChildFlg_TextureColorOne       = nw::eft::EFT_CHILD_FLAG_TEXTURE_COLOR_ONE,
    cChildFlg_PrimitiveColorOne     = nw::eft::EFT_CHILD_FLAG_PRIMITIVE_COLOR_ONE,
    cChildFlg_TextureAlphaOne       = nw::eft::EFT_CHILD_FLAG_TEXTURE_ALPHA_ONE,
    cChildFlg_PrimitiveAlphaOne     = nw::eft::EFT_CHILD_FLAG_PRIMITIVE_ALPHA_ONE
};

enum FieldType
{
    cFieldType_Random       = nw::eft::EFT_FIELD_TYPE_RANDOM,
    cFieldType_Magnet       = nw::eft::EFT_FIELD_TYPE_MAGNET,
    cFieldType_Spin         = nw::eft::EFT_FIELD_TYPE_SPIN,
    cFieldType_Collision    = nw::eft::EFT_FIELD_TYPE_COLLISION,
    cFieldType_Convergence  = nw::eft::EFT_FIELD_TYPE_CONVERGENCE,
    cFieldType_PosAdd       = nw::eft::EFT_FIELD_TYPE_POSADD,
    cFieldType_Max          = nw::eft::EFT_FIELD_TYPE_MAX
};

enum FieldMask
{
    cFieldMask_Random       = nw::eft::EFT_FIELD_MASK_RANDOM,
    cFieldMask_Magnet       = nw::eft::EFT_FIELD_MASK_MAGNET,
    cFieldMask_Spin         = nw::eft::EFT_FIELD_MASK_SPIN,
    cFieldMask_Collision    = nw::eft::EFT_FIELD_MASK_COLLISION,
    cFieldMask_Convergence  = nw::eft::EFT_FIELD_MASK_CONVERGENCE,
    cFieldMask_PosAdd       = nw::eft::EFT_FIELD_MASK_POSADD
};

enum FluctuationFlg
{
    cFluctuationFlg_Enable      = nw::eft::EFT_FLUCTUATION_FALG_ENABLE,
    cFluctuationFlg_ApplyAlpha  = nw::eft::EFT_FLUCTUATION_FALG_APPLY_ALPHA,
    cFluctuationFlg_ApplyScale  = nw::eft::EFT_FLUCTUATION_FALG_APPLY_SCLAE
};

enum
{
    cFluctuationTable_Num   = nw::eft::EFT_FLUCTUATION_TABLE_NUM,
    cFluctuationTable_Mask  = nw::eft::EFT_FLUCTUATION_TABLE_MASK
};

enum FieldMagnetFlg
{
    cFieldMagnetFlg_X   = nw::eft::EFT_MAGNET_FLAG_X,
    cFieldMagnetFlg_Y   = nw::eft::EFT_MAGNET_FLAG_Y,
    cFieldMagnetFlg_Z   = nw::eft::EFT_MAGNET_FLAG_Z
};

enum FieldCollisionReaction
{
    cFieldCollisionReaction_Cesser      = nw::eft::EFT_FIELD_COLLISION_REACTION_CESSER,
    cFieldCollisionReaction_Reflection  = nw::eft::EFT_FIELD_COLLISION_REACTION_REFLECTION
};

enum StripeFlg
{
    cStripeFlg_EmitterCoord = nw::eft::EFT_STRIPE_FLAG_EMITTER_COORD
};

enum StripeType
{
    cStripeType_Billboard       = nw::eft::EFT_STRIPE_TYPE_BILLBOARD,
    cStripeType_EmitterMatrix   = nw::eft::EFT_STRIPE_TYPE_EMITTER_MATRIX,
    cStripeType_EmitterUpDown   = nw::eft::EFT_STRIPE_TYPE_EMITTER_UP_DOWN,
    cStripeType_Max             = nw::eft::EFT_STRIPE_TYPE_MAX
};

enum StripeOption
{
    cStripeOption_Normal    = nw::eft::EFT_STRIPE_OPTION_TYPE_NORMAL,
    cStripeOption_Cross     = nw::eft::EFT_STRIPE_OPTION_TYPE_CROSS,
    cStripeOption_Max       = nw::eft::EFT_STRIPE_OPTION_TYPE_MAX
};

enum StripeConnectOption
{
    cStripeConnectOption_Normal         = nw::eft::EFT_STRIPE_CONNECT_OPTION_NORMAL,
    cStripeConnectOption_Head           = nw::eft::EFT_STRIPE_CONNECT_OPTION_HEAD,
    cStripeConnectOption_Emitter        = nw::eft::EFT_STRIPE_CONNECT_OPTION_EMITTER,
    cStripeConnectOption_EmitterUnbind  = nw::eft::EFT_STRIPE_CONNECT_OPTION_EMITTER_UNBIND
};

enum StripeTexCoordOption
{
    cStripeTexCoordOption_Full      = nw::eft::EFT_STRIPE_TEXCOORD_OPTION_TYPE_FULL,
    cStripeTexCoordOption_Division  = nw::eft::EFT_STRIPE_TEXCOORD_OPTION_TYPE_DIVISION,
    cStripeTexCoordOption_Max       = nw::eft::EFT_STRIPE_TEXCOORD_OPTION_TYPE_MAX
};

enum UserDataFlag
{
    cUserDataFlag_0     = nw::eft::EFT_USER_DATA_FLAG_0,
    cUserDataFlag_1     = nw::eft::EFT_USER_DATA_FLAG_1,
    cUserDataFlag_2     = nw::eft::EFT_USER_DATA_FLAG_2,
    cUserDataFlag_3     = nw::eft::EFT_USER_DATA_FLAG_3,
    cUserDataFlag_4     = nw::eft::EFT_USER_DATA_FLAG_4,
    cUserDataFlag_5     = nw::eft::EFT_USER_DATA_FLAG_5,
    cUserDataFlag_6     = nw::eft::EFT_USER_DATA_FLAG_6,
    cUserDataFlag_7     = nw::eft::EFT_USER_DATA_FLAG_7,
    cUserDataFlag_8     = nw::eft::EFT_USER_DATA_FLAG_8,
    cUserDataFlag_9     = nw::eft::EFT_USER_DATA_FLAG_9,
    cUserDataFlag_10    = nw::eft::EFT_USER_DATA_FLAG_10,
    cUserDataFlag_11    = nw::eft::EFT_USER_DATA_FLAG_11,
    cUserDataFlag_12    = nw::eft::EFT_USER_DATA_FLAG_12,
    cUserDataFlag_13    = nw::eft::EFT_USER_DATA_FLAG_13,
    cUserDataFlag_14    = nw::eft::EFT_USER_DATA_FLAG_14,
    cUserDataFlag_15    = nw::eft::EFT_USER_DATA_FLAG_15
};

enum UserDataParamIdx
{
    cUserDataParam_0    = nw::eft::EFT_USER_DATA_PARAM_0,
    cUserDataParam_1    = nw::eft::EFT_USER_DATA_PARAM_1,
    cUserDataParam_2    = nw::eft::EFT_USER_DATA_PARAM_2,
    cUserDataParam_3    = nw::eft::EFT_USER_DATA_PARAM_3,
    cUserDataParam_4    = nw::eft::EFT_USER_DATA_PARAM_4,
    cUserDataParam_5    = nw::eft::EFT_USER_DATA_PARAM_5,
    cUserDataParam_6    = nw::eft::EFT_USER_DATA_PARAM_6,
    cUserDataParam_7    = nw::eft::EFT_USER_DATA_PARAM_7,
    cUserDataParam_Max  = nw::eft::EFT_USER_DATA_PARAM_MAX
};

struct VertexTextureLocation : private nw::eft::VertexTextureLocation
{
    using nw::eft::VertexTextureLocation::loc;

    bool isValid() const
    {
        return IsValid();
    }
};
static_assert(sizeof(VertexTextureLocation) == sizeof(nw::eft::VertexTextureLocation));

struct FragmentTextureLocation : private nw::eft::FragmentTextureLocation
{
    using nw::eft::FragmentTextureLocation::loc;

    bool isValid() const
    {
        return IsValid();
    }
};
static_assert(sizeof(FragmentTextureLocation) == sizeof(nw::eft::FragmentTextureLocation));

typedef nw::eft::PtclAttributeBuffer PtclAttributeBuffer;

typedef nw::eft::PrimitiveImageInformation PrimitiveImageInformation;

typedef nw::eft::PrimitiveTableInfo PrimitiveTableInfo;

typedef nw::eft::HeaderData HeaderData;

typedef nw::eft::EmitterSetData EmitterSetData;

typedef nw::eft::EmitterTblData EmitterTblData;

typedef nw::eft::UserShaderParam UserShaderParam;

typedef nw::eft::TextureRes TextureRes;

typedef nw::eft::AnimKeyTable AnimKeyTable;

typedef nw::eft::PrimitiveFigure PrimitiveFigure;

typedef nw::eft::CommonEmitterData CommonEmitterData;

typedef nw::eft::TextureEmitterData TextureEmitterData;

typedef nw::eft::SimpleEmitterData SimpleEmitterData;

typedef nw::eft::ComplexEmitterData ComplexEmitterData;

typedef nw::eft::ChildData ChildData;

typedef nw::eft::FieldRandomData FieldRandomData;

typedef nw::eft::FieldMagnetData FieldMagnetData;

typedef nw::eft::FieldSpinData FieldSpinData;

typedef nw::eft::FieldCollisionData FieldCollisionData;

typedef nw::eft::FieldConvergenceData FieldConvergenceData;

typedef nw::eft::FieldPosAddData FieldPosAddData;

typedef nw::eft::FluctuationData FluctuationData;

typedef nw::eft::StripeData StripeData;

} }

#endif // SEAD_PTCL_DATA_H_
