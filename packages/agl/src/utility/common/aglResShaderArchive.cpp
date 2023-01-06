#include <common/aglResShaderArchive.h>
#include <common/aglResShaderVariation.h>
#include <common/aglResShaderSymbol.h>
#include <common/aglShader.h>
#include <prim/seadEndian.h>

#ifdef cafe
#include <cafe/gx2.h>
#endif // cafe

static inline void swap32(void* ptr, size_t size)
{
    u32* ptr_u32 = static_cast<u32*>(ptr);
    u32 count = size / sizeof(u32);

    for (u32 i = 0; i < count; i++)
    {
        *ptr_u32 = sead::Endian::swap(*ptr_u32);
        ptr_u32++;
    }
}

namespace {

void modifyEndianResSymbolArray(bool is_le, agl::ResShaderSymbolArray symbol_array, agl::ShaderSymbolType type)
{
    symbol_array.modifyEndianArray(is_le);

    if (type != agl::cShaderSymbolType_UniformBlock)
        for (agl::ResShaderSymbolArray::iterator it = symbol_array.begin(), it_end = symbol_array.end(); it != it_end; ++it)
            agl::ModifyEndianU32(is_le, agl::ResShaderSymbol(&(*it)).getDefaultValue(), it->mDefaultValueSize);
}

#ifdef cafe

template <typename T>
T* modifyBinaryAndNamePtr(void* base_ptr, T* ptr, s32 num)
{
    if (!ptr)
        return nullptr;

    ptr = (T*)(uintptr_t(base_ptr) + uintptr_t(ptr));

    for (s32 i = 0; i < num; i++)
        ptr[i].name += uintptr_t(base_ptr);

    return ptr;
}

void* modifyBinaryPtr(void* base_ptr, void* ptr)
{
    return (void*)(uintptr_t(base_ptr) + uintptr_t(ptr));
}

#endif // cafe

}

namespace agl {

void ResShaderBinary::modifyBinaryEndian()
{
    size_t size = 0;
    void* data = nullptr;

#ifdef cafe
    switch (getShaderType())
    {
    case cShaderType_Vertex:
        {
            GX2VertexShader* vertex_shader = static_cast<GX2VertexShader*>(getData());
            swap32(vertex_shader, sizeof(GX2VertexShader));

            size += vertex_shader->numUniformBlocks * sizeof(GX2UniformBlock) +
                    vertex_shader->numUniforms * sizeof(GX2UniformVar) +
                    vertex_shader->numInitialValues * sizeof(GX2UniformInitialValue) +
                    vertex_shader->numSamplers * sizeof(GX2SamplerVar) +
                    vertex_shader->numAttribs * sizeof(GX2AttribVar);

            data = vertex_shader + 1;
        }
        break;
    case cShaderType_Fragment:
        {
            GX2PixelShader* pixel_shader = static_cast<GX2PixelShader*>(getData());
            swap32(pixel_shader, sizeof(GX2PixelShader));

            size += pixel_shader->numUniformBlocks * sizeof(GX2UniformBlock) +
                    pixel_shader->numUniforms * sizeof(GX2UniformVar) +
                    pixel_shader->numInitialValues * sizeof(GX2UniformInitialValue) +
                    pixel_shader->numSamplers * sizeof(GX2SamplerVar);

            data = pixel_shader + 1;
        }
        break;
    case cShaderType_Geometry:
        {
            GX2GeometryShader* geometry_shader = static_cast<GX2GeometryShader*>(getData());
            swap32(geometry_shader, sizeof(GX2GeometryShader));

            size += geometry_shader->numUniformBlocks * sizeof(GX2UniformBlock) +
                    geometry_shader->numUniforms * sizeof(GX2UniformVar) +
                    geometry_shader->numInitialValues * sizeof(GX2UniformInitialValue) +
                    geometry_shader->numSamplers * sizeof(GX2SamplerVar);

            data = geometry_shader + 1;
        }
        break;
    }
#endif // cafe

    swap32(data, size);
}

void ResShaderBinary::setUp()
{
#ifdef cafe
    switch (getShaderType())
    {
    case cShaderType_Vertex:
        {
            GX2VertexShader* vertex_shader = static_cast<GX2VertexShader*>(getData());

            vertex_shader->uniformBlocks = modifyBinaryAndNamePtr<GX2UniformBlock>(vertex_shader, vertex_shader->uniformBlocks, vertex_shader->numUniformBlocks);
            vertex_shader->uniformVars   = modifyBinaryAndNamePtr<GX2UniformVar  >(vertex_shader, vertex_shader->uniformVars,   vertex_shader->numUniforms);
            vertex_shader->samplerVars   = modifyBinaryAndNamePtr<GX2SamplerVar  >(vertex_shader, vertex_shader->samplerVars,   vertex_shader->numSamplers);
            vertex_shader->attribVars    = modifyBinaryAndNamePtr<GX2AttribVar   >(vertex_shader, vertex_shader->attribVars,    vertex_shader->numAttribs);

            vertex_shader->_loopVars = modifyBinaryPtr(vertex_shader, vertex_shader->_loopVars);
            vertex_shader->shaderPtr = modifyBinaryPtr(vertex_shader, vertex_shader->shaderPtr);
        }
        break;
    case cShaderType_Fragment:
        {
            GX2PixelShader* pixel_shader = static_cast<GX2PixelShader*>(getData());

            pixel_shader->uniformBlocks = modifyBinaryAndNamePtr<GX2UniformBlock>(pixel_shader, pixel_shader->uniformBlocks, pixel_shader->numUniformBlocks);
            pixel_shader->uniformVars   = modifyBinaryAndNamePtr<GX2UniformVar  >(pixel_shader, pixel_shader->uniformVars,   pixel_shader->numUniforms);
            pixel_shader->samplerVars   = modifyBinaryAndNamePtr<GX2SamplerVar  >(pixel_shader, pixel_shader->samplerVars,   pixel_shader->numSamplers);

            pixel_shader->_loopVars = modifyBinaryPtr(pixel_shader, pixel_shader->_loopVars);
            pixel_shader->shaderPtr = modifyBinaryPtr(pixel_shader, pixel_shader->shaderPtr);
        }
        break;
    case cShaderType_Geometry:
        {
            GX2GeometryShader* geometry_shader = static_cast<GX2GeometryShader*>(getData());

            geometry_shader->uniformBlocks = modifyBinaryAndNamePtr<GX2UniformBlock>(geometry_shader, geometry_shader->uniformBlocks, geometry_shader->numUniformBlocks);
            geometry_shader->uniformVars   = modifyBinaryAndNamePtr<GX2UniformVar  >(geometry_shader, geometry_shader->uniformVars,   geometry_shader->numUniforms);
            geometry_shader->samplerVars   = modifyBinaryAndNamePtr<GX2SamplerVar  >(geometry_shader, geometry_shader->samplerVars,   geometry_shader->numSamplers);

            geometry_shader->_loopVars     = modifyBinaryPtr(geometry_shader, geometry_shader->_loopVars);
            geometry_shader->shaderPtr     = modifyBinaryPtr(geometry_shader, geometry_shader->shaderPtr);
            geometry_shader->copyShaderPtr = modifyBinaryPtr(geometry_shader, geometry_shader->copyShaderPtr);
        }
        break;
    }
#endif // cafe
}

const char* ResShaderVariation::getID() const
{
    const char* value = getName() + ref().mNameLen;

    for (s32 i = 0, index = ref().mValueNum; ; i++)
    {
        while (*value == '\0')
            value++;

        if (i == index)
            break;

        do
        {
            value++;
        }
        while (*value != '\0');

        value++;
    }

    return value;
}

const char* ResShaderVariation::getValue(s32 index) const
{
    // SEAD_ASSERT(0 <= index && index < static_cast< int >( ref().mValueNum ));

    const char* value = getName() + ref().mNameLen;

    for (s32 i = 0; ; i++)
    {
        while (*value == '\0')
            value++;

        if (i == index)
            break;

        do
        {
            value++;
        }
        while (*value != '\0');

        value++;
    }

    return value;
}

ResShaderSymbol ResShaderSymbolArray::searchResShaderSymbolByID(const sead::SafeString& id) const
{
    for (constIterator it = begin(), it_end = end(); it != it_end; ++it)
    {
        if (id.isEqual(ResShaderSymbol(&(*it)).getID()))
            return &(*it);
    }

    return nullptr;
}

ResShaderMacroArray ResShaderProgram::getResShaderMacroArray(ShaderType type) const
{
    const ResShaderMacroArrayData* macro_array;
    {
        const DataType* const data = ptr();
        macro_array = (const ResShaderMacroArrayData*)((uintptr_t)(data + 1) + data->mNameLen);
    }

    for (s32 i = 0; i < type; i++)
        macro_array = (const ResShaderMacroArrayData*)((uintptr_t)macro_array + macro_array->mSize);

    return macro_array;
}

ResShaderVariationArray ResShaderProgram::getResShaderVariationArray() const
{
    const ResShaderMacroArrayData* macro_array;
    {
        const DataType* const data = ptr();
        macro_array = (const ResShaderMacroArrayData*)((uintptr_t)(data + 1) + data->mNameLen);
    }

    for (s32 i = 0; i < cShaderType_Num; i++)
        macro_array = (const ResShaderMacroArrayData*)((uintptr_t)macro_array + macro_array->mSize);

    return (const ResShaderVariationArrayData*)macro_array;
}

ResShaderSymbolArray ResShaderProgram::getResShaderSymbolArray(ShaderSymbolType type) const
{
    const ResShaderSymbolArrayData* symbol_array;
    {
        const ResShaderVariationArrayData* const data = getResShaderVariationDefaultArray().ptr();
        symbol_array = (const ResShaderSymbolArrayData*)((uintptr_t)data + data->mSize);
    }

    for (s32 i = 0; i < type; i++)
        symbol_array = (const ResShaderSymbolArrayData*)((uintptr_t)symbol_array + symbol_array->mSize);

    return symbol_array;
}

ResShaderSymbolArray ResBinaryShaderProgram::getResShaderSymbolArray(ShaderSymbolType type) const
{
    const ResShaderSymbolArrayData* symbol_array;
    {
        const ResShaderVariationArrayData* const data = getResShaderVariationDefaultArray().ptr();
        symbol_array = (const ResShaderSymbolArrayData*)((uintptr_t)data + data->mSize);
    }

    for (s32 i = 0; i < type; i++)
        symbol_array = (const ResShaderSymbolArrayData*)((uintptr_t)symbol_array + symbol_array->mSize);

    return symbol_array;
}

bool ResShaderArchive::setUp()
{
    if (modifyEndian())
    {
        ModifyEndianU32(true, ptr(), sizeof(DataType));

        ResShaderProgramArray prog_arr = getResShaderProgramArray();
        prog_arr.modifyEndianArray(modifyEndian());

        ResShaderSourceArray source_arr = getResShaderSourceArray();
        source_arr.modifyEndianArray(modifyEndian());

        for (ResShaderProgramArray::iterator it = prog_arr.begin(), it_end = prog_arr.end(); it != it_end; ++it)
        {
            ResShaderProgram prog(&(*it));

            for (s32 type = 0; type < cShaderType_Num; type++)
                prog.getResShaderMacroArray(ShaderType(type)).modifyEndianArray(modifyEndian());

            prog.getResShaderVariationArray().modifyEndianArray(modifyEndian());
            prog.getResShaderVariationDefaultArray().modifyEndianArray(modifyEndian());

            for (s32 type = 0; type < cShaderSymbolType_Num; type++)
                modifyEndianResSymbolArray(modifyEndian(), prog.getResShaderSymbolArray(ShaderSymbolType(type)), ShaderSymbolType(type));
        }

        ref().mEndian = 1 - ref().mEndian;
    }

    return true;
}

bool ResBinaryShaderArchive::setUp(bool le_resolve_pointers)
{
    if (modifyEndian())
        ModifyEndianU32(true, ptr(), sizeof(DataType));

    if (!modifyEndian())
    {
        if (ref().mResolved == 0)
        {
            for (ResShaderBinaryArray::iterator it = getResShaderBinaryArray().begin(), it_end = getResShaderBinaryArray().end(); it != it_end; ++it)
                ResShaderBinary(&(*it)).setUp();

            ref().mResolved = 1;
        }
    }
    else
    {
        ResShaderBinaryArray binary_arr = getResShaderBinaryArray();
        binary_arr.modifyEndianArray(modifyEndian());

        ResBinaryShaderProgramArray binary_prog_arr = getResBinaryShaderProgramArray();
        binary_prog_arr.modifyEndianArray(modifyEndian());

        for (ResBinaryShaderProgramArray::iterator it = binary_prog_arr.begin(), it_end = binary_prog_arr.end(); it != it_end; ++it)
        {
            ResBinaryShaderProgram binary_prog(&(*it));

            binary_prog.getResShaderVariationArray().modifyEndianArray(modifyEndian());
            binary_prog.getResShaderVariationDefaultArray().modifyEndianArray(modifyEndian());

            for (s32 type = 0; type < cShaderSymbolType_Num; type++)
                modifyEndianResSymbolArray(modifyEndian(), binary_prog.getResShaderSymbolArray(ShaderSymbolType(type)), ShaderSymbolType(type));
        }

        for (ResShaderBinaryArray::iterator it = binary_arr.begin(), it_end = binary_arr.end(); it != it_end; ++it)
        {
            ResShaderBinary binary(&(*it));
            binary.modifyBinaryEndian();

            if (le_resolve_pointers && ref().mResolved == 0)
                binary.setUp();
        }

        if (le_resolve_pointers)
            ref().mResolved = 1;

        ref().mEndian = 1 - ref().mEndian;
    }

    return true;
}

}
