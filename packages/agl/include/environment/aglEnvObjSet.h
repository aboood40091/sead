#pragma once

#include <environment/aglEnvObjBuffer.h>
#include <hostio/seadHostIODummy.h>
#include <utility/aglNamedObjIndex.h>
#include <utility/aglParameterList.h>
#include <utility/aglParameterObj.h>

namespace agl { namespace env {

class EnvObjMgr;

class EnvObjSet : public EnvObjBuffer, public utl::INamedObjIndexCallback, public utl::IParameterList, public sead::hostio::Node
{
protected:
    class Ref : public utl::IParameterObj
    {
    public:
        Ref()
        {
        }

    protected:
        virtual bool isApply_(utl::ResParameterObj obj) const;

    private:
        EnvObj::Index mEnvIndex;
        utl::Parameter< sead::FixedSafeString<32> > mType;

        friend class EnvObjSet;
    };
    static_assert(sizeof(Ref) == 0x100);

public:
    EnvObjSet();
    virtual ~EnvObjSet();

    void bind(EnvObjMgr* p_env_mgr);

    virtual void allocBuffer(const AllocateArg& arg, sead::Heap* heap);

protected:
    virtual bool preRead_();
    virtual void postRead_();

public:
    virtual void callbackSyncNameToIndex(utl::INamedObjIndex* p_index);
    virtual void callbackSyncIndexToName(utl::INamedObjIndex* p_index);

    virtual void genMessageEachObj(sead::hostio::Context*, s32, const EnvObj*)
    {
    }

    void setName(const sead::SafeString& name)
    {
        *mName = name;
    }

    const sead::SafeString& getName() const
    {
        return *mName;
    }

    bool pushBack(EnvObj* p_obj);

protected:
    utl::Parameter< sead::FixedSafeString<32> > mName;
    utl::IParameterList mObjRefArrayParameterList;
    EnvObjMgr* mpEnvObjMgr;
    utl::IParameterObj mSetting;
    sead::Buffer<Ref> mObjRefArrayBuffer;

    friend class EnvObjMgr;
};
static_assert(sizeof(EnvObjSet) == 0x1DC);

} }
