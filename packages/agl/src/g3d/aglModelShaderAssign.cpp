#include <common/aglResShaderSymbol.h>
#include <common/aglShaderProgram.h>
#include <container/seadBuffer.h>
#include <g3d/aglModelShaderAssign.h>

namespace agl { namespace g3d {

ModelShaderAttribute::ModelShaderAttribute()
{
    clear();
}

ModelShaderAttribute::~ModelShaderAttribute()
{
    if (mFetchShader.GetShaderPtr())
        delete mFetchShader.GetShaderPtr(); // Nintendo forgot to do delete[] again
}

void ModelShaderAttribute::create(sead::Heap* heap)
{
    if (mFetchShader.GetShaderPtr())
        delete[] mFetchShader.GetShaderPtr();

    mFetchShader.SetAttribCount(16);
    mFetchShader.CalcSize();
    mFetchShader.SetShaderPtr(new (heap, nw::g3d::fnd::GfxFetchShader::SHADER_ALIGNMENT) u8[mFetchShader.GetShaderSize()]);
}

void ModelShaderAttribute::clear()
{
    mVertexBufferNum = 0;

    for (sead::UnsafeArray<const nw::g3d::fnd::GfxBuffer*, 16>::iterator it = mVertexBuffer.begin(), it_end = mVertexBuffer.end(); it != it_end; ++it)
        *it = nullptr;
}

void ModelShaderAttribute::bind(const nw::g3d::res::ResMaterial* p_res_mat, const nw::g3d::res::ResShape* p_res_shp, const ShaderProgram* p_program, bool use_res_assign, bool use_shader_symbol_id)
{
    // SEAD_ASSERT(p_res_mat != nullptr);
    // SEAD_ASSERT(p_res_shp != nullptr);

    const nw::g3d::res::ResShaderAssign* p_res_shader_assign;
    if (use_res_assign)
        p_res_shader_assign = p_res_mat->GetShaderAssign();
    else
        p_res_shader_assign = nullptr;

    const ResShaderSymbolArray& symbol_array = p_program->getResShaderSymbolArray(cShaderSymbolType_Attribute);
    sead::UnsafeArray<Attribute, 16> attribute;
    s32 attribute_num = 0;
    for (ResShaderSymbolArray::constIterator it = symbol_array.begin(), it_end = symbol_array.end(); it != it_end; ++it)
    {
        const ResShaderSymbol& symbol = &(*it);
        if (!symbol.isValid())
            continue;

        const char* symbol_id = symbol.getID();
        if (!symbol.isVariationEnable(p_program->getVariationID()))
            continue;

        const char* name = nullptr;
        if (p_res_shader_assign)
            name = p_res_shader_assign->GetAttribAssign(symbol_id);

        if (name == nullptr)
        {
            if (!use_shader_symbol_id)
                continue;

            name = symbol.getID();
            if (name == nullptr)
                continue;
        }

        AttributeLocation location(symbol.getName(), *p_program);
        s32 index = p_res_shp->GetVertex()->GetVtxAttribIndex(name);

        if (location.isValid() && index != -1)
        {
            attribute[attribute_num].mName = location.getName();
            attribute[attribute_num].mIndex = index;
            attribute[attribute_num].mLocation = location.getVertexLocation();
            attribute_num++;
        }
    }

    const nw::g3d::res::ResVertex* p_res_vtx = p_res_shp->GetVertex();

// void ModelShaderAttribute::bind_(const nw::g3d::res::ResVertex* p_res_vtx, const sead::UnsafeArray<Attribute, 16>& attribute, s32 attribute_num)
{
    // SEAD_ASSERT(p_res_vtx != nullptr);

    mVertexBufferNum = 0;

    u32 slot[16];
    for (s32 idx_attrib = 0; idx_attrib < attribute_num; idx_attrib++)
        slot[idx_attrib] = 0xFFFFFFFF;

    mFetchShader.Cleanup();
    mFetchShader.SetAttribCount(attribute_num);
    mFetchShader.CalcSize();
    mFetchShader.SetDefault(mFetchShader.GetGX2FetchShader()->shaderPtr);

    for (s32 idx_attrib = 0; idx_attrib < attribute_num; idx_attrib++)
    {
        const nw::g3d::res::ResVtxAttrib* p_res_vtx_attrib = p_res_vtx->GetVtxAttrib(attribute[idx_attrib].mIndex);
        const nw::g3d::res::ResBuffer* p_res_buffer = p_res_vtx->GetVtxBuffer(p_res_vtx_attrib->GetBufferIndex());

        if (slot[p_res_vtx_attrib->GetBufferIndex()] == 0xFFFFFFFF)
        {
            slot[p_res_vtx_attrib->GetBufferIndex()] = mVertexBufferNum;
            mVertexBuffer[mVertexBufferNum] = p_res_vtx->GetVtxBuffer(p_res_vtx_attrib->GetBufferIndex())->GetGfxBuffer();
            mVertexBufferNum++;
        }

        mFetchShader.SetLocation(mFetchShader.GetGX2FetchShader()->shaderPtr, idx_attrib, attribute[idx_attrib].mLocation);
        mFetchShader.SetVertexBuffer(idx_attrib, p_res_buffer->GetGfxBuffer());
        mFetchShader.SetBufferSlot(mFetchShader.GetGX2FetchShader()->shaderPtr, idx_attrib, slot[p_res_vtx_attrib->GetBufferIndex()]);
        mFetchShader.SetFormat(mFetchShader.GetGX2FetchShader()->shaderPtr, idx_attrib, p_res_vtx_attrib->GetFormat());
        mFetchShader.SetOffset(mFetchShader.GetGX2FetchShader()->shaderPtr, idx_attrib, p_res_vtx_attrib->GetOffset());
    }

    mFetchShader.Setup();
    mFetchShader.DCFlush();
}

}

void ModelShaderAttribute::activateVertexBuffer() const
{
    mFetchShader.Load();

    for (u32 i = 0; i < mVertexBufferNum; i++)
        mVertexBuffer[i]->LoadVertices(i);
}

void ModelShaderAttribute::setVertexBuffer(const nw::g3d::fnd::GfxBuffer* p_buffer, s32 index)
{
    for (u32 idx_attrib = 0; idx_attrib < mFetchShader.GetAttribCount(); idx_attrib++)
        if (mFetchShader.GetVertexBuffer(idx_attrib) == mVertexBuffer[index])
            mFetchShader.SetVertexBuffer(idx_attrib, p_buffer);

    mVertexBuffer[index] = p_buffer;
}

ModelShaderAssign::ModelShaderAssign()
    : mpProgram(nullptr)
{
    clear_();
}

ModelShaderAssign::~ModelShaderAssign()
{
}

void ModelShaderAssign::create(sead::Heap* heap)
{
    mAttribute.create(heap);
}

void ModelShaderAssign::clear_()
{
    mAttribute.clear();
    mSamplerNum = 0;

    for (sead::UnsafeArray<const nw::g3d::res::ResSampler*, 16>::iterator it = mResSampler.begin(), it_end = mResSampler.end(); it != it_end; ++it)
        *it = nullptr;
}

void ModelShaderAssign::bind(const nw::g3d::res::ResMaterial* p_res_mat, const ShaderProgram* p_program, bool use_res_assign, bool use_shader_symbol_id)
{
    const nw::g3d::res::ResShaderAssign* p_res_shader_assign;
    if (use_res_assign)
        p_res_shader_assign = p_res_mat->GetShaderAssign();
    else
        p_res_shader_assign = nullptr;

    const ResShaderSymbolArray& symbol_array = p_program->getResShaderSymbolArray(cShaderSymbolType_Sampler);
    for (ResShaderSymbolArray::constIterator it = symbol_array.begin(), it_end = symbol_array.end(); it != it_end; ++it)
    {
        const ResShaderSymbol& symbol = &(*it);
        if (!symbol.isValid())
            continue;

        const char* symbol_id = symbol.getID();
        if (!symbol.isVariationEnable(p_program->getVariationID()))
            continue;

        const char* name = nullptr;
        if (p_res_shader_assign)
            name = p_res_shader_assign->GetSamplerAssign(symbol_id);

        if (name == nullptr)
        {
            if (!use_shader_symbol_id)
                continue;

            name = symbol.getID();
            if (name == nullptr)
                continue;
        }

        const nw::g3d::res::ResSampler* p_res_sampler = p_res_mat->GetSampler(name);
        if (p_res_sampler)
            pushBackSampler(p_res_sampler, SamplerLocation(symbol.getName(), *p_program));
    }
}

void ModelShaderAssign::bindShaderResAssign(const nw::g3d::res::ResMaterial* p_res_mat, const nw::g3d::res::ResShape* p_res_shp, const ShaderProgram* p_program, const char*)
{
    const nw::g3d::res::ResShaderAssign* p_res_shader_assign = p_res_mat->GetShaderAssign();

    clear_();

    if (p_res_shader_assign && p_program)
    {
        mpProgram = p_program;
        bind(p_res_mat, p_program, true, false);
        mAttribute.bind(p_res_mat, p_res_shp, p_program, true, false);
        updateLocation_("Mat");
    }
}

void ModelShaderAssign::bindShader(const nw::g3d::res::ResMaterial* p_res_mat, const nw::g3d::res::ResShape* p_res_shp, const ShaderProgram* p_program, const char*)
{
    clear_();

    if (p_program)
    {
        mpProgram = p_program;
        bind(p_res_mat, p_program, false, true);
        mAttribute.bind(p_res_mat, p_res_shp, p_program, false, true);
        updateLocation_("Mat");
    }
}

void ModelShaderAssign::updateLocation_(const char* uniform_block_name)
{
    mUniformBlockLocation.setName(uniform_block_name);
    mUniformBlockLocation.search(*mpProgram);
}

void ModelShaderAssign::pushBackSampler(const nw::g3d::res::ResSampler* p_res_sampler, const SamplerLocation& location)
{
    if (!location.isValid())
        return;

    mSamplerLocation[mSamplerNum] = location;
    mResSampler[mSamplerNum] = p_res_sampler;
    mSamplerNum++;
}

void ModelShaderAssign::activateMaterialUniformBlock(const nw::g3d::MaterialObj* p_material) const
{
    if (mUniformBlockLocation.isValid() && p_material->GetMatBlock().GetSize() > 0)
    {
        if (mUniformBlockLocation.getVertexLocation() != -1)
            p_material->GetMatBlock().LoadVertexUniforms(mUniformBlockLocation.getVertexLocation());

        if (mUniformBlockLocation.getGeometryLocation() != -1)
            p_material->GetMatBlock().LoadGeometryUniforms(mUniformBlockLocation.getGeometryLocation());

        if (mUniformBlockLocation.getFragmentLocation() != -1)
            p_material->GetMatBlock().LoadFragmentUniforms(mUniformBlockLocation.getFragmentLocation());
    }
}

void ModelShaderAssign::activateTextureSampler(const nw::g3d::MaterialObj* p_material) const
{
    for (u32 i = 0; i < mSamplerNum; i++)
    {
        const nw::g3d::res::ResSampler* p_res_sampler = mResSampler[i];
        const nw::g3d::res::ResTexture* p_res_texture = p_material->GetResTexture(p_res_sampler->GetIndex());

        if (p_res_texture)
        {
            const nw::g3d::fnd::GfxSampler* p_gfx_sampler = p_res_sampler->GetGfxSampler();
            const nw::g3d::fnd::GfxTexture* p_gfx_texture = p_res_texture->GetGfxTexture();

            const agl::SamplerLocation& sampler_location = mSamplerLocation[i];

            if (sampler_location.getVertexLocation() != -1)
            {
                p_gfx_sampler->LoadVertexSampler(sampler_location.getVertexLocation());
                p_gfx_texture->LoadVertexTexture(sampler_location.getVertexLocation());
            }

            if (sampler_location.getGeometryLocation() != -1)
            {
                p_gfx_sampler->LoadGeometrySampler(sampler_location.getGeometryLocation());
                p_gfx_texture->LoadGeometryTexture(sampler_location.getGeometryLocation());
            }

            if (sampler_location.getFragmentLocation() != -1)
            {
                p_gfx_sampler->LoadFragmentSampler(sampler_location.getFragmentLocation());
                p_gfx_texture->LoadFragmentTexture(sampler_location.getFragmentLocation());
            }
        }
    }
}

} }
