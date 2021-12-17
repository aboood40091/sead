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
    Heap* mDisposerHeap;
    ListNode mListNode;

    friend class Heap;
};
#ifdef cafe
static_assert(sizeof(IDisposer) == 0x10, "sead::IDisposer size mismatch");
#endif // cafe

} // namespace sead

#define SEAD_SINGLETON_DISPOSER(CLASS)                                            \
    public:                                                                       \
        static CLASS* instance() { return sInstance; }                            \
        static CLASS* createInstance(sead::Heap* heap);                           \
        static void deleteInstance();                                             \
                                                                                  \
    protected:                                                                    \
        class SingletonDisposer_ : public sead::IDisposer                         \
        {                                                                         \
        public:                                                                   \
            SingletonDisposer_() : sead::IDisposer() { }                          \
            virtual ~SingletonDisposer_();                                        \
                                                                                  \
            static SingletonDisposer_* sStaticDisposer;                           \
        };                                                                        \
                                                                                  \
    private:                                                                      \
        CLASS(const CLASS&);                                                      \
        const CLASS& operator=(const CLASS&);                                     \
                                                                                  \
    protected:                                                                    \
        u32 mSingletonDisposerBuf_[sizeof(SingletonDisposer_) / sizeof(u32)];     \
                                                                                  \
        static CLASS* sInstance;                                                  \
                                                                                  \
        friend class SingletonDisposer_;

#define SEAD_CREATE_SINGLETON_INSTANCE(CLASS)                                                                    \
    CLASS* CLASS::createInstance(sead::Heap* heap)                                                               \
    {                                                                                                            \
        CLASS::SingletonDisposer_* staticDisposer = CLASS::SingletonDisposer_::sStaticDisposer;                  \
        CLASS* instance = CLASS::sInstance;                                                                      \
                                                                                                                 \
        if (CLASS::sInstance == NULL)                                                                            \
        {                                                                                                        \
            instance = reinterpret_cast<CLASS*>(new(heap, 4) u8[sizeof(CLASS)]);                                 \
                                                                                                                 \
            /*SEAD_ASSERT_MSG(staticDisposer == NULL, "Create Singleton Twice (%s).", "CLASS");*/                \
            staticDisposer = reinterpret_cast<CLASS::SingletonDisposer_*>(instance->mSingletonDisposerBuf_);     \
                                                                                                                 \
            CLASS::SingletonDisposer_::sStaticDisposer = new (staticDisposer) SingletonDisposer_();              \
            CLASS::sInstance = new (instance) CLASS;                                                             \
        }                                                                                                        \
        else                                                                                                     \
        {                                                                                                        \
            /*SEAD_ASSERT_MSG(false, "Create Singleton Twice (%s) : addr %x", "CLASS", CLASS::sInstance);*/      \
        }                                                                                                        \
                                                                                                                 \
        return CLASS::sInstance;                                                                                 \
    }

#define SEAD_DELETE_SINGLETON_INSTANCE(CLASS)                                                       \
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

#define SEAD_SINGLETON_DISPOSER_IMPL(CLASS)                                           \
    CLASS* CLASS::sInstance = NULL;                                                   \
    CLASS::SingletonDisposer_* CLASS::SingletonDisposer_::sStaticDisposer = NULL;     \
                                                                                      \
    SEAD_CREATE_SINGLETON_INSTANCE(CLASS)                                             \
    SEAD_DELETE_SINGLETON_INSTANCE(CLASS)                                             \
                                                                                      \
    CLASS::SingletonDisposer_::~SingletonDisposer_()                                  \
    {                                                                                 \
        if (this == sStaticDisposer)                                                  \
        {                                                                             \
            sStaticDisposer = NULL;                                                   \
            CLASS::sInstance->~CLASS();                                               \
            CLASS::sInstance = NULL;                                                  \
        }                                                                             \
    }

#endif // SEAD_DISPOSER_H_
