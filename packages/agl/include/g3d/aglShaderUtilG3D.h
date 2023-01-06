#pragma once

#include <prim/seadSafeString.h>

namespace nw { namespace g3d { namespace res {

class ResFile;
class ResModel;

} } }

namespace agl {

class ShaderProgramArchive;

namespace g3d {

class ShaderUtilG3D
{
public:
    // Named these myself, not sure
    static void setMatBlockAndShaderParamOffs(nw::g3d::res::ResFile* p_res_file, const ShaderProgramArchive* shader_archive, const sead::SafeString& mat_block_name);

private:
    static void setMatBlockAndShaderParamOffs_(nw::g3d::res::ResModel* p_res_mdl, const ShaderProgramArchive* shader_archive, const sead::SafeString& mat_block_name);
};

}

}
