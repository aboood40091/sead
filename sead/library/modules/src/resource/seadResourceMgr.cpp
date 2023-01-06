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

} // namespace sead
