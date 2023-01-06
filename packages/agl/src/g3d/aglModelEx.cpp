#include <common/aglShaderProgram.h>
#include <g3d/aglModelEx.h>

namespace agl { namespace g3d {

MaterialEx::MaterialEx()
    : mpModelEx(nullptr)
    , mpMaterialObj(nullptr)
    , mpProgram(nullptr)
    , mMatBlock()
    , _20(true)
{
}

MaterialEx::~MaterialEx()
{
    fixUpUBO();
}

void MaterialEx::init(agl::g3d::ModelEx* p_model, u32 index, sead::Heap* heap)
{
    mpModelEx = p_model;
    mpMaterialObj = p_model->GetMaterial(index);
    mMatBlock = static_cast<nw::g3d::fnd::GfxBuffer_t&>(mpMaterialObj->GetMatBlock());
}

void MaterialEx::bindShaderResAssign(const ShaderProgram* p_program, const char* skin_macro, const char** skin_value_array)
{
    const char* skin_macro_array[1] = { skin_macro };
    const u32 skin_macro_num = 1;

    mpProgram = p_program;

    const nw::g3d::res::ResShaderAssign* p_res_shader_assign = mpMaterialObj->GetResource()->GetShaderAssign();
    if (p_res_shader_assign == nullptr || p_program == nullptr)
    {
        for (s32 idx_shape = 0; idx_shape < mpModelEx->GetShapeCount(); idx_shape++)
        {
            if (&mpModelEx->getMaterialEx(mpModelEx->GetShape(idx_shape)->GetMaterialIndex()) == this)
            {
                mpModelEx->getShaderAssign(idx_shape).bindShaderResAssign(
                    mpMaterialObj->GetResource(),
                    mpModelEx->GetShape(idx_shape)->GetResource(),
                    nullptr
                );
            }
        }
        return;
    }

    const char* macro_array[ShaderProgram::cVariationMacroMax];
    const char* value_array[ShaderProgram::cVariationValueMax];
    s32 macro_num = p_res_shader_assign->GetShaderOptionCount();

    for (s32 idx_macro = 0; idx_macro < macro_num; idx_macro++)
    {
        sead::SafeString id = p_res_shader_assign->GetShaderOptionName(idx_macro);
        const char* value = p_res_shader_assign->GetShaderOption(idx_macro);

        macro_array[idx_macro] = p_program->searchVariationMacroName(id).cstr();
        value_array[idx_macro] = value;
    }

    const ShaderProgram* p_base_variation = p_program->searchVariationShaderProgram(macro_num, macro_array, value_array);

    for (s32 idx_shape = 0; idx_shape < mpModelEx->GetShapeCount(); idx_shape++)
    {
        if (&mpModelEx->getMaterialEx(mpModelEx->GetShape(idx_shape)->GetMaterialIndex()) == this)
        {
            const ShaderProgram* p_variation = p_base_variation;
            if (p_base_variation && skin_value_array)
                p_variation = p_base_variation->searchVariationShaderProgram(skin_macro_num, skin_macro_array, &skin_value_array[mpModelEx->GetShape(idx_shape)->GetVtxSkinCount()]);

            mpModelEx->getShaderAssign(idx_shape).bindShaderResAssign(
                mpMaterialObj->GetResource(),
                mpModelEx->GetShape(idx_shape)->GetResource(),
                p_variation
            );
        }
    }
}

void MaterialEx::bindShader(const ShaderProgram* p_program)
{
    mpProgram = p_program;

    for (s32 idx_shape = 0; idx_shape < mpModelEx->GetShapeCount(); idx_shape++)
    {
        if (&mpModelEx->getMaterialEx(mpModelEx->GetShape(idx_shape)->GetMaterialIndex()) == this)
        {
            mpModelEx->getShaderAssign(idx_shape).bindShader(
                mpMaterialObj->GetResource(),
                mpModelEx->GetShape(idx_shape)->GetResource(),
                p_program
            );
        }
    }
}

void MaterialEx::replaceUBO(const nw::g3d::fnd::GfxBuffer_t& buffer)
{
    static_cast<nw::g3d::fnd::GfxBuffer_t&>(mpMaterialObj->GetMatBlock()) = buffer;
}

void MaterialEx::fixUpUBO()
{
    replaceUBO(mMatBlock);
}

ModelEx::ModelEx()
    : nw::g3d::ModelObj()
    , mpShaderAssign(nullptr)
    , mpMaterialEx(nullptr)
{
}

ModelEx::~ModelEx()
{
    destroyEx();
}

void ModelEx::createEx(sead::Heap* heap)
{
    const nw::g3d::res::ResModel* p_res_mdl = GetResource();
    s32 shape_num = p_res_mdl->GetShapeCount();
    s32 material_num = p_res_mdl->GetMaterialCount();

    mpShaderAssign = new (heap) ModelShaderAssign[shape_num];
    mpMaterialEx = new (heap) MaterialEx[material_num];

    for (s32 idx_material = 0; idx_material < material_num; idx_material++)
        mpMaterialEx[idx_material].init(this, idx_material, heap);

    for (s32 idx_shape = 0; idx_shape < shape_num; idx_shape++)
        mpShaderAssign[idx_shape].create(heap);
}

void ModelEx::destroyEx()
{
    if (mpShaderAssign)
    {
        delete[] mpShaderAssign;
        mpShaderAssign = nullptr;
    }
    if (mpMaterialEx)
    {
        delete[] mpMaterialEx;
        mpMaterialEx = nullptr;
    }
}

} }
