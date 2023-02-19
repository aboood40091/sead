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
    typedef ResArray<ResParameter>::iterator iterator;
    typedef ResArray<ResParameter>::constIterator constIterator;

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

    ResParameter getResParameter(u32 index) const;

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
    typedef ResArray<ResParameterList>::iterator listIterator;
    typedef ResArray<ResParameterList>::constIterator listConstIterator;

    typedef ResArray<ResParameterObj>::iterator objIterator;
    typedef ResArray<ResParameterObj>::constIterator objConstIterator;

public:
    listIterator listBegin() { return listIterator(0, getResParameterListBasePtr_()); }
    listConstIterator listBegin() const { return listConstIterator(0, getResParameterListBasePtr_()); }
    listConstIterator listConstBegin() const { return listConstIterator(0, getResParameterListBasePtr_()); }

    listIterator listEnd() { return listIterator(getResParameterListNum(), nullptr); }
    listConstIterator listEnd() const { return listConstIterator(getResParameterListNum(), nullptr); }
    listConstIterator listConstEnd() const { return listConstIterator(getResParameterListNum(), nullptr); }

    objIterator objBegin() { return objIterator(0, getResParameterObjBasePtr_()); }
    objConstIterator objBegin() const { return objConstIterator(0, getResParameterObjBasePtr_()); }
    objConstIterator objConstBegin() const { return objConstIterator(0, getResParameterObjBasePtr_()); }

    objIterator objEnd() { return objIterator(getResParameterObjNum(), nullptr); }
    objConstIterator objEnd() const { return objConstIterator(getResParameterObjNum(), nullptr); }
    objConstIterator objConstEnd() const { return objConstIterator(getResParameterObjNum(), nullptr); }

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

    ResParameterList getResParameterList(u32 index) const;
    ResParameterObj getResParameterObj(u32 index) const;

    inline s32 searchListIndex(u32 name_hash) const; // TODO
    inline s32 searchObjIndex(u32 name_hash) const; // TODO

    void modifyEndianList(bool is_le);

private:
    ResParameterListData* getResParameterListBasePtr_() const
    {
        return (ResParameterListData*)(ptr() + 1);
    }

    ResParameterObjData* getResParameterObjBasePtr_() const
    {
        listConstIterator itr = listConstBegin();
        while (static_cast<u32>(itr.getIndex()) < getResParameterListNum())
            ++itr;

        return (ResParameterObjData*)(&(*itr));
    }
};

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
    u32 mTypeVersion;
    u32 mTypeLen;
    // char mType[];

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

    const char* getType() const
    {
        const DataType* const data = ptr();
        return (const char*)(data + 1);
    }

    ResParameterList getResParameterList() const
    {
        const DataType* const data = ptr();
        return (const ResParameterListData*)((uintptr_t)(data + 1) + data->mTypeLen);
    }
};

} }
