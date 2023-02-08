#pragma once

#include <common/aglResCommon.h>

namespace agl { namespace utl {

struct ResParameterData
{
    u32 mSize;
    u32 mType;
    u32 mNameHash;
};
static_assert(sizeof(ResParameterData) == 0xC, "agl::ResParameterData size mismatch");

class ResParameter : public ResCommon<ResParameterData>
{
    AGL_RES_COMMON(ResParameter)

public:
    u32 getParameterNameHash() const
    {
        return ptr()->mNameHash;
    }

    const void* getValue() const
    {
        return (const void*)(ptr() + 1);
    }
};

struct ResParameterObjData
{
    u32 mSize;
    u32 mNum;
    u32 mNameHash;
    u32 _c;
};
static_assert(sizeof(ResParameterObjData) == 0x10, "agl::ResParameterObjData size mismatch");

class ResParameterObj : public ResCommon<ResParameterObjData>
{
    AGL_RES_COMMON(ResParameterObj)

public:
    class iterator
    {
    public:
        iterator(s32 index, ResParameterData* elem)
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
            mElem = (ResParameterData*)((uintptr_t)mElem + mElem->mSize);
            return *this;
        }

        ResParameterData& operator*() const { return *mElem; }
        ResParameterData* operator->() const { return mElem; }
        s32 getIndex() const { return mIndex; }

    private:
        s32 mIndex;
        ResParameterData* mElem;
    };

    class constIterator
    {
    public:
        constIterator(s32 index, const ResParameterData* elem)
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
            mElem = (const ResParameterData*)((uintptr_t)mElem + mElem->mSize);
            return *this;
        }

        const ResParameterData& operator*() const { return *mElem; }
        const ResParameterData* operator->() const { return mElem; }
        s32 getIndex() const { return mIndex; }

    private:
        s32 mIndex;
        const ResParameterData* mElem;
    };

public:
    iterator begin() { return iterator(0, (ResParameterData*)(ptr() + 1)); }
    constIterator begin() const { return constIterator(0, (ResParameterData*)(ptr() + 1)); }
    constIterator constBegin() const { return constIterator(0, (ResParameterData*)(ptr() + 1)); }

    iterator end() { return iterator(getNum(), nullptr); }
    constIterator end() const { return constIterator(getNum(), nullptr); }
    constIterator constEnd() const { return constIterator(getNum(), nullptr); }

public:
    u32 getParameterObjNameHash() const
    {
        return ptr()->mNameHash;
    }

    u32 getNum() const
    {
        return ptr()->mNum;
    }

    ResParameter getResParameter(s32 n) const
    {
        // SEAD_ASSERT(0 <= index && index < getNum());

        constIterator it = begin();
        constIterator it_end = constIterator(n, nullptr);

        while (it != it_end)
            ++it;

        return &(*it);
    }

    inline s32 searchIndex(u32 name_hash) const; // TODO

    void modifyEndianObj(bool is_le);
};

struct ResParameterListData
{
    u32 mSize;
    u32 mNameHash;
    u32 mResParameterListNum;
    u32 mResParameterObjNum;
};
static_assert(sizeof(ResParameterListData) == 0x10, "agl::ResParameterListData size mismatch");

class ResParameterList : public ResCommon<ResParameterListData>
{
    AGL_RES_COMMON(ResParameterList)

public:
    template <typename ElemDataType>
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
            mElem = (ElemDataType*)((uintptr_t)mElem + mElem->mSize);
            return *this;
        }

        ElemDataType& operator*() const { return *mElem; }
        ElemDataType* operator->() const { return mElem; }
        s32 getIndex() const { return mIndex; }

    private:
        s32 mIndex;
        ElemDataType* mElem;
    };

    template <typename ElemDataType>
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
            mElem = (const ElemDataType*)((uintptr_t)mElem + mElem->mSize);
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
    template <typename ElemDataType>
    iterator<ElemDataType> begin();
    template <typename ElemDataType>
    constIterator<ElemDataType> begin() const;

    template <typename ElemDataType>
    iterator<ElemDataType> end();
    template <typename ElemDataType>
    constIterator<ElemDataType> end() const;

public:
    typedef iterator<ResParameterListData> listIterator;
    typedef constIterator<ResParameterListData> listConstIterator;

    listIterator listBegin();
    listConstIterator listBegin() const;
    listConstIterator listConstBegin() const;

    listIterator listEnd();
    listConstIterator listEnd() const;
    listConstIterator listConstEnd() const;

public:
    typedef iterator<ResParameterObjData> objIterator;
    typedef constIterator<ResParameterObjData> objConstIterator;

    objIterator objBegin();
    objConstIterator objBegin() const;
    objConstIterator objConstBegin() const;

    objIterator objEnd();
    objConstIterator objEnd() const;
    objConstIterator objConstEnd() const;

public:
    u32 getParameterListNameHash() const
    {
        return ptr()->mNameHash;
    }

    u32 getResParameterListNum() const
    {
        return ptr()->mResParameterListNum;
    }

    u32 getResParameterObjNum() const
    {
        return ptr()->mResParameterObjNum;
    }

    inline ResParameterList getResParameterList(s32 n) const;
    inline ResParameterObj getResParameterObj(s32 n) const;

    inline s32 searchListIndex(u32 name_hash) const; // TODO
    inline s32 searchObjIndex(u32 name_hash) const; // TODO

    void modifyEndianList(bool is_le);

private:
    inline ResParameterListData* getResParameterListBasePtr_() const;
    inline ResParameterObjData* getResParameterObjBasePtr_() const;
};

template <>
inline ResParameterList::iterator<ResParameterListData> ResParameterList::begin<ResParameterListData>() { return iterator<ResParameterListData>(0, getResParameterListBasePtr_()); }
template <>
inline ResParameterList::constIterator<ResParameterListData> ResParameterList::begin<ResParameterListData>() const { return constIterator<ResParameterListData>(0, getResParameterListBasePtr_()); }

template <>
inline ResParameterList::iterator<ResParameterListData> ResParameterList::end<ResParameterListData>() { return iterator<ResParameterListData>(getResParameterListNum(), nullptr); }
template <>
inline ResParameterList::constIterator<ResParameterListData> ResParameterList::end<ResParameterListData>() const { return constIterator<ResParameterListData>(getResParameterListNum(), nullptr); }

template <>
inline ResParameterList::iterator<ResParameterObjData> ResParameterList::begin<ResParameterObjData>() { return iterator<ResParameterObjData>(0, getResParameterObjBasePtr_()); }
template <>
inline ResParameterList::constIterator<ResParameterObjData> ResParameterList::begin<ResParameterObjData>() const { return constIterator<ResParameterObjData>(0, getResParameterObjBasePtr_()); }

template <>
inline ResParameterList::iterator<ResParameterObjData> ResParameterList::end<ResParameterObjData>() { return iterator<ResParameterObjData>(getResParameterObjNum(), nullptr); }
template <>
inline ResParameterList::constIterator<ResParameterObjData> ResParameterList::end<ResParameterObjData>() const { return constIterator<ResParameterObjData>(getResParameterObjNum(), nullptr); }

inline ResParameterList::listIterator ResParameterList::listBegin() { return begin<ResParameterListData>(); }
inline ResParameterList::listConstIterator ResParameterList::listBegin() const { return begin<ResParameterListData>(); }
inline ResParameterList::listConstIterator ResParameterList::listConstBegin() const { return begin<ResParameterListData>(); }

inline ResParameterList::listIterator ResParameterList::listEnd() { return end<ResParameterListData>(); }
inline ResParameterList::listConstIterator ResParameterList::listEnd() const { return end<ResParameterListData>(); }
inline ResParameterList::listConstIterator ResParameterList::listConstEnd() const { return end<ResParameterListData>(); }

inline ResParameterList::objIterator ResParameterList::objBegin() { return begin<ResParameterObjData>(); }
inline ResParameterList::objConstIterator ResParameterList::objBegin() const { return begin<ResParameterObjData>(); }
inline ResParameterList::objConstIterator ResParameterList::objConstBegin() const { return begin<ResParameterObjData>(); }

inline ResParameterList::objIterator ResParameterList::objEnd() { return end<ResParameterObjData>(); }
inline ResParameterList::objConstIterator ResParameterList::objEnd() const { return end<ResParameterObjData>(); }
inline ResParameterList::objConstIterator ResParameterList::objConstEnd() const { return end<ResParameterObjData>(); }

inline ResParameterList ResParameterList::getResParameterList(s32 n) const
{
    // SEAD_ASSERT(0 <= index && index < getResParameterListNum());

    constIterator<ResParameterListData> it = begin<ResParameterListData>();
    constIterator<ResParameterListData> it_end = constIterator<ResParameterListData>(n, nullptr);

    while (it != it_end)
        ++it;

    return &(*it);
}

inline ResParameterObj ResParameterList::getResParameterObj(s32 n) const
{
    // SEAD_ASSERT(0 <= index && index < getResParameterObjNum());

    constIterator<ResParameterObjData> it = begin<ResParameterObjData>();
    constIterator<ResParameterObjData> it_end = constIterator<ResParameterObjData>(n, nullptr);

    while (it != it_end)
        ++it;

    return &(*it);
}

inline ResParameterListData* ResParameterList::getResParameterListBasePtr_() const
{
    return (ResParameterListData*)(ptr() + 1);
}

inline ResParameterObjData* ResParameterList::getResParameterObjBasePtr_() const
{
    constIterator<ResParameterListData> it = begin<ResParameterListData>();
    constIterator<ResParameterListData> it_end = end<ResParameterListData>();

    while (it != it_end)
        ++it;

    return (ResParameterObjData*)(&(*it));
}

struct ResParameterArchiveData
{
    union
    {
        char mSignature[4];
        u32 mSigWord;
    };
    u32 mVersion;
    u32 mEndian;
    u32 mFileSize;
    u32 _10;
    u32 mNameLen;
    // char mName[];

public:
    static u32 getVersion();
    static u32 getSignature();

private:
    static const u32 cVersion = 1;
    static const u32 cSignature = 0x504D4141; // PMAA
    static const u32 cEndianCheckBit = 0x01000001;

    friend class ResCommon<ResParameterArchiveData>;
    friend class ResParameterArchive;
};
static_assert(sizeof(ResParameterArchiveData) == 0x18, "agl::ResParameterArchiveData size mismatch");

class ResParameterArchive : public ResCommon<ResParameterArchiveData>
{
    AGL_RES_FILE_HEADER()

public:
    ResParameterArchive(const void* p_data);

    const char* getName() const
    {
        const DataType* const data = ptr();
        return (const char*)(data + 1);
    }

    ResParameterList getResParameterList() const
    {
        const DataType* const data = ptr();
        return (const ResParameterListData*)((uintptr_t)(data + 1) + data->mNameLen);
    }
};

} }
