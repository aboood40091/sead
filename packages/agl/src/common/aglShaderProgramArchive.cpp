#include <common/aglShaderProgramArchive.h>
#include <detail/aglPrivateResource.h>
#include <detail/aglRootNode.h>
#include <detail/aglShaderTextUtil.h>
#include <time/seadTickTime.h>

#ifdef cafe
#include <cafe.h>
#endif // cafe

namespace {

void createShaderProgram(
    agl::ShaderProgram* program,
    const sead::SafeString& name,
    const agl::ResShaderVariationArray& variation_macros,
    const agl::ResShaderVariationArray& variation_macro_defaults,
    const agl::ResShaderSymbolArray& attributes,
    const agl::ResShaderSymbolArray& uniforms,
    const agl::ResShaderSymbolArray& uniform_blocks,
    const agl::ResShaderSymbolArray& samplers,
    sead::Heap* heap
)
{
    program->initialize(name, heap);

    program->setResShaderVariationDefaultArray(variation_macro_defaults);
    program->setResShaderSymbolArray(agl::cShaderSymbolType_Attribute, attributes);
    program->setResShaderSymbolArray(agl::cShaderSymbolType_Uniform, uniforms);
    program->setResShaderSymbolArray(agl::cShaderSymbolType_UniformBlock, uniform_blocks);
    program->setResShaderSymbolArray(agl::cShaderSymbolType_Sampler, samplers);

    u32 num_variation_macro = variation_macros.getNum();
    if (num_variation_macro == 0)
        return;

    program->createVariationBuffer(num_variation_macro, heap);

    for (agl::ResShaderVariationArray::constIterator it = variation_macros.begin(), it_end = variation_macros.end(); it != it_end; ++it)
    {
        const agl::ResShaderVariation variation(&(*it));

        program->createVariationMacro(
            it.getIndex(),
            variation.getName(),
            variation.getID(),
            variation.ref().mValueNum,
            heap
        );

        for (s32 i = 0; i < variation.ref().mValueNum; i++)
        {
            program->setVariationMacroValue(
                it.getIndex(),
                i,
                variation.getValue(i)
            );
        }
    }

    program->createVariation(heap);
}

}

namespace agl {

ShaderProgramArchive::ShaderProgramArchive()
    : sead::IDisposer()
    , mResBinary()
    , mResText()
    , mProgram()
    , _20(0)
    , _24(0)
    , _28(0)
    , mFlag()
    , mProgramEx()
    , mSource()
    , mSourceText()
    , mSourceName()
{
    detail::RootNode::setNodeMeta(this, "Icon = LAYOUT, Security = agl_shader");
}

ShaderProgramArchive::~ShaderProgramArchive()
{
    destroy();
}

void ShaderProgramArchive::destroy()
{
    destroyResFile_();
    mProgram.freeBuffer();
    mResBinary = nullptr;
    _28 = 0;
}

void ShaderProgramArchive::destroyResFile_()
{
    if (mResText.isValid())
    {
        mProgramEx.freeBuffer();
        mSourceText.freeBuffer();
        mSourceName.freeBuffer();
        mSource.freeBuffer();
    }
    mResText = nullptr;
}

void ShaderProgramArchive::createWithOption(ResBinaryShaderArchive res_binary_archive, ResShaderArchive res_archive, u32 flag, sead::Heap* heap)
{
    mResBinary = res_binary_archive;
    // SEAD_ASSERT_MSG(uintptr_t(mResBinary.ptr()) % cShaderArchiveAlignment == 0, "agl::ResBinaryShaderArchive must be aligned agl::cShaderArchiveAlignment byte.");

    if (flag & 2)
        mFlag.set(1);

    if (mResBinary.isValid())
    {
        mResBinary.setUp(true);

        mProgram.allocBuffer(mResBinary.getResBinaryShaderProgramNum(), heap);

        bool create_dl = !(flag & 1);

        u8* dl_buf = nullptr;
        u32 cur_dl_offs = 0;
        const u32 dl_size = 0x220;

        if (create_dl)
        {
            u32 dl_buf_size = mResBinary.getResShaderBinaryNum() * dl_size;
            dl_buf = new (heap, DisplayList::cDisplayListAlignment) u8[dl_buf_size];
#ifdef cafe
            DCFlushRange(dl_buf, dl_buf_size);
#endif // cafe
        }

        const ResBinaryShaderProgramArray binary_prog_arr = mResBinary.getResBinaryShaderProgramArray();
        u32 verify_binary_num = 0;

        for (ResBinaryShaderProgramArray::constIterator it = binary_prog_arr.begin(), it_end = binary_prog_arr.end(); it != it_end; ++it)
        {
            const ResBinaryShaderProgram binary_prog(&(*it));

            ShaderProgram& program = mProgram[it.getIndex()];

            createShaderProgram(
                &program,
                binary_prog.getName(),
                binary_prog.getResShaderVariationArray(),
                binary_prog.getResShaderVariationDefaultArray(),
                binary_prog.getResShaderSymbolArray(cShaderSymbolType_Attribute),
                binary_prog.getResShaderSymbolArray(cShaderSymbolType_Uniform),
                binary_prog.getResShaderSymbolArray(cShaderSymbolType_UniformBlock),
                binary_prog.getResShaderSymbolArray(cShaderSymbolType_Sampler),
                heap
            );

            for (s32 i = 0; i < program.getVariationNum(); i++)
            {
                ShaderProgram* var_program = program.getVariation(i);

                for (s32 type = 0; type < cShaderType_Num; type++)
                {
                    Shader* shader = var_program->getShader(ShaderType(type));
                    if (binary_prog.ref().mKind & 1 << type)
                    {
                        u32 num_shader = 3;
                        if ((binary_prog.ref().mKind >> cShaderType_Geometry & 1) == 0)
                            num_shader = 2;

                        u32 index = binary_prog.ref().mBaseIndex + type + num_shader * i;
                        if (index != 0xFFFFFFFF)
                        {
                            const ResShaderBinary binary = mResBinary.getResShaderBinaryArray().get(index);
                            // SEAD_ASSERT(binary.isValid());
                            // SEAD_ASSERT(binary.getShaderType() == type);
#ifdef cafe
                            DCFlushRangeNoSync(binary.getData(), binary.ref().mDataSize);
#endif // cafe
                            shader->setBinary(binary.getData());

                            // SEAD_ASSERT(verify_binary_num == index);
                            verify_binary_num++;
                        }
                    }
                }

                if (create_dl)
                {
                    var_program->getDisplayList().setBuffer(dl_buf + cur_dl_offs, dl_size);
                    cur_dl_offs += dl_size;
                }
            }
        }

        // SEAD_ASSERT(verify_binary_num == mResBinary.getResShaderBinaryArray().getNum());
    }

    setResShaderArchive_(res_archive, heap);

    for (sead::Buffer<ShaderProgram>::iterator it = mProgram.begin(), it_end = mProgram.end(); it != it_end; ++it)
        it->reserveSetUpAllVariation();

    if (mProgram.size() > 0)
        return;

    // SEAD_ASSERT_MSG(false, "No shader program.");
}

bool ShaderProgramArchive::setUp()
{
    updateCompileInfo();
    return setUp_(mFlag.isOn(1));
}

s32 ShaderProgramArchive::searchShaderProgramIndex(const sead::SafeString& name) const
{
    for (sead::Buffer<ShaderProgram>::constIterator it = mProgram.begin(), it_end = mProgram.end(); it != it_end; ++it)
        if (it->getName().isEqual(name))
            return it.getIndex();

    return -1;
}

void ShaderProgramArchive::updateCompileInfo()
{
    sead::TickTime time;

    for (sead::Buffer<ShaderSource>::iterator it = mSource.begin(), it_end = mSource.end(); it != it_end; ++it)
        if (it->mFlag.isOn(1))
            it->expand();

    for (sead::Buffer<ShaderProgramEx>::iterator it = mProgramEx.begin(), it_end = mProgramEx.end(); it != it_end; ++it)
        it->updateRawText();

    for (sead::Buffer<ShaderSource>::iterator it = mSource.begin(), it_end = mSource.end(); it != it_end; ++it)
        it->mFlag.reset(1);
}

void ShaderProgramArchive::setResShaderArchive_(ResShaderArchive res_archive, sead::Heap* heap)
{
    destroyResFile_();

    if (!res_archive.isValid())
        return;

    if (!detail::PrivateResource::instance()->getShaderCompileHeap())
        return;

    mResText = res_archive;
    mResText.setUp();

    bool do_init_programs = false;

    if (!mResBinary.isValid())
    {
        mProgram.allocBuffer(mResText.getResShaderProgramNum(), heap);
        do_init_programs = true;
    }
    else
    {
        // SEAD_ASSERT(mResText.getResShaderProgramNum() == mResBinary.getResBinaryShaderProgramNum());
    }

    mSource.allocBuffer(mResText.getResShaderSourceNum(), heap);

    mSourceText.allocBuffer(mSource.size(), heap);
    mSourceName.allocBuffer(mSource.size(), heap);

    mProgramEx.allocBuffer(mProgram.size(), heap);

    sead::UnsafeArray<bool, 1024> source_is_used;

    const ResShaderSourceArray source_arr = mResText.getResShaderSourceArray();
    const ResShaderProgramArray prog_arr = mResText.getResShaderProgramArray();

    for (ResShaderSourceArray::constIterator it = source_arr.begin(), it_end = source_arr.end(); it != it_end; ++it)
    {
        const ResShaderSource source(&(*it));
        source_is_used[it.getIndex()] = false;

        for (ResShaderProgramArray::constIterator it_prog = prog_arr.begin(), it_prog_end = prog_arr.end(); it_prog != it_prog_end; ++it_prog)
        {
            const ResShaderProgram prog(&(*it_prog));
            bool found = false;

            for (s32 type = 0; type < cShaderType_Num; type++)
            {
                if (prog.ref().mSourceIndex[type] == it.getIndex())
                {
                    source_is_used[it.getIndex()] = true;
                    found = true;
                    break;
                }
            }

            if (found)
                break;
        }
    }

    for (ResShaderSourceArray::constIterator it = source_arr.begin(), it_end = source_arr.end(); it != it_end; ++it)
    {
        const ResShaderSource source(&(*it));
        mSource[it.getIndex()].initialize(this, it.getIndex(), source, source_is_used[it.getIndex()], heap);
    }

    sead::TickTime time;

    for (ResShaderProgramArray::constIterator it = prog_arr.begin(), it_end = prog_arr.end(); it != it_end; ++it)
    {
        const ResShaderProgram prog(&(*it));

        if (do_init_programs)
        {
            ShaderProgram& program = mProgram[it.getIndex()];

            createShaderProgram(
                &program,
                prog.getName(),
                prog.getResShaderVariationArray(),
                prog.getResShaderVariationDefaultArray(),
                prog.getResShaderSymbolArray(cShaderSymbolType_Attribute),
                prog.getResShaderSymbolArray(cShaderSymbolType_Uniform),
                prog.getResShaderSymbolArray(cShaderSymbolType_UniformBlock),
                prog.getResShaderSymbolArray(cShaderSymbolType_Sampler),
                heap
            );
        }

        mProgramEx[it.getIndex()].initialize(this, it.getIndex(), prog, heap);
    }

    updateCompileInfo();
}

bool ShaderProgramArchive::setUp_(bool unk)
{
    _28 += 1;
    sead::TickTime time;

    for (sead::Buffer<ShaderProgramEx>::iterator it = mProgramEx.begin(), it_end = mProgramEx.end(); it != it_end; ++it)
        it->updateAnalyze();

    for (sead::Buffer<ShaderProgram>::iterator it = mProgram.begin(), it_end = mProgram.end(); it != it_end; ++it)
    {
        // TODO
        // it->mpSharedData->_10 = _20;

        if ((!unk || _28 <= 1) && it->setUpAllVariation() != 0)
            return false;
    }

    (void)time.diffToNow();

    // TODO
    // if (_24)
    //     _24->vf0C(this);

    return true;
}

ShaderProgramArchive::ShaderProgramEx::ShaderProgramEx()
    : mIndex(0)
    , mpArchive(nullptr)
    , mVariationIndex(0)
    , mFlag(0)
{
}

ShaderProgramArchive::ShaderProgramEx::~ShaderProgramEx()
{
}

void ShaderProgramArchive::ShaderProgramEx::initialize(ShaderProgramArchive* archive, s32 index, ResShaderProgram res, sead::Heap* heap)
{
    mIndex = index;
    mpArchive = archive;

    ShaderProgram& program = archive->mProgram[index];

    for (sead::UnsafeArray<ShaderCompileInfoEx, cShaderType_Num>::iterator it = mCompileInfoEx.begin(), it_end = mCompileInfoEx.end(); it != it_end; ++it)
    {
        s32 source_index = res.ref().mSourceIndex[it.getIndex()];
        ShaderType type = ShaderType(it.getIndex());

        if (source_index != -1)
        {
            it->mSource = &mpArchive->mSource[source_index];

            const ResShaderMacroArray macro_arr = res.getResShaderMacroArray(type);
            it->mCompileInfo.create(macro_arr.getNum(), program.getVariationMacroNum(), heap);

            it->mCompileInfo.setName(it->mSource->getName());

            for (ResShaderMacroArray::constIterator macro_it = macro_arr.begin(), macro_it_end = macro_arr.end(); macro_it != macro_it_end; ++macro_it)
            {
                const ResShaderMacro macro(&(*macro_it));
                it->mCompileInfo.pushBackMacro(macro.getName(), macro.getValue());
            }

            program.getShader(type)->setCompileInfo(&it->mCompileInfo);
        }
        else
        {
            it->mSource = nullptr;
        }
    }

    if (program.getVariationMacroNum() > 0)
    {
        _110.allocBuffer(program.getVariationMacroNum(), heap);
        for (sead::Buffer<u32>::iterator it = _110.begin(), it_end = _110.end(); it != it_end; ++it)
            *it = 0;
    }

    detail::RootNode::setNodeMeta(this, "Icon = CIRCLE_ORENGE");
}

void ShaderProgramArchive::ShaderProgramEx::updateRawText()
{
    for (sead::UnsafeArray<ShaderCompileInfoEx, cShaderType_Num>::iterator it = mCompileInfoEx.begin(), it_end = mCompileInfoEx.end(); it != it_end; ++it)
    {
        ShaderSource* source = it->mSource;
        if (source && source->mFlag.isOn(1))
        {
            it->mCompileInfo.setSourceText(source->mRawText);
            mpArchive->getShaderProgram(mIndex).reserveSetUpAllVariation();
        }
    }
}

void ShaderProgramArchive::ShaderProgramEx::updateAnalyze()
{
    if (mFlag.isOn(1))
        mpArchive->mProgram[mIndex].updateVariation(mVariationIndex);
}

ShaderProgramArchive::ShaderSource::ShaderSource()
    : IDisposer()
    , mFlag(1)
    , mpArchive(nullptr)
    , mRes()
    , mText(nullptr)
    , mRawText(nullptr)
    , mUsedInSource()
{
}

ShaderProgramArchive::ShaderSource::~ShaderSource()
{
    delete mRawText;
    delete mText;
    mUsedInSource.freeBuffer();
}

void ShaderProgramArchive::ShaderSource::initialize(ShaderProgramArchive* archive, s32 index, ResShaderSource res, bool is_used, sead::Heap* heap)
{
    mpArchive = archive;
    mRes = res;
    mIndex = index;

    mFlag.change(1 << 1, is_used);

    mText = new sead::HeapSafeString(detail::PrivateResource::instance()->getShaderCompileHeap(), res.getText(), res.ref().mTextLen * 2);

    mpArchive->mSourceName[mIndex] = mRes.getName();
    mpArchive->mSourceText[mIndex] = mText->cstr();

    mUsedInSource.allocBuffer(mpArchive->mSource.size(), heap);
    for (sead::Buffer<bool>::iterator it = mUsedInSource.begin(), it_end = mUsedInSource.end(); it != it_end; ++it)
        *it = false;

    detail::RootNode::setNodeMeta(this, "Icon = NOTE");
}

void ShaderProgramArchive::ShaderSource::expand()
{
    if (mFlag.isOff(1 << 1))
        return;

    if (mRawText)
    {
        delete mRawText;
        mRawText = nullptr;
    }

    // SEAD_ASSERT(mpArchive->mSource.size() < 1024);
    bool source_is_used[1024];

    mRawText = detail::ShaderTextUtil::createRawText(
        *mText,
        mpArchive->mSourceName.getBufferPtr(),
        mpArchive->mSourceText.getBufferPtr(),
        mpArchive->mSource.size(),
        source_is_used,
        detail::PrivateResource::instance()->getShaderCompileHeap()
    );

    for (sead::Buffer<ShaderSource>::iterator it = mpArchive->mSource.begin(), it_end = mpArchive->mSource.end(); it != it_end; ++it)
        it->mUsedInSource[mIndex] = source_is_used[it.getIndex()];
}

}
