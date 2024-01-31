#ifndef SEAD_THREAD_H_
#define SEAD_THREAD_H_

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <hostio/seadHostIODummy.h>
#include <prim/seadNamable.h>
#include <thread/seadMessageQueue.h>
#include <thread/seadThreadLocalStorage.h>
#include <time/seadTickSpan.h>

#ifdef cafe
#include <cafe/os/OSThread.h>
#endif // cafe

namespace sead {

class Thread;

typedef TList<Thread*> ThreadList;
typedef TListNode<Thread*> ThreadListNode;

class Thread : public IDisposer, public INamable
{
public:
    virtual ~Thread();

    virtual void destroy();
    virtual bool sendMessage(s32, u32); // sendMessage(MessageQueue::Element, MessageQueue::BlockType)
    virtual s32 recvMessage(u32); // MessageQueue::Element recvMessage(MessageQueue::BlockType)
    virtual bool start();
    virtual void quit(bool);
    virtual void waitDone();
    virtual void quitAndDestroySingleThread(bool is_jam);
    //...

    Heap* getCurrentHeap() const { return mCurrentHeap; }

    static void sleep(TickSpan span);

    //...

private:
    MessageQueue mMessageQueue;
    s32 mStackSize;
    ThreadListNode mListNode;
    Heap* mCurrentHeap;
    MessageQueue::BlockType mBlockType;
    MessageQueue::Element mQuitMsg;
    u32 mID;
    u32 mState[1]; // State
    s32 mCoreNo;
#ifdef cafe
    OSThread* mThreadInner;
#endif // cafe
    u8* mStackTop;
    s32 mPriority;
};
#ifdef cafe
static_assert(sizeof(Thread) == 0x90, "sead::Thread size mismatch");
#endif // cafe

class ThreadMgr : public hostio::Node
{
    SEAD_SINGLETON_DISPOSER(ThreadMgr)

public:
    ThreadMgr();
    virtual ~ThreadMgr();

    void initialize(Heap*);
    void destroy();
    //void initHostIO();

    Thread* getCurrentThread() const;
    bool isMainThread() const;
    Thread* getMainThread() const { return mMainThread; }

    ThreadList::constIterator constBegin() const { return mList.constBegin(); }
    ThreadList::constIterator constEnd() const { return mList.constEnd(); }

    static void quitAndWaitDoneMultipleThread(Thread **, s32, bool);
    static void waitDoneMultipleThread(Thread* const*, s32);

protected:
    void initMainThread_(Heap*);
    void destroyMainThread_();

protected:
    ThreadList mList;
    Thread* mMainThread;
    ThreadLocalStorage mThreadPtrTLS;
};
#ifdef cafe
static_assert(sizeof(ThreadMgr) == 0x28, "sead::ThreadMgr size mismatch");
#endif // cafe

#ifdef cafe

inline Thread*
ThreadMgr::getCurrentThread() const
{
    return static_cast<Thread*>(OSGetThreadSpecific(mThreadPtrTLS.getValue()));
}

#endif // cafe

} // namespace sead

#endif // SEAD_THREAD_H_
