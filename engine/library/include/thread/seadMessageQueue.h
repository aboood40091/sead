#ifndef SEAD_MESSAGE_QUEUE_H_
#define SEAD_MESSAGE_QUEUE_H_

#include <basis/seadTypes.h>

#ifdef cafe
#include <cafe/os/OSMessage.h>
#endif // cafe

namespace sead {

class Heap;

class MessageQueue
{
public:
    enum BlockType
    {
        cBlock = 0,
        cNoBlock = 1,
    };

    typedef s32 Element;

    static const Element cNullElement;

public:
    MessageQueue();
    ~MessageQueue();

    void allocate(s32 size, Heap* heap);
    void free();

    bool push(Element message, BlockType blockType);
    Element pop(BlockType blockType);
    Element peek(BlockType blockType) const;
    bool jam(Element message, BlockType blockType);

protected:
#ifdef cafe
    Element* mBuffer;
    OSMessageQueue mMessageQueueInner;
#else
#error "Unsupported platform"
#endif
};
#ifdef cafe
static_assert(sizeof(MessageQueue) == 0x40, "sead::MessageQueue size mismatch");
#endif // cafe

} // namespace sead

#endif // SEAD_MESSAGE_QUEUE_H_
