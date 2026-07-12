#ifndef SEAD_ALIAS_FILEDEVICE_H_
#define SEAD_ALIAS_FILEDEVICE_H_

#include <filedevice/seadFileDevice.h>

namespace sead {

class AliasFileDevice : public FileDevice
{
    SEAD_RTTI_OVERRIDE(AliasFileDevice, FileDevice)

public:
    AliasFileDevice(const SafeString& drive_name, FileDevice* device, const SafeString& path);
    virtual ~AliasFileDevice();

    virtual void traceFilePath(const SafeString& path) const;
    virtual void traceDirectoryPath(const SafeString& path) const;
    virtual void resolveFilePath(BufferedSafeString* out, const SafeString& path) const;
    virtual void resolveDirectoryPath(BufferedSafeString* out, const SafeString& path) const;

private:
    virtual bool doIsAvailable_() const;
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeString& filename, FileOpenFlag flag);
    virtual bool doClose_(FileHandle* handle);
    virtual bool doRead_(u32* read_size, FileHandle* handle, u8* buf, u32 size);
    virtual bool doWrite_(u32* write_size, FileHandle* handle, const u8* buf, u32 size);
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin);
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle);
    virtual bool doGetFileSize_(u32* size, const SafeString& path);
    virtual bool doGetFileSize_(u32* size, FileHandle* handle);
    virtual bool doIsExistFile_(bool* is_exist, const SafeString& path);
    virtual bool doIsExistDirectory_(bool* is_exist, const SafeString& path);
    virtual FileDevice* doOpenDirectory_(DirectoryHandle* handle, const SafeString& dirname);
    virtual bool doCloseDirectory_(DirectoryHandle* handle);
    virtual bool doReadDirectory_(u32* read_num, DirectoryHandle* handle, DirectoryEntry* entry, u32 num);
    virtual bool doMakeDirectory_(const SafeString& path, u32 permission);
    virtual bool isMatchDevice_(const HandleBase* handle) const;
    virtual RawErrorCode doGetLastRawError_() const;

private:
    FileDevice* mFileDevice;
    FixedSafeString<256> mPath;
};
#ifdef cafe
static_assert(sizeof(AliasFileDevice) == 0x164, "sead::AliasFileDevice size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_ALIAS_FILEDEVICE_H_
