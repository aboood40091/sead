#ifndef SEAD_RESOURCEMGR_H_
#define SEAD_RESOURCEMGR_H_

#include <container/seadTList.h>
#include <filedevice/seadFileDevice.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>

namespace sead {

class Resource;

class ResourcePtr
{
public:
    ResourcePtr(Resource* ptr)
        : mPtr(ptr)
    {
    }

    operator Resource*() { return mPtr; }
    operator const Resource*() const { return mPtr; }

    Resource& operator*() { return *mPtr; }
    Resource* operator->() { return mPtr; }

    const Resource& operator*() const { return *mPtr; }
    const Resource* operator->() const { return mPtr; }

private:
    Resource* mPtr;
};

class ResourceFactory;
class Decompressor;

class ResourceMgr
{
    SEAD_SINGLETON_DISPOSER(ResourceMgr)

public:
    struct CreateArg
    {
        CreateArg()
            : buffer(nullptr)
            , file_size(0)
            , buffer_size(0)
            , need_unload(false)
            , factory(nullptr)
            , ext()
            , heap(nullptr)
            , alignment(0x20)
        {
        }

        u8* buffer;
        u32 file_size;
        u32 buffer_size;
        bool need_unload;
        ResourceFactory* factory;
        SafeString ext;
        Heap* heap;
        s32 alignment;
    };
#ifdef cafe
    static_assert(sizeof(CreateArg) == 0x24, "sead::ResourceMgr::CreateArg size mismatch");
#endif // cafe

    struct LoadArg
    {
        LoadArg()
            : path()
            , instance_heap(nullptr)
            , load_data_heap(nullptr)
            , instance_alignment(0x20)
            , load_data_alignment(0)
            , load_data_buffer(nullptr)
            , load_data_buffer_size(0)
            , factory(nullptr)
            , device(nullptr)
            , div_size(0)
        {
        }

        SafeString path;
        Heap* instance_heap;
        Heap* load_data_heap;
        s32 instance_alignment;
        s32 load_data_alignment;
        u8* load_data_buffer;
        u32 load_data_buffer_size;
        ResourceFactory* factory;
        FileDevice* device;
        u32 div_size;
    };
#ifdef cafe
    static_assert(sizeof(LoadArg) == 0x2C, "sead::ResourceMgr::LoadArg size mismatch");
#endif // cafe

    typedef TList<ResourceFactory*> FactoryList;
    typedef TList<Resource*> ResourceList;
    typedef TList<Decompressor*> DecompressorList;

public:
    ResourceMgr();
    ~ResourceMgr();

    void postCreate();

    void registerFactory(ResourceFactory* factory, const SafeString& name);
    void registerDecompressor(Decompressor* decompressor, const SafeString& name);

    void unregisterFactory(ResourceFactory* factory);
    void unregisterDecompressor(Decompressor* decompressor);

    ResourcePtr tryLoadWithoutDecomp(const LoadArg& arg);
    ResourcePtr tryLoad(const LoadArg& arg, const SafeString& convert_ext, Decompressor* decomp);

    ResourceFactory* findFactory(const SafeString& ext);
    Decompressor* findDecompressor(const SafeString& ext);

private:
    FactoryList mFactoryList;
    ResourceList mPostCreateResourceList;
    DecompressorList mDecompList;
    ResourceFactory* mNullResourceFactory;
};
#ifdef cafe
static_assert(sizeof(ResourceMgr) == 0x38, "sead::ResourceMgr size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_RESOURCEMGR_H_
