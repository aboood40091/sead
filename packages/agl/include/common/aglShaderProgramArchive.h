#pragma once

#include <common/aglResShaderArchive.h>
#include <common/aglResShaderSource.h>
#include <common/aglShaderCompileInfo.h>
#include <common/aglShaderProgram.h>
#include <container/seadBuffer.h>
#include <container/seadSafeArray.h>
#include <heap/seadDisposer.h>
#include <hostio/seadHostIODummy.h>
#include <prim/seadBitFlag.h>
#include <prim/seadSafeString.h>

namespace agl {

class ShaderProgramArchive : public sead::IDisposer, public sead::hostio::Node
{
    class ShaderProgramEx;
    class ShaderSource;

    class ShaderCompileInfoEx
    {
    public:
        ShaderCompileInfoEx()
            : mCompileInfo()
        {
        }

    private:
        ShaderCompileInfo mCompileInfo;
        ShaderSource* mSource;

        friend class ShaderProgramArchive;
    };
    static_assert(sizeof(ShaderCompileInfoEx) == 0x58, "agl::ShaderProgramArchive::ShaderCompileInfoEx size mismatch");

    class ShaderProgramEx : public sead::hostio::Node
    {
    public:
        ShaderProgramEx();
        virtual ~ShaderProgramEx();

        void initialize(ShaderProgramArchive* archive, s32 index, ResShaderProgram res, sead::Heap* heap);
        void updateRawText();
        void updateAnalyze();

    private:
        s32 mIndex;
        ShaderProgramArchive* mpArchive;
        sead::UnsafeArray<ShaderCompileInfoEx, cShaderType_Num> mCompileInfoEx;
        sead::Buffer<u32> _110; // No idea buffer of what
        s32 mVariationIndex;
        sead::BitFlag8 mFlag;

        friend class ShaderProgramArchive;
    };
    static_assert(sizeof(ShaderProgramEx) == 0x124, "agl::ShaderProgramArchive::ShaderProgramEx size mismatch");

    class ShaderSource : public sead::IDisposer, public sead::hostio::Node
    {
    public:
        ShaderSource();
        virtual ~ShaderSource();

        void initialize(ShaderProgramArchive* archive, s32 index, ResShaderSource res, bool is_used, sead::Heap* heap);
        void expand();

        const char* getName() const
        {
            return mRes.getName();
        }

    private:
        s32 mIndex;
        sead::BitFlag32 mFlag;
        ShaderProgramArchive* mpArchive;
        ResShaderSource mRes;
        sead::HeapSafeString* mText;
        sead::HeapSafeString* mRawText;
        sead::Buffer<bool> mUsedInSource;

        friend class ShaderProgramArchive;
    };
    static_assert(sizeof(ShaderSource) == 0x30, "agl::ShaderProgramArchive::ShaderSource size mismatch");

public:
    ShaderProgramArchive();
    virtual ~ShaderProgramArchive();

    void destroy();

    bool isValid() const
    {
        return mResBinary.isValid() || mResText.isValid();
    }

    sead::SafeString getName() const
    {
        if (mResBinary.isValid())
            return mResBinary.getName();
        else
            return mResText.getName();
    }

    s32 getShaderProgramNum() const
    {
        return mProgram.size();
    }

    ShaderProgram& getShaderProgram(s32 idx)
    {
        return mProgram[idx];
    }

    const ShaderProgram& getShaderProgram(s32 idx) const
    {
        return mProgram[idx];
    }

    void createWithOption(ResBinaryShaderArchive res_binary_archive, ResShaderArchive res_archive, u32 flag, sead::Heap* heap);
    bool setUp();

    s32 searchShaderProgramIndex(const sead::SafeString& name) const;

    const ShaderProgram* searchShaderProgram(const sead::SafeString& name) const
    {
        s32 index = searchShaderProgramIndex(name);
        if (index >= 0 && index < mProgram.size())
            return mProgram.unsafeGet(index);

        return nullptr;
    }

    const ShaderProgram* searchShaderProgram(s32 index) const
    {
        return mProgram.get(index);
    }

    void updateCompileInfo();

private:
    void destroyResFile_();
    void setResShaderArchive_(ResShaderArchive res_archive, sead::Heap* heap);
    bool setUp_(bool);

private:
    ResBinaryShaderArchive mResBinary;
    ResShaderArchive mResText;
    sead::Buffer<ShaderProgram> mProgram;
    u32 _20;
    u32 _24;
    u16 _28;
    sead::BitFlag16 mFlag;
    sead::Buffer<ShaderProgramEx> mProgramEx;
    sead::Buffer<ShaderSource> mSource;
    sead::Buffer<const char*> mSourceText;
    sead::Buffer<const char*> mSourceName;

    friend class ShaderSource;
    friend class ShaderProgramEx;
};
static_assert(sizeof(ShaderProgramArchive) == 0x4C, "agl::ShaderProgramArchive size mismatch");

}
