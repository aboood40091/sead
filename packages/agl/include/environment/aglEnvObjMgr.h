#pragma once

#include <environment/aglEnvObjSet.h>
#include <math/seadMatrix.h>
#include <utility/aglNamedObjMgr.h>
#include <utility/aglParameterIO.h>
#include <utility/aglResParameter.h>

namespace agl {

class RenderBuffer;

namespace env {

class EnvObjMgr : public EnvObjBuffer, public utl::INamedObjMgr, public utl::IParameterIO
{
    struct ViewInfo
    {
        sead::Matrix34f view_mtx;
        sead::Matrix44f proj_mtx;
        f32 near;                       // Maybe
        f32 far;                        // ^^
        u32 _78;
        sead::BitFlag32 _7c;            // ^^
        RenderBuffer* p_render_buffer;
        f32 _84;
    };
    static_assert(sizeof(ViewInfo) == 0x88);

public:
    class InitArg : public AllocateArg
    {
    public:
        InitArg();

        void setGroupMax(s32 max)
        {
            mGroupMax = max;
        }

        s32 getGroupMax() const
        {
            return mGroupMax;
        }

        void setViewMax(s32 max)
        {
            mViewMax = max;
        }

        s32 getViewMax() const
        {
            return mViewMax;
        }

    private:
        s32 mGroupMax;
        s32 mViewMax;
    };
    static_assert(sizeof(InitArg) == 0x110);

    class TypeNode : public utl::IParameterList, public sead::hostio::Node
    {
    public:
        TypeNode();

        virtual ~TypeNode()
        {
        }

        void initialize(s32 type, EnvObjMgr* p_mgr, sead::Heap* heap);

    protected:
        s32 mType;
        EnvObjMgr* mpMgr;
    };
    static_assert(sizeof(TypeNode) == 0x88);

public:
    EnvObjMgr();
    virtual ~EnvObjMgr();

    virtual bool save(const sead::SafeString& path, u32) const
    {
        return saveImpl_(path, 1, -1);
    }

    virtual void applyResParameterArchive(utl::ResParameterArchive arc)
    {
        applyResource_(arc, arc, 1.0f);
    }

    virtual void listenPropertyEventFromGroup(GroupEventType, Group*);

    virtual const sead::SafeString& getSaveFilePath() const
    {
        return mSaveFilePath;
    }

    virtual const sead::SafeString& getNamedObjName(s32 index, s32 type) const
    {
        return getEnvObj(type, index)->getEnvObjName();
    }

    virtual s32 getNamedObjNum(s32 type) const
    {
        return getEnvObjMaxNum(type);
    }

public:
    void initialize(const InitArg& arg, sead::Heap* heap = nullptr);

    void applyResource(utl::ResParameterArchive arc)
    {
        applyResource_(arc, arc, 1.0f);
    }

    void applyResourceLerp(utl::ResParameterArchive arc_a, utl::ResParameterArchive arc_b, f32 t)
    {
        applyResource_(arc_a, arc_b, t);
    }

    void reconstruct()
    {
        mFlag.setBit(0);
        mFlag.setBit(1);
    }

    void update();

    void updateView(const sead::Matrix34f& view_mtx, const sead::Matrix44f& proj_mtx, s32 view_index);

    void constructList();

    const EnvObjSet& getEnvObjSet() const
    {
        return mEnvObjSet;
    }

private:
    bool saveImpl_(const sead::SafeString&, u32, s32) const;

    void applyResource_(utl::ResParameterArchive arc_a, utl::ResParameterArchive arc_b, f32 t);

    ShaderMode drawFog_(s32 view_index, const EnvObj& obj, f32 start, f32 end, const sead::Vector3f& direction, const sead::Color4f& color, ShaderMode mode) const;

private:
    sead::BitFlag32 mFlag;
    sead::PtrArray<EnvObj> mEnvObjPtrArray;
    sead::Buffer<ViewInfo> mViewInfo;
    f32 _24c;
    EnvObjSet mEnvObjSet;
    sead::UnsafeArray<u32, 4> _42c;
    u32 mConstructListType; // 0 == By Name, 1 == By EnvObjSet, 2 == By Group
    sead::Buffer<TypeNode> mTypeNode;
    sead::PtrArray<void> _448; // Unused
    s32 mConstructListGroupIndex;
    s32 mApplyResourceGroupIndex;
    u32 _45c;
    u32 _460;

    friend class EnvObj;
};
static_assert(sizeof(EnvObjMgr) == 0x464, "agl::env::EnvObjMgr size mismatch");

} }
