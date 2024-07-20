#ifndef SEAD_DELEGATE_THREAD_H_
#define SEAD_DELEGATE_THREAD_H_

#include <thread/seadThread.h>

namespace sead {

template <typename T, typename U>
class IDelegate2;

class DelegateThread : public Thread
{
public:
    DelegateThread(const SafeString& name, IDelegate2<Thread*, MessageQueue::Element>* delegate, Heap* heap, s32 priority = cDefaultPriority, MessageQueue::BlockType block_type = cDefaultBlockType, MessageQueue::Element quit_msg = cDefaultQuitMsg, s32 stack_size = cDefaultStackSize, s32 msg_queue_size = cDefaultMsgQueueSize);
    virtual ~DelegateThread();

protected:
    virtual void calc_(MessageQueue::Element msg);

protected:
    IDelegate2<Thread*, MessageQueue::Element>* mDelegate;
};
#ifdef cafe
static_assert(sizeof(DelegateThread) == 0x94, "sead::DelegateThread size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_DELEGATE_THREAD_H_
