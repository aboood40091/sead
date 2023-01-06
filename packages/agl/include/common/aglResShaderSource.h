#pragma once

#include <common/aglResCommon.h>

namespace agl {

struct ResShaderSourceData
{
    u32 mSize;
    u32 mNameLen;
    u32 mTextLen; // Text Length
    u32 _c;       // Text Length... 2
    // char mName[];
};
static_assert(sizeof(ResShaderSourceData) == 0x10, "agl::ResShaderSourceData size mismatch");

class ResShaderSource : public ResCommon<ResShaderSourceData>
{
    AGL_RES_COMMON(ResShaderSource)

public:
    const char* getName() const
    {
        return (const char*)(ptr() + 1);
    }

    const char* getText() const
    {
        return getName() + ptr()->mNameLen;
    }
};

typedef ResArray<ResShaderSource> ResShaderSourceArray;

typedef ResShaderSourceArray::DataType ResShaderSourceArrayData;
static_assert(sizeof(ResShaderSourceArrayData) == 8, "agl::ResShaderSourceArrayData size mismatch");

}
