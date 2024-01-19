#include <filedevice/seadFileDeviceMgr.h>
#include <math/seadMathCalcCommon.h>
#include <prim/seadSafeString.h>
#include <resource/seadResource.h>

namespace sead {

Resource::Resource()
    : TListNode<Resource*>(this)
{
}

Resource::~Resource()
{
}

DirectResource::DirectResource()
    : Resource()
    , mRawData(nullptr)
    , mRawSize(0)
    , mBufferSize(0)
    , mSettingFlag()
{
}

DirectResource::~DirectResource()
{
    if (mSettingFlag.isOnBit(0))
        delete[] mRawData;
}

void DirectResource::create(u8* data, u32 size, u32 buffer_size, bool need_delete, Heap* instance_heap)
{
    if (mRawData != nullptr)
    {
        //SEAD_ASSERT_MSG(false, "read twice");
        return;
    }

    mRawData = data;
    mRawSize = size;
    mBufferSize = buffer_size;
    mSettingFlag.changeBit(0, need_delete);

    return doCreate_(data, size, instance_heap);
}

ResourceFactory::~ResourceFactory()
{
    ResourceMgr* mgr = ResourceMgr::instance();
    if (mgr != nullptr)
        mgr->unregisterFactory(this);
}

Resource* DirectResourceFactoryBase::create(const ResourceMgr::CreateArg& arg)
{
    DirectResource* res = newResource_(arg.heap, arg.alignment);
    if (res == nullptr)
    {
        //SEAD_ASSERT_MSG(false, "resource new failed.");
        return nullptr;
    }

    if (!PtrUtil::isAligned(arg.buffer, res->getLoadDataAlignment()))
    {
        //SEAD_ASSERT_MSG(false, "buffer alignment invalid: %p, %d", arg.buffer, res->getLoadDataAlignment());
        delete res;
        return nullptr;
    }

    res->create(arg.buffer, arg.file_size, arg.buffer_size, arg.need_unload, arg.heap);
    return res;
}

Resource* DirectResourceFactoryBase::tryCreate(const ResourceMgr::LoadArg& arg)
{
    DirectResource* res = newResource_(arg.instance_heap, arg.instance_alignment);
    if (res == nullptr)
    {
        //SEAD_ASSERT_MSG(false, "resource new failed.");
        return nullptr;
    }

    FileDevice::LoadArg fd_arg;

    fd_arg.path = arg.path;
    fd_arg.buffer = arg.load_data_buffer;
    fd_arg.buffer_size = arg.load_data_buffer_size;
    fd_arg.heap = arg.load_data_heap;
    fd_arg.div_size = arg.div_size;

    if (arg.load_data_alignment != 0)
        fd_arg.alignment = arg.load_data_alignment;

    else
        fd_arg.alignment = Mathi::sign(arg.instance_alignment) * res->getLoadDataAlignment();

    u8* data;
    if (arg.device != nullptr)
        data = arg.device->tryLoad(fd_arg);
    else
        data = FileDeviceMgr::instance()->tryLoad(fd_arg);

    if (data == nullptr)
    {
        delete res;
        return nullptr;
    }

    res->create(data, fd_arg.read_size, fd_arg.roundup_size, fd_arg.need_unload, arg.instance_heap);
    return res;
}

Resource*
DirectResourceFactoryBase::tryCreateWithDecomp(
    const ResourceMgr::LoadArg& arg, Decompressor* decomp
)
{
    DirectResource* res = newResource_(arg.instance_heap, arg.instance_alignment);
    if (res == nullptr)
    {
        //SEAD_ASSERT_MSG(false, "resource new failed.");
        return nullptr;
    }

    u32 size = 0;
    u32 buffer_size = 0;
    bool need_delete = false;

    u8* data = decomp->tryDecompFromDevice(arg, res, &size, &buffer_size, &need_delete);
    if (data == nullptr)
    {
        delete res;
        return nullptr;
    }

    res->create(data, size, buffer_size, need_delete, arg.instance_heap);
    return res;
}

}
