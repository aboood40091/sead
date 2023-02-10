#pragma once

#include <basis/seadTypes.h>

namespace agl {

void ModifyEndianU32(bool is_le, void* p_data, size_t size);

template <typename _DataType>
class ResCommon
{
public:
    typedef _DataType DataType;

public:
    ResCommon()
        : mpData(nullptr)
    {
    }

    ResCommon(const void* data)
        : mpData(static_cast<const DataType*>(data))
    {
    }

    bool isValid() const
    {
        return mpData != nullptr;
    }

    void verify() const
    {
        if (ref().mSigWord != DataType::cSignature)
        {
            const char* signature = ptr()->mSignature;
            // SEAD_ASSERT_MSG(false, "Wrong binary. [%c%c%c%c].",
            //                        signature[0], signature[1],
            //                        signature[2], signature[3]);
        }

        if (ref().mVersion != DataType::cVersion)
        {
            // SEAD_ASSERT_MSG(false, "Version error.current:%d binary:%d",
            //                        DataType::cVersion,
            //                        ref().mVersion);
        }
    }

    DataType* ptr()
    {
        return const_cast<DataType*>(mpData);
    }

    const DataType* ptr() const
    {
        return mpData;
    }

    DataType& ref()
    {
        // SEAD_ASSERT(isValid());
        return *ptr();
    }

    const DataType& ref() const
    {
        // SEAD_ASSERT(isValid());
        return *ptr();
    }

private:
    const DataType* mpData;
};

#define AGL_RES_COMMON(class_name)                              \
    public:                                                     \
        class_name()                                            \
            : ResCommon<typename class_name::DataType>()        \
        {                                                       \
        }                                                       \
                                                                \
        class_name(const void* data)                            \
            : ResCommon<typename class_name::DataType>(data)    \
        {                                                       \
        }

#define AGL_RES_FILE_HEADER()                                                   \
    public:                                                                     \
        bool modifyEndian() const                                               \
        {                                                                       \
            return ref().mEndian & DataType::cEndianCheckBit;                   \
        }                                                                       \
                                                                                \
        bool isEndianResolved() const                                           \
        {                                                                       \
            return !modifyEndian();                                             \
        }                                                                       \
                                                                                \
        void setEndianResolved()                                                \
        {                                                                       \
            ref().mEndian = 1 - ref().mEndian;                                  \
        }

template <typename DataType>
struct ResArrayData
{
    u32 mSize;
    u32 mNum;
    // DataType mData[];

    typedef DataType ElemType;
};

template <typename Type>
class ResArray : public ResCommon< ResArrayData<typename Type::DataType> >
{
    AGL_RES_COMMON(ResArray<Type>)

public:
    typedef Type ElemType;
    typedef typename Type::DataType ElemDataType;
    typedef typename ResArray<Type>::DataType DataType;
    typedef ResCommon<DataType> Base;

public:
    class iterator
    {
    public:
        iterator(s32 index, ElemDataType* elem)
            : mIndex(index)
            , mElem(elem)
        {
        }

        friend bool operator==(const iterator& lhs, const iterator& rhs)
        {
            return lhs.mIndex == rhs.mIndex;
        }

        friend bool operator!=(const iterator& lhs, const iterator& rhs)
        {
            return lhs.mIndex != rhs.mIndex;
        }

        iterator& operator++()
        {
            ++mIndex;
            mElem = (ElemDataType*)((uintptr_t)mElem + Type(mElem).ref().mSize);
            return *this;
        }

        ElemDataType& operator*() const { return *mElem; }
        ElemDataType* operator->() const { return mElem; }
        s32 getIndex() const { return mIndex; }

    private:
        s32 mIndex;
        ElemDataType* mElem;
    };

    class constIterator
    {
    public:
        constIterator(s32 index, const ElemDataType* elem)
            : mIndex(index)
            , mElem(elem)
        {
        }

        friend bool operator==(const constIterator& lhs, const constIterator& rhs)
        {
            return lhs.mIndex == rhs.mIndex;
        }

        friend bool operator!=(const constIterator& lhs, const constIterator& rhs)
        {
            return lhs.mIndex != rhs.mIndex;
        }

        constIterator& operator++()
        {
            ++mIndex;
            mElem = (const ElemDataType*)((uintptr_t)mElem + Type(mElem).ref().mSize);
            return *this;
        }

        const ElemDataType& operator*() const { return *mElem; }
        const ElemDataType* operator->() const { return mElem; }
        s32 getIndex() const { return mIndex; }

    private:
        s32 mIndex;
        const ElemDataType* mElem;
    };

public:
    iterator begin() { return iterator(0, (ElemDataType*)(Base::ptr() + 1)); }
    constIterator begin() const { return constIterator(0, (const ElemDataType*)(Base::ptr() + 1)); }
    constIterator constBegin() const { return constIterator(0, (const ElemDataType*)(Base::ptr() + 1)); }

    iterator end() { return iterator(getNum(), nullptr); }
    constIterator end() const { return constIterator(getNum(), nullptr); }
    constIterator constEnd() const { return constIterator(getNum(), nullptr); }

public:
    u32 getNum() const
    {
        return Base::ref().mNum;
    }

    ElemType get(s32 n) const
    {
        // SEAD_ASSERT(0 <= n && n <= static_cast< int >( this->getNum() ));

        constIterator itr = constBegin();
        constIterator itr_end = constIterator(n, nullptr);

        while (itr != itr_end)
            ++itr;

        return &(*itr);
    }

    void modifyEndianArray(bool is_le)
    {
        ModifyEndianU32(is_le, Base::ptr(), sizeof(DataType));

        for (iterator it = begin(), it_end = end(); it != it_end; ++it)
            ModifyEndianU32(is_le, &(*it), sizeof(ElemDataType));
    }
};

#define AGL_RES_ARRAY(class_name)                       \
    public:                                             \
        class_name()                                    \
             : ResArray<class_name::ElemType>()         \
         {                                              \
         }                                              \
                                                        \
         class_name(const void* data)                   \
             : ResArray<class_name::ElemType>(data)     \
         {                                              \
         }

}
