#ifndef SEAD_RUNTIMETYPEINFO_H_
#define SEAD_RUNTIMETYPEINFO_H_

namespace sead { namespace RuntimeTypeInfo {

struct Interface
{
    Interface() { }

    virtual bool isDerived(const Interface* other) const = 0;
};

struct Root : public Interface
{
    Root() { }

    virtual bool isDerived(const Interface* other) const
    {
        return other == this;
    }
};

template <typename Super>
struct Derive : public Interface
{
    Derive() { }

    virtual bool isDerived(const Interface* other) const
    {
        if (this == other)
            return true;

        const RuntimeTypeInfo::Interface* rootTypeInfo = Super::getRuntimeTypeInfoStatic();
        return rootTypeInfo->isDerived(other);
    }
};

} // namespace sead::RuntimeTypeInfo

template <typename DerivedType, typename Type>
inline bool
IsDerivedTypes(const Type* ptr)
{
    const RuntimeTypeInfo::Interface* typeInfo = DerivedType::getRuntimeTypeInfoStatic();
    return ptr != NULL && ptr->checkDerivedRuntimeTypeInfo(typeInfo);
}

template<typename DerivedType, typename Type>
inline DerivedType*
DynamicCast(Type* ptr)
{
    if (IsDerivedTypes<DerivedType, Type>(ptr))
        return static_cast<DerivedType*>(ptr);

    return NULL;
}

} // namespace sead

#define SEAD_RTTI_BASE(CLASS)                                                                                \
    public:                                                                                                  \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()                            \
        {                                                                                                    \
            static const sead::RuntimeTypeInfo::Root typeInfo;                                               \
            return &typeInfo;                                                                                \
        }                                                                                                    \
                                                                                                             \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface* type) const         \
        {                                                                                                    \
            const sead::RuntimeTypeInfo::Interface* clsTypeInfo = CLASS::getRuntimeTypeInfoStatic();         \
            return type == clsTypeInfo;                                                                      \
        }                                                                                                    \
                                                                                                             \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                           \
        {                                                                                                    \
            return getRuntimeTypeInfoStatic();                                                               \
        }

#define SEAD_RTTI_OVERRIDE(CLASS, BASE)                                                                      \
    public:                                                                                                  \
        static const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfoStatic()                            \
        {                                                                                                    \
            static const sead::RuntimeTypeInfo::Derive<BASE> typeInfo;                                       \
            return &typeInfo;                                                                                \
        }                                                                                                    \
                                                                                                             \
        virtual bool checkDerivedRuntimeTypeInfo(const sead::RuntimeTypeInfo::Interface* type) const         \
        {                                                                                                    \
            const sead::RuntimeTypeInfo::Interface* clsTypeInfo = CLASS::getRuntimeTypeInfoStatic();         \
            if (type == clsTypeInfo)                                                                         \
                return true;                                                                                 \
                                                                                                             \
            return BASE::checkDerivedRuntimeTypeInfo(type);                                                  \
        }                                                                                                    \
                                                                                                             \
        virtual const sead::RuntimeTypeInfo::Interface* getRuntimeTypeInfo() const                           \
        {                                                                                                    \
            return getRuntimeTypeInfoStatic();                                                               \
        }

#endif // SEAD_RUNTIMETYPEINFO_H_
