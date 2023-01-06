#include <common/aglShader.h>
#include <common/aglShaderCompileInfo.h>
#include <detail/aglFileIOMgr.h>
#include <detail/aglPrivateResource.h>

namespace agl {

Shader::Shader()
    : mBinary(nullptr)
    , mCompileInfo(nullptr)
{
}

u32 Shader::setUp(bool compile_source, bool) const
{
    u32 ret = 2;

    if (mCompileInfo && compile_source)
    {
        sead::HeapSafeString* temp_text = detail::PrivateResource::instance()->getShaderText();
#ifdef cafe
        mCompileInfo->calcCompileSource(getShaderType(), temp_text, ShaderCompileInfo::cTarget_GX2, true);
#else
        mCompileInfo->calcCompileSource(getShaderType(), temp_text, ShaderCompileInfo::cTarget_GL, true);
#endif

        static const char* sSavePath[cShaderType_Num] = {
            "%AGL_ROOT%/tools/temporary/temp_vs.sh",
            "%AGL_ROOT%/tools/temporary/temp_fs.sh",
            "%AGL_ROOT%/tools/temporary/temp_gs.sh"
        };

        detail::FileIOMgr::DialogArg arg;
        arg.mPath = sSavePath[getShaderType()];

        detail::FileIOMgr::instance()->save(temp_text->cstr(), temp_text->calcLength(), arg);

        ret = 0;
    }

    if (mBinary)
        ret = 0;

    return ret;
}

void Shader::setBinary(const void* binary)
{
    mBinary = binary;
}

}
