#pragma once

#include <prim/seadMemUtil.h>

namespace sead {

template <typename CharType>
inline s32
SafeStringBase<CharType>::calcLength() const
{
    // SEAD_ASSERT(mStringTop);

    assureTerminationImpl_();
    s32 length = 0;

    for (;;)
    {
        if (length > cMaximumLength || mStringTop[length] == 0)
            break;

        length++;
    }

    if (length > cMaximumLength)
    {
        // SEAD_ASSERT_MSG(false, "too long string\n");
        return 0;
    }

    return length;
}

template <typename CharType>
inline const SafeStringBase<CharType>
SafeStringBase<CharType>::getPart(s32 at) const
{
    s32 len = calcLength();
    if (at < 0 || at > len)
        return cEmptyString;

    return SafeStringBase<CharType>(mStringTop + at);
}

template <typename CharType>
inline bool
SafeStringBase<CharType>::isEqual(const SafeStringBase<CharType>& rhs) const
{
    assureTerminationImpl_();
    if (cstr() == rhs.cstr())
        return true;

    for (s32 i = 0; i <= cMaximumLength; i++)
    {
        if (unsafeAt_(i) != rhs.unsafeAt_(i))
            return false;

        else if (unsafeAt_(i) == 0)
            return true;
    }

    // SEAD_ASSERT_MSG(false, "too long string\n");
    return false;
}

template <typename CharType>
inline s32
SafeStringBase<CharType>::comparen(const SafeStringBase<CharType>& rhs, s32 n) const
{
    assureTerminationImpl_();
    if (cstr() == rhs.cstr())
        return 0;

    if (n > cMaximumLength)
    {
        //SEAD_ASSERT_MSG(false, "paramater(%d) out of bounds [0, %d]", n, cMaximumLength);
        n = cMaximumLength;
    }

    for (s32 i = 0; i < n; i++)
    {
        if (unsafeAt_(i) == 0 && rhs.unsafeAt_(i) == 0)
            return 0;

        else if (unsafeAt_(i) == 0)
            return -1;

        else if (rhs.unsafeAt_(i) == 0)
            return 1;

        else if (unsafeAt_(i) < rhs.unsafeAt_(i))
            return -1;

        else if (unsafeAt_(i) > rhs.unsafeAt_(i))
            return 1;
    }

    return 0;
}

template <typename CharType>
inline s32
SafeStringBase<CharType>::findIndex(const SafeStringBase<CharType>& token) const
{
    s32 len = calcLength();
    s32 token_len = token.calcLength();

    for (s32 i = 0; i <= len - token_len; i++)
        if (SafeStringBase<CharType>(&mStringTop[i]).comparen(token, token_len) == 0)
            return i;

    return -1;
}

template <typename CharType>
inline s32
BufferedSafeStringBase<CharType>::copy(const SafeStringBase<CharType>& rhs, s32 size)
{
    CharType* mutable_string_top = getMutableStringTop_();
    if (size < 0)
        size = rhs.calcLength();

    if (size >= getBufferSize())
    {
        //SEAD_ASSERT_MSG(false, "Buffer overflow. (Buffer Size: %d, Copy Size: %d)", getBufferSize(), size);
        size = getBufferSize() - 1;
    }

    MemUtil::copy(mutable_string_top, rhs.cstr(), size * sizeof(CharType));
    mutable_string_top[size] = 0;

    return size;
}

template <typename CharType>
inline s32
BufferedSafeStringBase<CharType>::copyAt(s32 at, const SafeStringBase<CharType>& src, s32 cpy_length)
{
    CharType* mutable_string_top = getMutableStringTop_();
    s32 len = calcLength();

    if (at < 0)
    {
        at = len + at + 1;
        if (at < 0)
        {
            //SEAD_ASSERT_MSG(false, "at(%d) out of range[0, %d]", at, len);
            at = 0;
        }
    }

    if (cpy_length < 0)
        cpy_length = src.calcLength();

    if (cpy_length >= getBufferSize() - at)
    {
        //SEAD_ASSERT_MSG(false, "Buffer overflow. (Buffer Size: %d, At: %d, Copy Length: %d)", getBufferSize(), at, cpy_length);
        cpy_length = getBufferSize() - at - 1;
    }

    if (cpy_length <= 0)
        return 0;

    MemUtil::copy(mutable_string_top + at, src.cstr(), cpy_length * sizeof(CharType));
    if (at + cpy_length > len)
        mutable_string_top[at + cpy_length] = 0;

    return cpy_length;
}

template <typename CharType>
inline s32
BufferedSafeStringBase<CharType>::trim(s32 trim_length)
{
    if (trim_length >= getBufferSize())
    {
        //SEAD_ASSERT_MSG(false, "trim_length(%d) out of bounds. [0,%d) \n", trim_length, getBufferSize());
        return calcLength();
    }

    if (trim_length < 0)
    {
        //SEAD_ASSERT_MSG(false, "trim_length(%d) out of bounds. [0,%d) \n", trim_length, getBufferSize());
        trim_length = 0;
    }

    CharType* mutable_string_top = getMutableStringTop_();
    mutable_string_top[trim_length] = 0;

    return trim_length;
}

} // namespace sead
