#pragma once

#include <common/aglResCommon.h>
#include <common/aglShaderEnum.h>
#include <prim/seadSafeString.h>

namespace agl {

struct ResShaderSymbolData
{
    u32 mSize;
    s32 mOffset;
    u32 mNameLen;
    u32 mIDLen;
    u32 mDefaultValueSize;
    u32 mVariationNum;
    // char mName[];
};
static_assert(sizeof(ResShaderSymbolData) == 0x18, "agl::ResShaderSymbolData size mismatch");

class ResShaderSymbol : public ResCommon<ResShaderSymbolData>
{
    AGL_RES_COMMON(ResShaderSymbol)

public:
    const char* getName() const
    {
        const DataType* const data = ptr();
        return (const char*)(data + 1);
    }

    const char* getID() const
    {
        const DataType* const data = ptr();
        return (const char*)((uintptr_t)(data + 1) + data->mNameLen);
    }

    void* getDefaultValue() const
    {
        const DataType* const data = ptr();
        return (void*)((uintptr_t)(data + 1) + data->mNameLen + data->mIDLen);
    }

    const u8* getVariationEnableArray() const
    {
        const DataType* const data = ptr();
        return (const u8*)((uintptr_t)(data + 1) + data->mNameLen + data->mIDLen + data->mDefaultValueSize);
    }

    bool isVariationEnable(s32 index) const
    {
        return getVariationEnableArray()[index];
    }
};

class ResShaderSymbolArray : public ResArray<ResShaderSymbol>
{
    AGL_RES_ARRAY(ResShaderSymbolArray)

public:
    ResShaderSymbol searchResShaderSymbolByID(const sead::SafeString& id) const;
};

typedef ResShaderSymbolArray::DataType ResShaderSymbolArrayData;
static_assert(sizeof(ResShaderSymbolArrayData) == 8, "agl::ResShaderSymbolArrayData size mismatch");

}
