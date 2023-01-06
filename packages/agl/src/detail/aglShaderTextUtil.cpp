#include <basis/seadNew.h>
#include <detail/aglShaderTextUtil.h>
#include <prim/seadMemUtil.h>
#include <prim/seadSafeString.h>

// TODO: Custom implementations of std::strchr && std::strspn
#include <cstring>

namespace agl { namespace detail {

s32 ShaderTextUtil::findLineFeedCode(const char* p_string, s32* p_length)
{
    // SEAD_ASSERT(p_string != nullptr);

    s32 length;

    s32 i = 0;
    for (;;)
    {
        if (p_string[i] == '\0')
        {
            length = 0;
            break;
        }
        else if (p_string[i] == '\r')
        {
            length = (p_string[i + 1] == '\n') ? 2 : 1;
            break;
        }
        else if (p_string[i] == '\n')
        {
            length = 1;
            break;
        }

        i++;
    }

    if (length != 0)
    {
        if (p_length)
            *p_length = length;

        return i;
    }

    return -1;
}

void ShaderTextUtil::replaceMacro(sead::BufferedSafeString* p_text, const char* const* macro, const char* const* value, s32 macro_num, char* p_work, s32 worksize)
{
    // SEAD_ASSERT(p_text != nullptr);
    // SEAD_ASSERT(p_work != nullptr);
    // SEAD_ASSERT(macro != nullptr);
    // SEAD_ASSERT(value != nullptr);

    // SEAD_ASSERT(macro_num < 1024);
    bool macro_replaced[1024];
    for (s32 i_macro = 0; i_macro < macro_num; i_macro++)
        macro_replaced[i_macro] = false;

    const char* p_src = p_text->cstr();
    char* p_dst = p_work;

    s32 i_macro = 0;
    for (;;)
    {
        s32 line_feed_pos, line_feed_len;
        line_feed_pos = detail::ShaderTextUtil::findLineFeedCode(p_src, &line_feed_len);
        if (line_feed_pos == -1)
            break;

        bool replaced = false;

        if (*p_src == '#')
        {
            s32 macro_pos = 1;
            while (p_src[macro_pos] != '\0')
            {
                if (p_src[macro_pos] != ' ' &&
                    p_src[macro_pos] != '\t' &&
                    p_src[macro_pos] != '\r' &&
                    p_src[macro_pos] != '\n')
                {
                    break;
                }
                macro_pos++;
            }

            if (p_src[macro_pos + 0] == 'd' &&
                p_src[macro_pos + 1] == 'e' &&
                p_src[macro_pos + 2] == 'f' &&
                p_src[macro_pos + 3] == 'i' &&
                p_src[macro_pos + 4] == 'n' &&
                p_src[macro_pos + 5] == 'e' &&
                (p_src[macro_pos + 6] == ' ' ||
                 p_src[macro_pos + 6] == '\t'))
            {
                macro_pos += 7;
                while (p_src[macro_pos] != '\0')
                {
                    if (p_src[macro_pos] != ' ' &&
                        p_src[macro_pos] != '\t' &&
                        p_src[macro_pos] != '\r' &&
                        p_src[macro_pos] != '\n')
                    {
                        break;
                    }
                    macro_pos++;
                }

                for (s32 i_match_macro = 0; i_match_macro < macro_num; i_match_macro++)
                {
                    if (macro_replaced[i_match_macro])
                        continue;

                    const char* const match_macro = macro[i_match_macro];
                    bool match = true;
                    u32 save_macro_pos = macro_pos;

                    for (s32 i = 0; match_macro[i] != '\0'; i++)
                    {
                        if (p_src[macro_pos] != match_macro[i])
                        {
                            match = false;
                            break;
                        }

                        macro_pos++;
                    }

                    if (!match || (p_src[macro_pos] != ' ' &&
                                   p_src[macro_pos] != '\t'))
                    {
                        macro_pos = save_macro_pos;
                        continue;
                    }

                    p_dst += sead::BufferedSafeString(p_dst, worksize - (s32)(p_dst - p_work))
                                .format("#define %s %s", match_macro, value[i_match_macro]);

                    for (s32 i = 0; i < line_feed_len; i++)
                    {
                        p_dst += sead::BufferedSafeString(p_dst, worksize - (s32)(p_dst - p_work))
                                    .append(p_src[line_feed_pos + i]);
                    }

                    macro_replaced[i_match_macro] = true;
                    replaced = true;
                    break;
                }
            }
        }

        if (!replaced)
        {
            sead::MemUtil::copy(p_dst, p_src, line_feed_pos + line_feed_len);
            p_dst += line_feed_pos + line_feed_len;
            *p_dst = '\0';
        }

        p_src += line_feed_pos + line_feed_len;

        if (replaced)
        {
            i_macro++;
            if (i_macro == macro_num)
                break;
        }
    }

    while (*p_src != '\0')
        *p_dst++ = *p_src++;

    // SEAD_ASSERT(static_cast<int>(p_dst - p_work) <= worksize);
    *p_dst = '\0';

    p_text->copy(sead::SafeString(p_work));
}

void ShaderTextUtil::replace(char* pSrc, const char* pValue, s32 begin, s32 end, void* pWork, s32 size)
{
    // SEAD_ASSERT(pSrc != nullptr);
    // SEAD_ASSERT(pWork != nullptr);
    char* const work = (char*)pWork;

    s32 i_count = 0;
    while (pSrc[end + i_count] != '\0')
    {
        work[i_count] = pSrc[end + i_count];
        i_count++;
    }
    // SEAD_ASSERT(i_count < size);
    work[i_count] = '\0';

    s32 i = 0;
    while (pValue[i] != '\0')
    {
        pSrc[begin + i] = pValue[i];
        i++;
    }

    s32 j = 0;
    while (work[j] != '\0')
    {
        pSrc[begin + i + j] = work[j];
        j++;
    }

    pSrc[begin + i + j] = '\0';
}

sead::HeapSafeString* ShaderTextUtil::createRawText(const sead::SafeString& text, const char* const* source_name, const char* const* source_text, s32 source_num, bool* source_used, sead::Heap* heap)
{
    if (source_used != nullptr)
        for (s32 i_source = 0; i_source < source_num; i_source++)
            source_used[i_source] = false;

    sead::HeapSafeString* p_text = new (heap) sead::HeapSafeString(heap, text);
    s32 text_len = p_text->calcLength();

    const char* p_src = p_text->cstr(); // r22 = p_src, r28 = p_text

    while (*p_src != '\0')
    {
        const char* const include_directive_begin = std::strchr(p_src, '#') + 1;
        if (include_directive_begin - 1 == nullptr)
            break;

        const char* directive = include_directive_begin + std::strspn(include_directive_begin, " \t\r\n");

        if (directive[0] == 'i' &&
            directive[1] == 'n' &&
            directive[2] == 'c' &&
            directive[3] == 'l' &&
            directive[4] == 'u' &&
            directive[5] == 'd' &&
            directive[6] == 'e')
        {
            const char* const include_name_begin = std::strchr(directive + 7, '\"') + 1;
            if (include_name_begin - 1 == nullptr)
                continue;

            const char* const include_directive_end = std::strchr(include_name_begin, '\"') + 1;
            if (include_directive_end - 1 == nullptr)
                continue;

            sead::FixedSafeString<1024> name;
            name.copy(include_name_begin, (s32)(include_directive_end - include_name_begin) - 1);

            s32 i_source = 0;
            {
                bool found = false;
                while (i_source < source_num)
                {
                    if (name.isEqual(source_name[i_source]))
                    {
                        found = true;
                        break;
                    }
                    i_source++;
                }
                if (!found)
                    return p_text;
            }

            const char* p_source_text = source_text[i_source];
            if (source_used != nullptr)
                source_used[i_source] = true;

            if (!p_source_text)
                break;

            sead::HeapSafeString* p_new_text = new (heap) sead::HeapSafeString(heap, *p_text, text_len + sead::SafeString(p_source_text).calcLength() + 1);

            const char* const text_base = p_text->cstr();
            u8* temp_buf = new (heap) u8[text_len + 1];
            detail::ShaderTextUtil::replace((char*)p_new_text->cstr(), p_source_text, (s32)(include_directive_begin - text_base) - 1, (s32)(include_directive_end - text_base), temp_buf, text_len + 1);
            delete temp_buf; // Nintendo did not use delete[] (fixed in later versions)

            delete p_text;
            p_text = new (heap) sead::HeapSafeString(heap, *p_new_text);
            delete p_new_text;

            p_src = p_text->cstr();
            text_len = p_text->calcLength();
        }
        else
        {
            p_src = directive;
        }
    }

    return p_text;
}

} }
