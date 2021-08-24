#ifndef SEAD_DISPOSER_H_
#define SEAD_DISPOSER_H_

#include <basis/seadNew.h>
#include <container/seadListImpl.h>

namespace sead {

class Heap;

class IDisposer
{
public:
    IDisposer();
    virtual ~IDisposer();

private:
    friend class Heap;

    Heap* mDisposerHeap;
    ListNode mListNode;
};

} // namespace sead

#define SEAD_SINGLETON_DISPOSER(CLASS)                                            \
    public:                                                                       \
        static CLASS* instance() { return sInstance; }                            \
        static CLASS* createInstance(sead::Heap* heap);                           \
        static void deleteInstance();                                             \
                                                                                  \
    protected:                                                                    \
        static CLASS* sInstance;                                                  \
                                                                                  \
    private:                                                                      \
        class SingletonDisposer_ : public sead::IDisposer                         \
        {                                                                         \
        public:                                                                   \
            SingletonDisposer_() : sead::IDisposer() { }                          \
            virtual ~SingletonDisposer_();                                        \
                                                                                  \
        private:                                                                  \
            friend class CLASS;                                                   \
                                                                                  \
            static SingletonDisposer_* sStaticDisposer;                           \
        };                                                                        \
                                                                                  \
        friend class SingletonDisposer_;                                          \
                                                                                  \
        u32 mSingletonDisposerBuf_[sizeof(SingletonDisposer_) / sizeof(u32)];

#define SEAD_CREATE_SINGLETON_INSTANCE(CLASS)                                                                    \
    CLASS* CLASS::createInstance(sead::Heap* heap)                                                               \
    {                                                                                                            \
        CLASS::SingletonDisposer_* staticDisposer = CLASS::SingletonDisposer_::sStaticDisposer;                  \
        CLASS* instance = CLASS::sInstance;                                                                      \
                                                                                                                 \
        if (CLASS::sInstance == NULL)                                                                            \
        {                                                                                                        \
            instance = reinterpret_cast<CLASS*>(new(heap, 4) u8[sizeof(CLASS)]);                                 \
            staticDisposer = reinterpret_cast<CLASS::SingletonDisposer_*>(instance->mSingletonDisposerBuf_);     \
                                                                                                                 \
            CLASS::SingletonDisposer_::sStaticDisposer = new (staticDisposer) SingletonDisposer_();              \
            CLASS::sInstance = new (instance) CLASS();                                                           \
        }                                                                                                        \
                                                                                                                 \
        return CLASS::sInstance;                                                                                 \
    }

#define SEAD_DELETE_SINGLETON_INSTANCE(CLASS)                                                       \
    CLASS::SingletonDisposer_::~SingletonDisposer_()                                                \
    {                                                                                               \
        if (this == sStaticDisposer)                                                                \
        {                                                                                           \
            sStaticDisposer = NULL;                                                                 \
            CLASS::sInstance->~CLASS();                                                             \
            CLASS::sInstance = NULL;                                                                \
        }                                                                                           \
    }                                                                                               \
                                                                                                    \
    void CLASS::deleteInstance()                                                                    \
    {                                                                                               \
        CLASS::SingletonDisposer_* staticDisposer = CLASS::SingletonDisposer_::sStaticDisposer;     \
        if (CLASS::SingletonDisposer_::sStaticDisposer != NULL)                                     \
        {                                                                                           \
            CLASS::SingletonDisposer_::sStaticDisposer = NULL;                                      \
            staticDisposer->~SingletonDisposer_();                                                  \
                                                                                                    \
            if (CLASS::sInstance != NULL)                                                           \
                delete CLASS::sInstance;                                                            \
                                                                                                    \
            CLASS::sInstance = NULL;                                                                \
        }                                                                                           \
    }

#endif // SEAD_DISPOSER_H_
