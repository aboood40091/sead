#pragma once

#include <common/aglTextureSampler.h>
#include <container/seadBuffer.h>
#include <container/seadPtrArray.h>
#include <container/seadSafeArray.h>
#include <environment/aglEnvObjMgr.h>
#include <lighting/aglLightMap.h>
#include <utility/aglDebugTexturePage.h>
#include <utility/aglParameterObj.h>
#include <utility/aglParameterCurve.h>
#include <utility/aglParameterIO.h>

namespace agl { namespace lght {

class LightMapMgr : public utl::IParameterIO, public sead::hostio::Node
{
    static const s32 cLUTValueNum = 128;

    enum LUTType
    {
        cLUT_Lambert = 0,
        cLUT_Half_Lambert,
        cLUT_Hemisphere,
        cLUT_UserData_0,
        cLUT_UserData_Num = 29,
        cLUT_Num = cLUT_UserData_0 + cLUT_UserData_Num
    };
    static_assert(cLUT_Num == 32);

    class CPUTexture
    {
    public:
        CPUTexture();
        ~CPUTexture();

    private:
        void* mPtr;
        u32 mSize;
        TextureData mTextureData;
        TextureSampler mTextureSampler;

        friend class LightMap;
        friend class LightMapMgr;
    };
    static_assert(sizeof(CPUTexture) == 0x244);

    class LUT   // Real name unknown
    {
    private:
        utl::Parameter< sead::FixedSafeString<32> > mName;
        utl::ParameterCurve1f mIntensity;

        friend class LightMap;
        friend class LightMapMgr;
    };
    static_assert(sizeof(LUT) == 0xD8);

    typedef LightMap::GraphicsContext GraphicsContext;

public:
    class CreateArg
    {
    public:
        CreateArg();

        virtual ~CreateArg()
        {
        }

        void setViewMax(s32 view_max)
        {
            // SEAD_ASSERT(0 < view_max);
            mViewMax = view_max;
        }

        s32 getViewMax() const
        {
            return mViewMax;
        }

        void setSizeSphere(s32 size_sphere)
        {
            // SEAD_ASSERT(0 < size_sphere);
            mSizeSphere = size_sphere;
        }

        s32 getSizeSphere() const
        {
            return mSizeSphere;
        }

        void setSizeCube(s32 size_cube)
        {
            // SEAD_ASSERT(0 < size_cube);
            mSizeCube = size_cube;
        }

        s32 getSizeCube() const
        {
            return mSizeCube;
        }

        void setNum(s32 num)
        {
            // SEAD_ASSERT(0 < num);
            mNum = num;
        }

        s32 getNum() const
        {
            return mNum;
        }

    protected:
        s32 mViewMax;
        s32 mSizeSphere;
        s32 mSizeCube;
        s32 mNum;
        u32 _10;
    };
    static_assert(sizeof(CreateArg) == 0x18);

public:
    LightMapMgr();
    virtual ~LightMapMgr();

    void initialize(const env::EnvObjBuffer::AllocateArg& alloc_arg, const CreateArg& create_arg, env::EnvObjMgr* p_env_mgr, sead::Heap* heap = nullptr);

    void update();

    void updateGPU(bool invalidate_gpu = false) const;
    void updateViewGPU(s32 view_index, bool ignore_mapping_type, const sead::Matrix34f& view_mtx, bool invalidate_gpu = false) const;

    ShaderMode draw(s32 view_index, bool ignore_mapping_type, ShaderMode mode) const;

    LightMap* getLightMap(s32 index)
    {
        if (index == -1)
            return nullptr;

        return &(mLightMap[index]);
    }

    const LightMap* getLightMap(s32 index) const
    {
        if (index == -1)
            return nullptr;

        return &(mLightMap[index]);
    }

    s32 searchIndex(const sead::SafeString& name) const;

protected:
    virtual void postRead_();

private:
    void updateLUT_();
    void createNormalMap_(s32 size_sphere, s32 size_cube);
    void constructList_();
    void constructMemory_();

private:
    sead::Buffer<LightMap> mLightMap;
    sead::PtrArray<LightMap> mLightMapCalc;
    CPUTexture mCPUTextureLUT;
    CPUTexture mCPUTextureSphereNormal;
    CPUTexture mCPUTextureCubeNormal;
    sead::UnsafeArray<LUT, cLUT_Num> mLUT;
    GraphicsContext mGraphicsContext;
    utl::IParameterObj mConfig;
    utl::Parameter< sead::FixedSafeString<32> > mType;
    utl::IParameterObj mLUTParam;
    f32 _2b84;
    u32 _2b88; // Unused
    utl::DebugTexturePage mDebugTexturePage;
    sead::Heap* mpLightMapHeap;

    friend class LightMap;
};
static_assert(sizeof(LightMapMgr) == 0x2D6C, "agl::lght::LightMapMgr size mismatch");

} }
