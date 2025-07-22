#ifndef SEAD_SAFE_STRING_H_
#define SEAD_SAFE_STRING_H_

#include <stdarg.h>

#include <basis/seadTypes.h>

namespace sead {

template <typename CharType>
class BufferedSafeStringBase;

template <typename CharType>
class SafeStringBase
{
public:
    static const CharType cNullChar;
    static const CharType cLineBreakChar;
    static const SafeStringBase<CharType> cEmptyString;
    static const s32 cMaximumLength = 256*1024;

public:
    SafeStringBase()
        : mStringTop(&cNullChar)
    {
    }

    SafeStringBase(const CharType* str)
        : mStringTop(str)
    {
        //SEAD_ASSERT_MSG(mStringTop, "str must not be nullptr.");
    }

    virtual ~SafeStringBase()
    {
    }

public:
    class iterator
    {
    public:
        explicit iterator(const SafeStringBase<CharType>* str)
            : mString(str)
            , mIndex(0)
        {
        }

        iterator(const SafeStringBase<CharType>* str, s32 idx)
            : mString(str)
            , mIndex(idx)
        {
        }

        virtual ~iterator()
        {
        }

    public:
        inline iterator& operator++()
        {
            ++mIndex;
            return *this;
        }

        inline iterator& operator--()
        {
            --mIndex;
            return *this;
        }

        friend bool operator==(const iterator& a, const iterator& b)
        {
            return a.mString == b.mString && a.mIndex == b.mIndex;
        };

        friend bool operator!=(const iterator& a_, const iterator& b_)
        {
            return !(a_ == b_);
        }

        const CharType& operator*() const
        {
            return mString->at(mIndex);
        }

        s32 getIndex() const
        {
            return mIndex;
        }

    protected:
        const SafeStringBase* mString;
        s32 mIndex;
    };

    class token_iterator : public iterator
    {
    public:
        token_iterator(const SafeStringBase* str, const SafeStringBase& delimiter)
            : iterator(str)
            , mDelimiter(delimiter)
        {
        }

        token_iterator(const SafeStringBase* str, s32 idx, const SafeStringBase& delimiter)
            : iterator(str, idx)
            , mDelimiter(delimiter)
        {
        }

        virtual ~token_iterator()
        {
        }

    public:
        inline token_iterator& operator++();
        inline token_iterator& operator--();

        inline s32 get(BufferedSafeStringBase<CharType>* out) const;
        inline s32 getAndForward(BufferedSafeStringBase<CharType>* out);

    private:
        SafeStringBase mDelimiter;
    };

public:
    iterator begin() const
    {
        return iterator(this, 0);
    };

    iterator end() const
    {
        return iterator(this, calcLength() + 1);
    };

public:
    token_iterator tokenBegin(const SafeStringBase<CharType>& delimiter) const
    {
        return token_iterator(this, 0, delimiter);
    };

    token_iterator tokenEnd(const SafeStringBase<CharType>& delimiter = SafeStringBase<CharType>()) const
    {
        return token_iterator(this, calcLength() + 1, delimiter);
    }

public:
    const CharType* cstr() const
    {
        assureTerminationImpl_();
        return mStringTop;
    }

    inline const CharType& at(s32 idx) const;

    inline const CharType& operator[](s32 idx) const
    {
        return at(idx);
    }

    inline s32 calcLength() const;
    inline const SafeStringBase<CharType> getPart(s32 at) const;
    inline const SafeStringBase<CharType> getPart(const iterator& it) const;
    inline bool include(const CharType& c) const;
    inline bool include(const SafeStringBase<CharType>& str) const;
    inline bool isEqual(const SafeStringBase<CharType>& rhs) const;

    friend bool operator==(const SafeStringBase<CharType>& lhs, const SafeStringBase<CharType>& rhs)
    {
        return lhs.isEqual(rhs);
    }

    friend bool operator!=(const SafeStringBase<CharType>& lhs, const SafeStringBase<CharType>& rhs)
    {
        return !lhs.isEqual(rhs);
    }

    inline s32 comparen(const SafeStringBase<CharType>& rhs, s32 n) const;

    inline s32 compare(const SafeStringBase<CharType>& rhs) const
    {
        return comparen(rhs, cMaximumLength);
    }

    friend bool operator>(const SafeStringBase<CharType>& lhs, const SafeStringBase<CharType>& rhs)
    {
        return lhs.compare(rhs) > 0;
    }

    friend bool operator<(const SafeStringBase<CharType>& lhs, const SafeStringBase<CharType>& rhs)
    {
        return lhs.compare(rhs) < 0;
    }

    inline iterator findIterator(const SafeStringBase<CharType>& token) const;
    s32 findIndex(const SafeStringBase<CharType>& token) const;
    inline iterator rfindIterator(const SafeStringBase<CharType>& token) const;
    s32 rfindIndex(const SafeStringBase<CharType>& token) const;

    bool isEmpty() const
    {
        return unsafeAt_(0) == 0;
    }

private:
    const CharType& unsafeAt_(s32 idx) const
    {
        return mStringTop[idx];
    }

protected:
    virtual void assureTerminationImpl_() const
    {
    }

protected:
    const CharType* mStringTop;
};

template <>
const char SafeStringBase<char>::cNullChar;

template <>
const char SafeStringBase<char>::cLineBreakChar;

template <>
const SafeStringBase<char> SafeStringBase<char>::cEmptyString;

template <>
const char16 SafeStringBase<char16>::cNullChar;

template <>
const char16 SafeStringBase<char16>::cLineBreakChar;

template <>
const SafeStringBase<char16> SafeStringBase<char16>::cEmptyString;

template <typename CharType>
class BufferedSafeStringBase : public SafeStringBase<CharType>
{
public:
    BufferedSafeStringBase(CharType* buffer, s32 size)
        : SafeStringBase<CharType>(buffer)
        , mBufferSize(size)
    {
        if (size <= 0)
        {
            //SEAD_ASSERT_MSG(false, "Invalied buffer size(%d).\n", size);
            mStringTop = nullptr;
            mBufferSize = 0;
            return;
        }

        assureTerminationImpl_();
    }

    BufferedSafeStringBase(BufferedSafeStringBase<CharType>* original, s32 pos)
        : SafeStringBase<CharType>(original->cstr())
        , mBufferSize(0)
    {
        //SEAD_ASSERT_MSG(original, "original string must not be nullptr.");

        if (pos >= original->getBufferSize() || pos < 0)
        {
            //SEAD_ASSERT_MSG(false, "pos(%d) out of bounds[0,%d)", pos, original->getBufferSize());
            mStringTop = nullptr;
            mBufferSize = 0;
            return;
        }

        mStringTop = original->mStringTop + pos;
        mBufferSize = original->getBufferSize() - pos;
        assureTerminationImpl_();
    }

private:
    BufferedSafeStringBase(const BufferedSafeStringBase<CharType>&);
    BufferedSafeStringBase<CharType>& operator=(const BufferedSafeStringBase<CharType>&);

public:
    virtual ~BufferedSafeStringBase()
    {
    }

    inline const CharType& operator[](s32 idx) const;

    s32 getBufferSize() const
    {
        return mBufferSize;
    }

    inline s32 copy(const SafeStringBase<CharType>& rhs, s32 size = -1);
    inline s32 copyAt(s32 at, const SafeStringBase<CharType>& src, s32 cpy_length = -1);

    s32 format(const CharType* format_string, ...);
    s32 formatV(const CharType* format_string, va_list varg);
    s32 appendWithFormat(const CharType* format_string, ...);
    s32 appendWithFormatV(const CharType* format_string, va_list varg);

    inline s32 append(const SafeStringBase<CharType>& src, s32 append_length = -1);
    inline s32 append(CharType src_chr);
    inline s32 chop(s32 chop_num = 1);
    inline s32 chopMatchedChar(CharType chop_char);
    inline s32 chopMatchedChar(const SafeStringBase<CharType>& chop_char_list);

    inline s32 chomp()
    {
        return chopMatchedChar(SafeStringBase<CharType>::cLineBreakChar);
    }

    inline s32 chopUnprintableAsciiChar();
    inline s32 rstrip(const SafeStringBase<CharType>& strip_char_list);
    inline s32 rstripUnprintableAsciiChars();
    inline s32 trim(s32 trim_length);
    inline s32 trimMatchedString(const SafeStringBase<CharType>& trim_string);

    inline void clear()
    {
        getMutableStringTop_()[0] = 0;
    }

    s32 convertFromMultiByteString(const SafeStringBase<CharType>& src, s32 src_size = -1)
    {
        return convertFromOtherType_(src, src_size);
    }

    s32 convertFromWideCharString(const SafeStringBase<char16>& src, s32 src_size = -1)
    {
        return convertFromOtherType_(src, src_size);
    }

    CharType* getBuffer()
    {
        assureTerminationImpl_();
        return getMutableStringTop_();
    }

private:
    CharType* getMutableStringTop_()
    {
        return const_cast<CharType*>(mStringTop);
    }

    template <typename Other>
    inline s32 convertFromOtherType_(const SafeStringBase<Other>& src, s32 src_size);

    inline bool isUnprintableChar_(CharType c) const;

    static s32 formatImpl_(CharType* dst, s32 dst_size, const CharType* format_string, va_list varg);

protected:
    virtual void assureTerminationImpl_() const
    {
        BufferedSafeStringBase<CharType>* mutable_ptr = const_cast<BufferedSafeStringBase<CharType>*>(this);
        mutable_ptr->getMutableStringTop_()[getBufferSize() - 1] = 0;
    }

private:
    s32 mBufferSize;
};

template <typename CharType, s32 N>
class FixedSafeStringBase : public BufferedSafeStringBase<CharType>
{
public:
    FixedSafeStringBase()
        : BufferedSafeStringBase<CharType>(mBuffer, N)
    {
        clear();
    }

    explicit FixedSafeStringBase(const SafeStringBase<CharType>& rhs)
        : BufferedSafeStringBase<CharType>(mBuffer, N)
    {
        copy(rhs);
    }

    FixedSafeStringBase(const FixedSafeStringBase<CharType, N>& rhs)
        : BufferedSafeStringBase<CharType>(mBuffer, N)
    {
        copy(rhs);
    }

    virtual ~FixedSafeStringBase()
    {
    }

    FixedSafeStringBase<CharType, N>& operator=(const FixedSafeStringBase<CharType, N>& rhs)
    {
        copy(rhs);
        return *this;
    }

    FixedSafeStringBase<CharType, N>& operator=(const SafeStringBase<CharType>& rhs)
    {
        copy(rhs);
        return *this;
    }

private:
    CharType mBuffer[N];
};

typedef SafeStringBase<char> SafeString;
typedef SafeStringBase<char16> WSafeString;
typedef BufferedSafeStringBase<char> BufferedSafeString;
typedef BufferedSafeStringBase<char16> WBufferedSafeString;

#ifdef cafe
static_assert(sizeof(SafeString) == 8, "sead::SafeStringBase<T> size mismatch");
static_assert(sizeof(BufferedSafeString) == 0xC, "sead::BufferedSafeStringBase<T> size mismatch");
#endif // cafe

template <s32 N>
class FixedSafeString : public FixedSafeStringBase<char, N>
{
public:
    FixedSafeString()
        : FixedSafeStringBase<char, N>()
    {
    }

    explicit FixedSafeString(const SafeString& rhs)
        : FixedSafeStringBase<char, N>(rhs)
    {
    }

    // Nintendo did not implement this
    //
    //FixedSafeString(const FixedSafeString<N>& rhs)
    //    : FixedSafeStringBase<char, N>(rhs)
    //{
    //}

    FixedSafeString<N>& operator=(const FixedSafeString<N>& rhs)
    {
        copy(rhs);
        return *this;
    }

    FixedSafeString<N>& operator=(const SafeString& rhs)
    {
        copy(rhs);
        return *this;
    }
};

template <s32 N>
class WFixedSafeString : public FixedSafeStringBase<char16, N>
{
public:
    WFixedSafeString()
        : FixedSafeStringBase<char16, N>()
    {
    }

    explicit WFixedSafeString(const WSafeString& rhs)
        : FixedSafeStringBase<char16, N>(rhs)
    {
    }

    // Nintendo did not implement this
    //
    //WFixedSafeString(const WFixedSafeString<N>& rhs)
    //    : FixedSafeStringBase<char16, N>(rhs)
    //{
    //}

    WFixedSafeString<N>& operator=(const WFixedSafeString<N>& rhs)
    {
        copy(rhs);
        return *this;
    }

    WFixedSafeString<N>& operator=(const WSafeString& rhs)
    {
        copy(rhs);
        return *this;
    }
};

template <s32 N>
class FormatFixedSafeString : public FixedSafeStringBase<char, N>
{
public:
    FormatFixedSafeString(const char* format_string, ...);

private:
    FormatFixedSafeString(const FormatFixedSafeString<N>&);
    FormatFixedSafeString<N>& operator=(const FormatFixedSafeString<N>&);
};

template <s32 N>
class WFormatFixedSafeString : public FixedSafeStringBase<char16, N>
{
public:
    WFormatFixedSafeString(const char16* format_string, ...);

private:
    WFormatFixedSafeString(const WFormatFixedSafeString<N>&);
    WFormatFixedSafeString<N>& operator=(const WFormatFixedSafeString<N>&);
};

class Heap;

template <typename T>
class HeapSafeStringBase : public BufferedSafeStringBase<T>
{
public:
    HeapSafeStringBase(Heap* heap, const SafeStringBase<T>& string, s32 size)
        : BufferedSafeStringBase<T>(new (heap, 4) T[size](),
                                    size)
    {
        copy(string);
    }

    HeapSafeStringBase(Heap* heap, const SafeStringBase<T>& string)
        : BufferedSafeStringBase<T>(new (heap, 4) T[string.calcLength() + 1](),
                                    string.calcLength() + 1)
    {
        copy(string);
    }

    virtual ~HeapSafeStringBase()
    {
        if (mStringTop)
            delete[] mStringTop;
    }

private:
    HeapSafeStringBase(const HeapSafeStringBase<T>&);
    HeapSafeStringBase<T>& operator=(const HeapSafeStringBase<T>&);
};

typedef HeapSafeStringBase<char> HeapSafeString;
typedef HeapSafeStringBase<char16> WHeapSafeString;

template <>
s32 BufferedSafeStringBase<char>::format(const char* format_string, ...);
template <>
s32 BufferedSafeStringBase<char16>::format(const char16* format_string, ...);
template <>
s32 BufferedSafeStringBase<char>::formatV(const char* format_string, va_list varg);
template <>
s32 BufferedSafeStringBase<char16>::formatV(const char16* format_string, va_list varg);
template <>
s32 BufferedSafeStringBase<char>::appendWithFormat(const char* format_string, ...);
template <>
s32 BufferedSafeStringBase<char16>::appendWithFormat(const char16* format_string, ...);
template <>
s32 BufferedSafeStringBase<char>::appendWithFormatV(const char* format_string, va_list varg);
template <>
s32 BufferedSafeStringBase<char16>::appendWithFormatV(const char16* format_string, va_list varg);
template <>
s32 BufferedSafeStringBase<char>::formatImpl_(char* dst, s32 dst_size, const char* format_string, va_list varg);
template <>
s32 BufferedSafeStringBase<char16>::formatImpl_(char16* dst, s32 dst_size, const char16* format_string, va_list varg);

} // namespace sead

#ifdef __cplusplus

#include <prim/seadSafeString.hpp>

#endif // __cplusplus

#endif // SEAD_SAFE_STRING_H_
