#include <common/aglShaderOptimizeInfo.h>

namespace agl {

ShaderOptimizeInfo::ShaderOptimizeInfo()
{
    setDefault();
}

ShaderOptimizeInfo::~ShaderOptimizeInfo()
{
}

void ShaderOptimizeInfo::setDefault()
{
    mVertexShader.gprs = 48;
    mVertexShader.stack_size = 64;

    mVertexShaderWithGS.gprs = 44;
    mVertexShaderWithGS.stack_size = 32;

    mFragmentShader.gprs = 200;
    mFragmentShader.stack_size = 192;

    mFragmentShaderWithGS.gprs = 76;
    mFragmentShaderWithGS.stack_size = 176;

    mGeometryShader.gprs = 64;
    mGeometryShader.stack_size = 48;
}

}
