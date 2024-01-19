#ifndef SEAD_RESOURCE_H_
#define SEAD_RESOURCE_H_

#include <basis/seadNew.h>
#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <prim/seadBitFlag.h>
#include <resource/seadDecompressor.h>
#include <resource/seadResourceMgr.h>

namespace sead {

class Resource : public TListNode<Resource*>
{
    SEAD_RTTI_BASE(Resource)

public:
    Resource();
    virtual ~Resource();

protected:
    virtual void doPostCreate_() { }
};
#ifdef cafe
static_assert(sizeof(Resource) == 0x14, "sead::Resource size mismatch");
#endif // cafe

class DirectResource : public Resource
{
    SEAD_RTTI_OVERRIDE(DirectResource, Resource)

public:
    DirectResource();
    virtual ~DirectResource();

    virtual u32 getLoadDataAlignment() { return 4; }

protected:
    virtual void doCreate_(u8*, u32, Heap*) { }

public:
    void create(u8* data, u32 size, u32 buffer_size, bool need_delete, Heap* instance_heap);

    u8* getData() const
    {
        return mRawData;
    }

    u32 getSize() const
    {
        return mRawSize;
    }

protected:
    u8* mRawData;
    u32 mRawSize;
    u32 mBufferSize;
    BitFlag32 mSettingFlag;
};
#ifdef cafe
static_assert(sizeof(DirectResource) == 0x24, "sead::DirectResource size mismatch");
#endif // cafe

class ResourceFactory : public TListNode<ResourceFactory*>, public IDisposer
{
public:
    ResourceFactory()
        : TListNode<ResourceFactory*>(this)
        , IDisposer()
        , mExt()
    {
    }

    virtual ~ResourceFactory();

    virtual Resource* create(const ResourceMgr::CreateArg& arg) = 0;
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& arg) = 0;
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& arg, Decompressor* decomp) = 0;

    void setExt(const SafeString& ext)
    {
        mExt.copy(ext);
    }

protected:
    FixedSafeString<32> mExt;
};
#ifdef cafe
static_assert(sizeof(ResourceFactory) == 0x50, "sead::ResourceFactory size mismatch");
#endif // cafe

class DirectResourceFactoryBase : public ResourceFactory
{
public:
    DirectResourceFactoryBase()
        : ResourceFactory()
    {
    }

    virtual ~DirectResourceFactoryBase()
    {
    }

    virtual Resource* create(const ResourceMgr::CreateArg& arg);
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& arg);
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& arg, Decompressor* decomp);

protected:
    virtual DirectResource* newResource_(Heap* heap, s32 alignment) = 0;
};

template <typename T>
class DirectResourceFactory : public DirectResourceFactoryBase
{
public:
    DirectResourceFactory()
        : DirectResourceFactoryBase()
    {
    }

    virtual ~DirectResourceFactory()
    {
    }

private:
    virtual DirectResource* newResource_(Heap* heap, s32 alignment)
    {
        return new(heap, alignment) T;
    }
};

} // namespace sead

#endif // SEAD_RESOURCE_H_
