#pragma once

#include <container/seadBuffer.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>
#include <thread/seadCriticalSection.h>

namespace agl { namespace detail {

class PrivateResource
{
    SEAD_SINGLETON_DISPOSER(PrivateResource)

public:
    PrivateResource();
    virtual ~PrivateResource();

    sead::Heap* getWorkHeap() const { return mWorkHeap; }
    sead::Heap* getShaderCompileHeap() const { return mShaderCompileHeap; }
    sead::HeapSafeString* getShaderText() const { return mShaderText; }
    sead::Buffer<char>& getShaderWorkBuffer() { return mShaderWorkBuffer; }
    sead::Heap* getDebugHeap() const { return mDebugHeap; }
    sead::CriticalSection& getCriticalSection() { return mCriticalSection; }
    const sead::CriticalSection& getCriticalSection() const { return mCriticalSection; }

private:
    sead::Heap* mWorkHeap; // ExpHeap*
    sead::Heap* mShaderCompileHeap; // ExpHeap*
    sead::HeapSafeString* mShaderText;
    sead::Buffer<char> mShaderWorkBuffer;
    u32 _24;
    u8 _28[4];
    sead::Heap* mDebugHeap;
    sead::CriticalSection mCriticalSection;
};
static_assert(sizeof(PrivateResource) == 0x70, "agl::detail::PrivateResource size mismatch");


} }
