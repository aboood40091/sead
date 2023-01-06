#pragma once

#include <prim/seadSafeString.h>

namespace agl { namespace detail {

class ShaderTextUtil
{
public:
    static s32 findLineFeedCode(const char* p_string, s32* p_length);
    static void replaceMacro(sead::BufferedSafeString* p_text, const char* const* macro, const char* const* value, s32 macro_num, char* p_work, s32 worksize);
    static void replace(char* pSrc, const char* pValue, s32 begin, s32 end, void* pWork, s32 size);
    static sead::HeapSafeString* createRawText(const sead::SafeString& text, const char* const* source_name, const char* const* source_text, s32 source_num, bool* source_used, sead::Heap* heap);
};

} }
