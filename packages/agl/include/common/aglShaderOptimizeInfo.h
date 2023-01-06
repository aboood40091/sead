#pragma once

#include <basis/seadTypes.h>

namespace agl {

class ShaderOptimizeInfo
{
public:
    ShaderOptimizeInfo();
    virtual ~ShaderOptimizeInfo();

    void setDefault();

    u32 getVertexShaderGprs() const { return mVertexShader.gprs; }
    u32 getVertexShaderStackSize() const { return mVertexShader.stack_size; }
    void setVertexShader(u32 gprs, u32 stack_size);

    u32 getVertexShaderWithGSGprs() const { return mVertexShaderWithGS.gprs; }
    u32 getVertexShaderWithGSStackSize() const { return mVertexShaderWithGS.stack_size; }
    void setVertexShaderWithGS(u32 gprs, u32 stack_size);

    u32 getFragmentShaderGprs() const { return mFragmentShader.gprs; }
    u32 getFragmentShaderStackSize() const { return mFragmentShader.stack_size; }
    void setFragmentShader(u32 gprs, u32 stack_size);

    u32 getFragmentShaderWithGSGprs() const { return mFragmentShaderWithGS.gprs; }
    u32 getFragmentShaderWithGSStackSize() const { return mFragmentShaderWithGS.stack_size; }
    void setFragmentShaderWithGS(u32 gprs, u32 stack_size);

    u32 getGeometryShaderGprs() const { return mGeometryShader.gprs; }
    u32 getGeometryShaderStackSize() const { return mGeometryShader.stack_size; }
    void setGeometryShader(u32 gprs, u32 stack_size);

private:
    struct
    {
        u8 gprs;
        u8 stack_size;
    } mVertexShader;
    struct
    {
        u8 gprs;
        u8 stack_size;
    } mVertexShaderWithGS;
    struct
    {
        u8 gprs;
        u8 stack_size;
    } mFragmentShader;
    struct
    {
        u8 gprs;
        u8 stack_size;
    } mFragmentShaderWithGS;
    struct
    {
        u8 gprs;
        u8 stack_size;
    } mGeometryShader;
};
static_assert(sizeof(ShaderOptimizeInfo) == 0x10, "agl::ShaderOptimizeInfo size mismatch");

}
