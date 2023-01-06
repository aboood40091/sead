#pragma once

#include <container/seadBuffer.h>
#include <container/seadPtrArray.h>
#include <heap/seadHeap.h>
#include <prim/seadNamable.h>

namespace agl {

enum ShaderType;

class ShaderCompileInfo : public sead::INamable
{
public:
    enum Target
    {
        cTarget_GL,
        cTarget_GX2,
        cTarget_Num
    };

public:
    ShaderCompileInfo();
    virtual ~ShaderCompileInfo();

    void create(s32 num_macro, s32 num_variation, sead::Heap* heap);

    void clearMacro()
    {
        mMacroName.clear();
        mMacroValue.clear();
    }

    void pushBackMacro(const char* name, const char* value)
    {
        mMacroName.pushBack(name);
        mMacroValue.pushBack(value);
    }

    void clearVariation();
    void pushBackVariation(const char* name, const char* value);

    void setSourceText(const sead::SafeString* text)
    {
        mSourceText = text;
    }

    const sead::SafeString* getSourceText() const
    {
        return mSourceText;
    }

    void setRawText(sead::BufferedSafeString* text)
    {
        mRawText = text;
    }

    sead::BufferedSafeString* getRawText() const
    {
        return mRawText;
    }

    void calcCompileSource(ShaderType type, sead::BufferedSafeString* p_buffer, Target target, bool) const;

    void destroy();

private:
    const sead::SafeString* mSourceText;
    sead::BufferedSafeString* mRawText;
    sead::PtrArray<const char> mMacroName;
    sead::PtrArray<const char> mMacroValue;
    sead::PtrArray<const char> mVariationName;
    sead::PtrArray<const char> mVariationValue;
    sead::Buffer<u8> _40; // No idea buffer of what
    sead::Buffer<u8> _48; // ^^
};
static_assert(sizeof(ShaderCompileInfo) == 0x54, "agl::ShaderCompileInfo size mismatch");

}
