#include <filedevice/seadPath.h>
#include <heap/seadHeap.h>
#include <heap/seadHeapMgr.h>
#include <prim/seadSafeString.h>
#include <resource/seadResource.h>
#include <resource/seadResourceMgr.h>

namespace sead {

SEAD_SINGLETON_DISPOSER_IMPL(ResourceMgr)

ResourceMgr::ResourceMgr()
    : mFactoryList()
    , mPostCreateResourceList()
    , mDecompList()
    , mNullResourceFactory(nullptr)
{
    if (!HeapMgr::isInitialized())
    {
        //SEAD_ASSERT_MSG(false, ResourceMgr need HeapMgr);
        return;
    }

    mNullResourceFactory = new (HeapMgr::instance()->findContainHeap(this), 4) DirectResourceFactory<DirectResource>();
    registerFactory(mNullResourceFactory, "");
}

ResourceMgr::~ResourceMgr()
{
    if (mNullResourceFactory == nullptr)
        return;

    delete mNullResourceFactory;
    mNullResourceFactory = nullptr;
}

void ResourceMgr::registerFactory(ResourceFactory* factory, const SafeString& name)
{
    factory->mExt.copy(name);

    mFactoryList.pushBack(factory);
}

void ResourceMgr::registerDecompressor(Decompressor* decompressor, const SafeString& name)
{
    if (!name.isEqual(SafeString::cEmptyString))
        decompressor->mExt.copy(name);

    mDecompList.pushBack(decompressor);
}

void ResourceMgr::unregisterFactory(ResourceFactory* factory)
{
    if (factory->mList == nullptr)
        return;

    mFactoryList.erase(factory);
}

void ResourceMgr::unregisterDecompressor(Decompressor* decompressor)
{
    if (decompressor->mList == nullptr)
        return;

    mDecompList.erase(decompressor);
}

ResourcePtr ResourceMgr::tryLoadWithoutDecomp(const LoadArg& arg)
{
    ResourceFactory* factory;
    if (arg.factory != nullptr)
        factory = arg.factory;

    else
    {
        FixedSafeString<32> ext;
        if (!Path::getExt(&ext, arg.path))
            factory = mNullResourceFactory;

        else
        {
            factory = findFactory(ext);
            // SEAD_ASSERT(factory);
        }
    }
    return factory->tryCreate(arg);
}

ResourcePtr ResourceMgr::tryLoad(const LoadArg& arg, const SafeString& convert_ext, Decompressor* decomp)
{
    SafeString factory_ext;
    FixedSafeString<32> ext;

    if (decomp == nullptr)
    {
        if (!Path::getExt(&ext, arg.path))
        {
            // SEAD_ASSERT_MSG(false, "no file extention");
            return nullptr;
        }
        decomp = findDecompressor(ext);
    }

    if (decomp != nullptr)
        factory_ext = convert_ext;
    else
        factory_ext = ext;

    ResourceFactory* factory;
    if (arg.factory != nullptr)
        factory = arg.factory;

    else
    {
        factory = findFactory(factory_ext);
        // SEAD_ASSERT(factory);
    }

    if (decomp != nullptr)
        return factory->tryCreateWithDecomp(arg, decomp);
    else
        return factory->tryCreate(arg);
}

} // namespace sead
