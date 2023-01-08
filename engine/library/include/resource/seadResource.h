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
    virtual void doCreate_(u8*, u32, Heap*) { }

    void create(u8* buffer, u32 bufferSize, u32 allocSize, bool allocated, Heap* heap);

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

    virtual Resource* create(const ResourceMgr::CreateArg& createArg) = 0;
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg) = 0;
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor) = 0;

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

    virtual Resource* create(const ResourceMgr::CreateArg& createArg);
    virtual Resource* tryCreate(const ResourceMgr::LoadArg& loadArg);
    virtual Resource* tryCreateWithDecomp(const ResourceMgr::LoadArg& loadArg, Decompressor* decompressor);
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

    virtual DirectResource* newResource_(Heap* heap, s32 alignment)
    {
        return new(heap, alignment) T;
    }
};

} // namespace sead

#endif // SEAD_RESOURCE_H_
