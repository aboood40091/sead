#pragma once

#include <common/aglResShaderMacro.h>
#include <common/aglResShaderSymbol.h>
#include <common/aglResShaderVariation.h>

namespace agl {

struct ResShaderProgramData
{
    u32 mSize;
    u32 mNameLen;
    s32 mSourceIndex[cShaderType_Num];
    // char mName[];
};
static_assert(sizeof(ResShaderProgramData) == 0x14, "agl::ResShaderProgramData size mismatch");

class ResShaderProgram : public ResCommon<ResShaderProgramData>
{
    AGL_RES_COMMON(ResShaderProgram)

public:
    const char* getName() const
    {
        return (const char*)(ptr() + 1);
    }

    ResShaderMacroArray getResShaderMacroArray(ShaderType type) const;

    ResShaderVariationArray getResShaderVariationArray() const;

    ResShaderVariationArray getResShaderVariationDefaultArray() const
    {
        return (const ResShaderVariationArrayData*)((uintptr_t)getResShaderVariationArray().ptr() + getResShaderVariationArray().ref().mSize);
    }

    ResShaderSymbolArray getResShaderSymbolArray(ShaderSymbolType type) const;
};

typedef ResArray<ResShaderProgram> ResShaderProgramArray;

typedef ResShaderProgramArray::DataType ResShaderProgramArrayData;
static_assert(sizeof(ResShaderProgramArrayData) == 8, "agl::ResShaderProgramArrayData size mismatch");

}
