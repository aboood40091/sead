#include <common/aglResShaderSymbol.h>
#include <common/aglShaderProgramArchive.h>
#include <g3d/aglShaderUtilG3D.h>

#include <nw/g3d/res/g3d_ResFile.h>

namespace agl { namespace g3d {

void ShaderUtilG3D::setMatBlockAndShaderParamOffs(nw::g3d::res::ResFile* p_res_file, const ShaderProgramArchive* shader_archive, const sead::SafeString& mat_block_name)
{
    for (s32 idx_model = 0, num_model = p_res_file->GetModelCount(); idx_model < num_model; idx_model++)
        setMatBlockAndShaderParamOffs_(p_res_file->GetModel(idx_model), shader_archive, mat_block_name);
}

void ShaderUtilG3D::setMatBlockAndShaderParamOffs_(nw::g3d::res::ResModel* p_res_mdl, const ShaderProgramArchive* shader_archive, const sead::SafeString& mat_block_name)
{
    for (s32 idx_material = 0, num_material = p_res_mdl->GetMaterialCount(); idx_material < num_material; idx_material++)
    {
        nw::g3d::res::ResMaterial* p_res_mat = p_res_mdl->GetMaterial(idx_material);

        if (!p_res_mat->GetShaderAssign() ||
            !p_res_mat->GetShaderAssign()->GetShaderArchiveName() ||
            !p_res_mat->GetShaderAssign()->GetShadingModelName() ||
            !shader_archive->getName().isEqual(p_res_mat->GetShaderAssign()->GetShaderArchiveName()))
        {
            continue;
        }

        for (s32 idx_program = 0, num_program = shader_archive->getShaderProgramNum(); idx_program < num_program; idx_program++)
        {
            if (!shader_archive->getShaderProgram(idx_program).getName().isEqual(p_res_mat->GetShaderAssign()->GetShadingModelName()))
                continue;

            const ShaderProgram& shader_program = shader_archive->getShaderProgram(idx_program);
            ResShaderSymbolArray uniforms = shader_program.getResShaderSymbolArray(cShaderSymbolType_Uniform);
            ResShaderSymbolArray uniformBlocks = shader_program.getResShaderSymbolArray(cShaderSymbolType_UniformBlock);
            if (uniforms.isValid() && uniformBlocks.isValid())
            {
                const ResShaderSymbol symbol = uniformBlocks.searchResShaderSymbolByID(mat_block_name);
                if (symbol.isValid())
                {
                    p_res_mat->SetRawParamSize(symbol.ref().mOffset);

                    for (s32 idx_param = 0, num_param = p_res_mat->GetShaderParamCount(); idx_param < num_param; idx_param++)
                    {
                        nw::g3d::res::ResShaderParam* p_res_shader_param = p_res_mat->GetShaderParam(idx_param);
                        const ResShaderSymbol param_symbol = uniforms.searchResShaderSymbolByID(p_res_shader_param->GetId());
                        if (param_symbol.isValid())
                            p_res_shader_param->SetOffset(param_symbol.ref().mOffset);
                    }

                    break;
                }
            }
        }
    }
}

} }
