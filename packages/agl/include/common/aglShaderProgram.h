#pragma once

#include <common/aglDisplayList.h>
#include <common/aglResShaderSymbol.h>
#include <common/aglResShaderVariation.h>
#include <common/aglShader.h>
#include <common/aglShaderLocation.h>
#include <container/seadBuffer.h>
#include <container/seadSafeArray.h>
#include <prim/seadBitFlag.h>
#include <prim/seadNamable.h>

namespace agl {

class ShaderProgram
{
public:
    static const s32 cVariationMacroMax = 1024;
    static const s32 cVariationValueMax = 1024;

    static void changeShaderMode(ShaderMode mode);

public:
    ShaderProgram();
    virtual ~ShaderProgram();

    s32 getVariationID() const
    {
        return mVariationID;
    }

    DisplayList& getDisplayList()
    {
        return mDisplayList;
    }

    const DisplayList& getDisplayList() const
    {
        return mDisplayList;
    }

    const sead::SafeString& getName() const
    {
        return mpSharedData->getName();
    }

    const ResShaderSymbolArray& getResShaderSymbolArray(ShaderSymbolType type) const
    {
        return mpSharedData->mResShaderSymbolArray[type];
    }

    void setResShaderSymbolArray(ShaderSymbolType type, const ResShaderSymbolArray& array)
    {
        mpSharedData->mResShaderSymbolArray[type] = array;
    }

    void setResShaderVariationDefaultArray(const ResShaderVariationArray& array)
    {
        mpSharedData->mResShaderVariationDefaultArray = array;
    }

    void initialize(const sead::SafeString& name, sead::Heap* heap);

    void createVariationBuffer(s32 macro_num, sead::Heap* heap);

    void createVariationMacro(s32 index, const sead::SafeString& name, const sead::SafeString& id, s32 value_num, sead::Heap* heap);
    void setVariationMacroValue(s32 macro_index, s32 value_index, const sead::SafeString& value);

    void createVariation(sead::Heap* heap);

    ShaderMode activate(ShaderMode current_mode = cShaderMode_Invalid, bool use_dl = true) const;

    Shader* getShader(ShaderType type);
    const Shader* getShader(ShaderType type) const;

#ifdef cafe
    GX2VertexShader* getVertexShaderBinary()
    {
        validate_();
        return mVertexShader.getBinary();
    }

    const GX2VertexShader* getVertexShaderBinary() const
    {
        validate_();
        return mVertexShader.getBinary();
    }

    GX2PixelShader* getFragmentShaderBinary()
    {
        validate_();
        return mFragmentShader.getBinary();
    }

    const GX2PixelShader* getFragmentShaderBinary() const
    {
        validate_();
        return mFragmentShader.getBinary();
    }

    GX2GeometryShader* getGeometryShaderBinary()
    {
        validate_();
        return mGeometryShader.getBinary();
    }

    const GX2GeometryShader* getGeometryShaderBinary() const
    {
        validate_();
        return mGeometryShader.getBinary();
    }
#endif // cafe

    u32 setUpAllVariation(); // I don't know the actual return type
    void reserveSetUpAllVariation();

    s32 getVariationNum() const;
    s32 getVariationMacroNum() const;

    s32 searchVariationShaderProgramIndex(s32 macro_num, const char* const* macro_array, const char* const* value_array) const;

    ShaderProgram* getVariation(s32 index);
    const ShaderProgram* getVariation(s32 index) const;

    const ShaderProgram* searchVariationShaderProgram(s32 macro_num, const char* const* macro_array, const char* const* value_array) const
    {
        s32 index = searchVariationShaderProgramIndex(macro_num, macro_array, value_array);
        return getVariation(index);
    }

    const sead::SafeString& searchVariationMacroName(const sead::SafeString& id) const;

    s32 getVariationMacroValueVariationNum(s32 macro_index) const;

    bool getCompileEnable() const
    {
        return mFlag.isOn(1);
    }

    void setCompileEnable(bool enable)
    {
        mFlag.change(1, enable);
    }

    void update() const // Shrug
    {
        validate_();
    }

    void updateVariation(s32 index) // I don't know the actual name
    {
        ShaderProgram* program = getVariation(index);
        program->mFlag.set(8 | 2);
        program->validate_();
    }

    const AttributeLocation& getAttributeLocation(s32 index) const { return mAttributeLocation[index]; }
    const UniformLocation& getUniformLocation(s32 index) const { return mUniformLocation[index]; }
    const UniformBlockLocation& getUniformBlockLocation(s32 index) const { return mUniformBlockLocation[index]; }
    const SamplerLocation& getSamplerLocation(s32 index) const { return mSamplerLocation[index]; }

    void updateAttributeLocation() const;
    void updateUniformLocation() const;
    void updateUniformBlockLocation() const;
    void updateSamplerLocation() const;

    void dump() const;

    void cleanUp();

    void destroyAttribute();
    void destroyUniform();
    void destroyUniformBlock();
    void destroySamplerLocation();

private:
    u32 validate_() const;
    u32 forceValidate_() const;

    void setUpForVariation_() const;

    void setShaderGX2_() const;

    class SharedData;
    class VariationBuffer;

private:
    class VariationBuffer
    {
        VariationBuffer();
        virtual ~VariationBuffer();

        void initialize(ShaderProgram* program, s32 macro_num, sead::Heap* heap);

        void createMacro(s32 index, const sead::SafeString& name, const sead::SafeString& id, s32 value_num, sead::Heap* heap);
        void setMacroValue(s32 macro_index, s32 value_index, const sead::SafeString& value);

        s32 searchShaderProgramIndex(s32 macro_num, const char* const* macro_array, const char* const* value_array, s32 index) const;

        const sead::SafeString& searchMacroName(const sead::SafeString& id) const;

        void create(sead::Heap* heap);

        s32 getMacroAndValueArray(s32 index, const char** macro_array, const char** value_array) const;
        s32 getMacroValueIndexArray(s32 index, s32* value_index_array) const;
        s32 calcVariationIndex(const s32* value_index_array) const;

        s32 getMacroValueVariationNum(s32 macro_index) const
        {
            return mMacroData[macro_index].mValueVariationNum;
        }

        class MacroData
        {
            sead::SafeString mName;
            sead::SafeString mID;
            sead::Buffer<sead::SafeString> mValue;
            u16 mValueVariationNum;                 // Number of variations using *succeeding* macros (i.e., ignoring preceding macros), if value of this macro is fixed.
                                                    // i.e., In a matrix where variations are the rows, macros are the columns, and a cell would be a macro's value
                                                    // in a certain variation, this field is the number a macro's value occurs before the next value in the macro's column.

            friend class VariationBuffer;
        };
        static_assert(sizeof(MacroData) == 0x1C, "agl::ShaderProgram::VariationBuffer::MacroData size mismatch");

        ShaderProgram* mpOriginal;
        sead::Buffer<ShaderProgram> mProgram;
        sead::Buffer<MacroData> mMacroData;

        friend class ShaderProgram;
        friend class SharedData;
    };
    static_assert(sizeof(VariationBuffer) == 0x18, "agl::ShaderProgram::VariationBuffer size mismatch");

    class SharedData : public sead::INamable
    {
        ShaderProgram* mpOriginal;
        VariationBuffer* mpVariationBuffer;
        u32 _10;
        ResShaderVariationArray mResShaderVariationDefaultArray; // Variation macro defaults
        sead::UnsafeArray<ResShaderSymbolArray, cShaderSymbolType_Num> mResShaderSymbolArray;
        u32 _28;

        friend class ShaderProgram;
    };
    static_assert(sizeof(SharedData) == 0x2C, "agl::ShaderProgram::SharedData size mismatch");

    VariationBuffer* getVariation_() { return mpSharedData->mpVariationBuffer; }
    const VariationBuffer* getVariation_() const { return mpSharedData->mpVariationBuffer; }

private:
    mutable sead::BitFlag8 mFlag;
    u16 mVariationID;
    mutable DisplayList mDisplayList;
    mutable sead::Buffer<AttributeLocation> mAttributeLocation;
    mutable sead::Buffer<UniformLocation> mUniformLocation;
    mutable sead::Buffer<UniformBlockLocation> mUniformBlockLocation;
    mutable sead::Buffer<SamplerLocation> mSamplerLocation;
    VertexShader mVertexShader;
    FragmentShader mFragmentShader;
    GeometryShader mGeometryShader;
    SharedData* mpSharedData;
};
static_assert(sizeof(ShaderProgram) == 0x60, "agl::ShaderProgram size mismatch");

}
