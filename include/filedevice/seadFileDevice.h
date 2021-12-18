#ifndef SEAD_FILEDEVICE_H_
#define SEAD_FILEDEVICE_H_

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <prim/seadRuntimeTypeInfo.h>
#include <prim/seadSafeString.h>

namespace sead {

typedef s32 RawErrorCode;
typedef u8 HandleBuffer[32]; // typedef SafeArray<u8, 32> HandleBuffer;

class FileDevice;

class HandleBase : public IDisposer
{
public:
    HandleBase()
        : IDisposer()
        , mDevice(NULL)
        , mOriginalDevice(NULL)
        //, mHandleBuffer()
    {
    }

private:
    HandleBase(const HandleBase&);
    const HandleBase& operator=(const HandleBase&);

public:
    virtual ~HandleBase() { }

    FileDevice* getDevice() const { return mDevice; }
    FileDevice* getOriginalDevice() const { return mOriginalDevice; }
    bool isOpen() const { return mDevice != NULL; }

protected:
    FileDevice* mDevice;
    FileDevice* mOriginalDevice;
    HandleBuffer mHandleBuffer;

    friend class FileDevice;
};
#ifdef cafe
static_assert(sizeof(HandleBase) == 0x38, "sead::HandleBase size mismatch");
#endif // cafe

class FileHandle;
class DirectoryHandle;
struct DirectoryEntry;

class FileDevice : public TListNode<FileDevice*>, public IDisposer
{
    SEAD_RTTI_BASE(FileDevice)

public:
    enum FileOpenFlag
    {
        cFileOpenFlag_ReadOnly = 0,   // r
        cFileOpenFlag_WriteOnly = 1,  // w
        cFileOpenFlag_ReadWrite = 2,  // r+
        cFileOpenFlag_Create = 3      // w+
    };

    enum SeekOrigin
    {
        cSeekOrigin_Begin = 0,
        cSeekOrigin_Current = 1,
        cSeekOrigin_End = 2
    };

    static const s32 cDriveNameBufferSize = 32;
#ifdef cafe
    static const s32 cBufferMinAlignment = 64;
#else
    static const s32 cBufferMinAlignment = 32;
#endif // cafe

    struct LoadArg
    {
        LoadArg()
            : path("")
            , buffer(NULL)
            , buffer_size(0)
            , heap(NULL)
            , alignment(0)
            , div_size(0)
            , read_size(0)
            , roundup_size(0)
            , need_unload(false)
        {
        }

        LoadArg(const LoadArg& arg)
            : path(arg.path)
            , buffer(arg.buffer)
            , buffer_size(arg.buffer_size)
            , heap(arg.heap)
            , alignment(arg.alignment)
            , div_size(arg.div_size)
            , read_size(arg.read_size)
            , roundup_size(arg.roundup_size)
            , need_unload(arg.need_unload)
        {
        }

        SafeString path;
        u8* buffer;
        u32 buffer_size;
        Heap* heap;
        s32 alignment;
        u32 div_size;
        u32 read_size;
        u32 roundup_size;
        bool need_unload;
    };
#ifdef cafe
    static_assert(sizeof(LoadArg) == 0x28, "sead::FileDevice::LoadArg size mismatch");
#endif // cafe

public:
    FileDevice(const SafeString& default_drive_name)
        : TListNode<FileDevice*>(this)
        , IDisposer()
        , mDriveName()
        , mPermission(true)
    {
        mDriveName.copy(default_drive_name);
    }

    virtual ~FileDevice();

    const SafeString& getDriveName() const
    {
        return mDriveName;
    }

    void setDriveName(const SafeString& drive_name)
    {
        mDriveName.copy(drive_name);
    }

    void setPermission(bool permission)
    {
        mPermission = permission;
    }

    bool hasPermission() const
    {
        return mPermission;
    }

    bool isAvailable() const;

    u8* load(LoadArg& arg)
    {
        u8* ret = tryLoad(arg);
        //SEAD_ASSERT_MSG(ret != NULL, "load failed. [%s]", arg.path.cstr());
        return ret;
    }

    u8* tryLoad(LoadArg& arg);

    void unload(u8* data)
    {
        //SEAD_ASSERT(data);
        delete data;
    }

    FileDevice* open(FileHandle* handle, const SafeString& filename, FileOpenFlag flag, u32 div_size)
    {
        FileDevice* device = tryOpen(handle, filename, flag, div_size);
        //SEAD_ASSERT_MSG(device != NULL, "open failed. [%s]", filename.cstr());
        return device;
    }

    FileDevice* tryOpen(FileHandle* handle, const SafeString& filename, FileOpenFlag flag, u32 div_size);

    bool close(FileHandle* handle)
    {
        bool success = tryClose(handle);
        //SEAD_ASSERT_MSG(success, "file close error");
        return success;
    }

    bool tryClose(FileHandle* handle);

    u32 read(FileHandle* handle, u8* buf, u32 size)
    {
        u32 read_size = 0;
        bool success = tryRead(&read_size, handle, buf, size);
        //SEAD_ASSERT_MSG(success, "file read error");
        return read_size;
    }

    bool tryRead(u32* read_size, FileHandle* handle, u8* buf, u32 size);

    u32 write(FileHandle* handle, const u8* buf, u32 size)
    {
        u32 write_size = 0;
        bool success = tryWrite(&write_size, handle, buf, size);
        //SEAD_ASSERT_MSG(success, "file write error");
        return write_size;
    }

    bool tryWrite(u32* write_size, FileHandle* handle, const u8* buf, u32 size);

    bool seek(FileHandle* handle, s32 offset, SeekOrigin origin)
    {
        bool success = trySeek(handle, offset, origin);
        //SEAD_ASSERT_MSG(success, "file seek error");
        return success;
    }

    bool trySeek(FileHandle* handle, s32 offset, SeekOrigin origin);

    u32 getCurrentSeekPos(FileHandle* handle)
    {
        u32 pos = 0;
        bool success = tryGetCurrentSeekPos(&pos, handle);
        //SEAD_ASSERT_MSG(success, "getCurrentSeekPos error");
        return pos;
    }

    bool tryGetCurrentSeekPos(u32* pos, FileHandle* handle);

    u32 getFileSize(const SafeString& path)
    {
        u32 size = 0;
        bool success = tryGetFileSize(&size, path);
        //SEAD_ASSERT_MSG(success, "getFileSize error");
        return size;
    }

    u32 getFileSize(FileHandle* handle)
    {
        u32 size = 0;
        bool success = tryGetFileSize(&size, handle);
        //SEAD_ASSERT_MSG(success, "getFileSize error");
        return size;
    }

    bool tryGetFileSize(u32* size, const SafeString& path);
    bool tryGetFileSize(u32* size, FileHandle* handle);

    bool isExistFile(const SafeString& path)
    {
        bool is_exist = false;
        bool success = tryIsExistFile(&is_exist, path);
        //SEAD_ASSERT_MSG(success, "isExistFile failed");
        return is_exist;
    }

    bool tryIsExistFile(bool* is_exist, const SafeString& path);

    bool isExistDirectory(const SafeString& path)
    {
        bool is_exist = false;
        bool success = tryIsExistDirectory(&is_exist, path);
        //SEAD_ASSERT_MSG(success, "isExistDirectory failed");
        return is_exist;
    }

    bool tryIsExistDirectory(bool* is_exist, const SafeString& path);

    FileDevice* openDirectory(DirectoryHandle* handle, const SafeString& dirname)
    {
        FileDevice* device = tryOpenDirectory(handle, dirname);
        //SEAD_ASSERT_MSG(device != NULL, "open directory failed. [%s]", dirname.cstr());
        return device;
    }

    FileDevice* tryOpenDirectory(DirectoryHandle* handle, const SafeString& dirname);

    bool closeDirectory(DirectoryHandle* handle)
    {
        bool success = tryCloseDirectory(handle);
        //SEAD_ASSERT_MSG(success, "directory close error");
        return success;
    }

    bool tryCloseDirectory(DirectoryHandle* handle);

    u32 readDirectory(DirectoryHandle* handle, DirectoryEntry* entry, u32 num)
    {
        u32 read_num = 0;
        bool success = tryReadDirectory(&read_num, handle, entry, num);
        //SEAD_ASSERT_MSG(success, "readDirectory failed");
        return read_num;
    }

    bool tryReadDirectory(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num);

    bool makeDirectory(const SafeString& path, u32 permission)
    {
        bool success = tryMakeDirectory(path, permission);
        //SEAD_ASSERT_MSG(success, "makeDirectory failed");
        return success;
    }

    bool tryMakeDirectory(const SafeString& path, u32 permission);

    bool makeDirectoryWithParent(const SafeString& path, u32 permission)
    {
        bool success = tryMakeDirectoryWithParent(path, permission);
        //SEAD_ASSERT_MSG(success, "makeDirectoryWithParent failed");
        return success;
    }

    bool tryMakeDirectoryWithParent(const SafeString& path, u32 permission);

    RawErrorCode getLastRawError() const;

    virtual void traceFilePath(const SafeString& path) const
    {
        doTracePath_(path);
    }

    virtual void traceDirectoryPath(const SafeString& path) const
    {
        doTracePath_(path);
    }

    virtual void resolveFilePath(BufferedSafeString* out, const SafeString& path) const
    {
        doResolvePath_(out, path);
    }

    virtual void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const
    {
        doResolvePath_(out, path);
    }

    virtual bool isMatchDevice_(const HandleBase* handle) const;

protected:
    virtual bool doIsAvailable_() const = 0;
    virtual u8* doLoad_(LoadArg& arg);
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag) = 0;
    virtual bool doClose_(FileHandle* handle) = 0;
    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size) = 0;
    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size) = 0;
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) = 0;
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle) = 0;
    virtual bool doGetFileSize_(u32* size, const SafeString& path) = 0;
    virtual bool doGetFileSize_(u32* size, FileHandle* handle) = 0;
    virtual bool doIsExistFile_(bool* is_exist, const SafeString& path) = 0;
    virtual bool doIsExistDirectory_(bool* is_exist, const SafeString& path) = 0;
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname) = 0;
    virtual bool doCloseDirectory_(DirectoryHandle* handle) = 0;
    virtual bool doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num) = 0;
    virtual bool doMakeDirectory_(const SafeString& path, u32 permission) = 0;
    virtual RawErrorCode doGetLastRawError_() const = 0;
    virtual void doTracePath_(const SafeString& path) const;
    virtual void doResolvePath_(BufferedSafeString* out, const SafeString& path) const;

    HandleBuffer& getHandleBaseHandleBuffer_(HandleBase* handle) const;
    void setHandleBaseFileDevice_(HandleBase* handle, FileDevice* device) const;
    void setHandleBaseOriginalFileDevice_(HandleBase* handle, FileDevice* device) const;
    void setFileHandleDivSize_(FileHandle* handle, u32 div_size) const;

protected:
    FixedSafeString<cDriveNameBufferSize> mDriveName;
    bool mPermission;

    friend class FileDeviceMgr;
};
#ifdef cafe
static_assert(sizeof(FileDevice) == 0x54, "sead::FileDevice size mismatch");
#endif // cafe

class FileHandle : public HandleBase
{
public:
    FileHandle()
        : HandleBase()
        , mDivSize(0)
    {
    }

    virtual ~FileHandle()
    {
        FileDevice* device = mOriginalDevice;
        if (device != NULL)
            device->tryClose(this);
    }

    u32 getDivSize() const { return mDivSize; }

    bool close();
    bool tryClose();

    u32 read(u8* buf, u32 size);
    bool tryRead(u32* read_size, u8* buf, u32 size);

    u32 write(const u8* buf, u32 size);
    bool tryWrite(u32* write_size, const u8* buf, u32 size);

    bool seek(s32 offset, FileDevice::SeekOrigin origin);
    bool trySeek(s32 offset, FileDevice::SeekOrigin origin);

    u32 getCurrentSeekPos();
    bool tryGetCurrentSeekPos(u32* pos);

    u32 getFileSize();
    bool tryGetFileSize(u32* size);

protected:
    u32 mDivSize;

    friend class FileDevice;
};
#ifdef cafe
static_assert(sizeof(FileHandle) == 0x3C, "sead::FileHandle size mismatch");
#endif // cafe

class DirectoryHandle : public HandleBase
{
public:
    DirectoryHandle()
        : HandleBase()
    {
    }

    virtual ~DirectoryHandle()
    {
    }

    bool close();
    bool tryClose();

    u32 read(DirectoryEntry* entry, u32 num);
    bool tryRead(u32* read_num, DirectoryEntry* entry, u32 num);
};

struct DirectoryEntry
{
    DirectoryEntry()
        : name()
        , is_directory(false)
    {
    }

    FixedSafeString<256> name;
    bool is_directory;
};
#ifdef cafe
static_assert(sizeof(DirectoryEntry) == 0x110, "sead::DirectoryEntry size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_FILEDEVICE_H_
