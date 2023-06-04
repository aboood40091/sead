#ifndef SEAD_EMITTER_SET_H_
#define SEAD_EMITTER_SET_H_

#include <ptcl/seadPtclData.h>
#include <ptcl/seadPtclSystem.h>
#include <ptcl/seadPtclResource.h>
#include <ptcl/seadEmitter.h>

#include <nw/eft/eft_EmitterSet.h>

namespace sead { namespace ptcl {

class EmitterController : private nw::eft::EmitterController
{
public:
    void reset()
    {
        Reset();
    }

    void setEmissionRatio(s32 ratio)
    {
        SetEmissionRatio(ratio);
    }

    void setEmissionRatio(f32 ratio)
    {
        SetEmissionRatio(ratio);
    }

    void setEmissionInterval(s32 ratio)
    {
        SetEmissionInterval(ratio);
    }

    void setEmissionInterval(f32 ratio)
    {
        SetEmissionInterval(ratio);
    }

    void setLife(s32 ratio)
    {
        SetLife(ratio);
    }

    void setLife(f32 ratio)
    {
        SetLife(ratio);
    }

    void setVisible(bool isVisible)
    {
        SetVisible(isVisible);
    }

    void setViewFlag(DrawViewFlag flag)
    {
        SetViewFlag((nw::eft::DrawViewFlag)flag);
    }

    void setStopDraw(bool isStopDraw)
    {
        SetStopDraw(isStopDraw);
    }

    bool isStopDraw() const
    {
        return IsStopDraw();
    }

    DrawViewFlag getDrawViewFlag() const
    {
        return (DrawViewFlag)GetDrawViewFlag();
    }

    static s32 F32ToScale(f32 val)
    {
        return nw::eft::EmitterController::F32ToScale(val);
    }

    void setFollowType(PtclFollowType followType)
    {
        SetFollowType((nw::eft::PtclFollowType)followType);
    }

    void setEmitterColor0(const Color4f& color)
    {
        SetEmitterColor0(reinterpret_cast<const nw::ut::FloatColor&>(color));
    }

    void setEmitterColor1(const Color4f& color)
    {
        SetEmitterColor1(reinterpret_cast<const nw::ut::FloatColor&>(color));
    }
};
static_assert(sizeof(EmitterController) == sizeof(nw::eft::EmitterController));

class EmitterSet : private nw::eft::EmitterSet
{
public:
    EmitterSet()
        : nw::eft::EmitterSet()
    {
    }

    void setSystem(PtclSystem* sys)
    {
        SetSystem(reinterpret_cast<nw::eft::System*>(sys));
    }

    PtclSystem* getSystem()
    {
        return reinterpret_cast<PtclSystem*>(GetSystem());
    }

    s32 getResourceID() const
    {
        return GetResourceID();
    }

    s32 getEmitterSetID() const
    {
        return GetEmitterSetID();
    }

    u32 getCreateID() const
    {
        return GetCreateID();
    }

    const char* getEmitterSetName() const
    {
        return GetEmitterSetName();
    }

    s32 getNumEmitter() const
    {
        return GetNumEmitter();
    }

    s32 getNumAliveEmitter() const
    {
        return GetNumAliveEmitter();
    }

    const EmitterInstance* getAliveEmitter(u32 idx)
    {
        return reinterpret_cast<const EmitterInstance*>(GetAliveEmitter(idx));
    }

    s32 getNumCreatedEmitter() const
    {
        return GetNumCreatedEmitter();
    }

    s32 getEmitterCounter(const char* emitterName)
    {
        return GetEmitterCounter(emitterName);
    }

    s32 getEmitterCounter(s32 emitterID)
    {
        return GetEmitterCounter(emitterID);
    }

    u8 getGroupID() const
    {
        return GetGroupID();
    }

    EmitterSet* getNext() const
    {
        return reinterpret_cast<EmitterSet*>(GetNext());
    }

    bool isLoopEffect() const
    {
        return IsLoopEffect();
    }

    bool isAlive() const
    {
        return IsAlive();
    }

    bool isHaveInfinityEmitter() const
    {
        return IsHaveInfinityEmitter();
    }

    Matrix34f& getSRTMatrix()
    {
        return reinterpret_cast<Matrix34f&>(GetSRTMatrix());
    }

    Matrix34f& getRTMatrix()
    {
        return reinterpret_cast<Matrix34f&>(GetRTMatrix());
    }

    void setDrawPriority(u8 priority)
    {
        SetDrawPriority(priority);
    }

    u8 getDrawPriority() const
    {
        return GetDrawPriority();
    }

    u32 getUserData() const
    {
        return GetUserData();
    }

    u8 getUserDataNum1() const
    {
        return GetUserDataNum1();
    }

    u8 getUserDataNum2() const
    {
        return GetUserDataNum2();
    }

    u16 getUserDataBit() const
    {
        return GetUserDataBit();
    }

    void setMtx(const Matrix34f& mtx)
    {
        SetMtx(reinterpret_cast<const nw::math::MTX34&>(mtx));
    }

    void setMtxFast(const Matrix34f& mtx)
    {
        SetMtxFast(reinterpret_cast<const nw::math::MTX34&>(mtx));
    }

    void setMtxFast(const Matrix34f& mtx, f32 scale)
    {
        SetMtxFast(reinterpret_cast<const nw::math::MTX34&>(mtx), scale);
    }

    void setMtxFast(const Matrix34f& mtx, const Vector3f& scale)
    {
        SetMtxFast(reinterpret_cast<const nw::math::MTX34&>(mtx), reinterpret_cast<const nw::math::VEC3&>(scale));
    }

    void setPos(const Vector3f& pos)
    {
        SetPos(reinterpret_cast<const nw::math::VEC3&>(pos));
    }

    void setPos(const Vector3f& pos, f32 scale)
    {
        SetPos(reinterpret_cast<const nw::math::VEC3&>(pos), scale);
    }

    void setPos(const Vector3f& pos, const Vector3f& scale)
    {
        SetPos(reinterpret_cast<const nw::math::VEC3&>(pos), reinterpret_cast<const nw::math::VEC3&>(scale));
    }

    void getPos(Vector3f& pos) const
    {
        GetPos(reinterpret_cast<nw::math::VEC3&>(pos));
    }

    void kill()
    {
        Kill();
    }

    void killInfinityEmitter()
    {
        KillInfinityEmitter();
    }

    void reset(const Matrix34f& matLW)
    {
        Reset(reinterpret_cast<const nw::math::MTX34&>(matLW));
    }

    void fade()
    {
        Fade()
    }

    bool isFadeRequest() const
    {
        return IsFadeRequest();
    }

    void cancelFade()
    {
        CancelFade();
    }

    void setParticleScale(f32 scale)
    {
        SetParticleScale(scale);
    }

    void setParticleScale(f32 scaleX, f32 scaleY)
    {
        SetParticleScale(scaleX, scaleY);
    }

    void setParticleScale(const Vector2f& scale)
    {
        SetParticleScale(reinterpret_cast<const nw::math::VEC2&>(scale));
    }

    const Vector2f& getParticleScale() const
    {
        return reinterpret_cast<const Vector2f&>(GetParticleScale());
    }

    void setEmissionParticleScale(f32 scale)
    {
        SetEmissionParticleScale(scale);
    }

    void setEmissionParticleScale(f32 scaleX, f32 scaleY)
    {
        SetEmissionParticleScale(scaleX, scaleY);
    }

    void setEmissionParticleScale(const Vector2f& scale)
    {
        SetEmissionParticleScale(reinterpret_cast<const nw::math::VEC2&>(scale));
    }

    const Vector2f& getEmissionParticleScale() const
    {
        return reinterpret_cast<const Vector2f&>(GetEmissionParticleScale());
    }

    void setEmitterScale(const Vector3f& scale)
    {
        SetEmitterScale(reinterpret_cast<const nw::math::VEC3&>(scale));
    }

    const Vector3f& getEmitterScale() const
    {
        return reinterpret_cast<const Vector3f&>(GetEmitterScale());
    }

    const Vector2f& getParticleScaleForCalc() const
    {
        return reinterpret_cast<const Vector2f&>(GetParticleScaleForCalc());
    }

    void setAlpha(f32 alpha)
    {
        SetAlpha(alpha);
    }

    void setColor(f32 r, f32 g, f32 b)
    {
        SetColor(r, g, b);
    }

    void setColor(const Color4f& color)
    {
        SetColor(reinterpret_cast<const nw::ut::FloatColor&>(color));
    }

    const Color4f& getColor() const
    {
        return reinterpret_cast<const Color4f&>(GetColor());
    }

    void setAllDirectionalVel(f32 velScale)
    {
        SetAllDirectionalVel(velScale);
    }

    f32 getAllDirectionalVel() const
    {
        return GetAllDirectionalVel();
    }

    void setDirectionalVel(f32 velScale)
    {
        SetDirectionalVel(velScale);
    }

    f32 getDirectionalVel() const
    {
        return GetDirectionalVel();
    }

    void setRandomVel(f32 velScale)
    {
        SetRandomVel(velScale);
    }

    f32 getRandomVel() const
    {
        return GetRandomVel();
    }

    void setAddVel(const Vector3f& velAdd)
    {
        SetAddVel(reinterpret_cast<const nw::math::VEC3&>(velAdd));
    }

    const Vector3f& getVelAdd() const
    {
        return reinterpret_cast<const Vector3f&>(GetVelAdd());
    }

    void setDirectional(const Vector3f& dir)
    {
        SetDirectional(reinterpret_cast<const nw::math::VEC3&>(dir));
    }

    const Vector3f& getDirectional() const
    {
        return reinterpret_cast<const Vector3f&>(GetDirectional());
    }

    void setInitRotate(const Vector3f& rot)
    {
        setInitRotate(reinterpret_cast<const nw::math::VEC3&>(rot));
    }

    void disableDirectional()
    {
        DisableDirectional();
    }

    bool isSetDirectional() const
    {
        return IsSetDirectional();
    }

    void setStartFrame(s32 frame)
    {
        SetStartFrame(frame);
    }

    s32 getStartFrame() const
    {
        return GetStartFrame();
    }

    EmitterController* getEmitterController(s32 ix)
    {
        return reinterpret_cast<EmitterController*>(GetEmitterController(ix));
    }

    const EmitterController* getAliveEmitterController(s32 ix)
    {
        return reinterpret_cast<const EmitterController*>(GetAliveEmitterController(ix));
    }

    void setParticleEmissionPoints(s32 numPoint, Vector3f* points)
    {
        SetParticleEmissionPoints(numPoint, reinterpret_cast<nw::math::VEC3*>(points));
    }

    void setManualParticleEmission(bool isManual)
    {
        SetManualParticleEmission(isManual);
    }

    void emitParticle(const Vector3f& pos)
    {
        EmitParticle(reinterpret_cast<const nw::math::VEC3&>(pos));
    }

    void forceCalc(s32 numLoop)
    {
        ForceCalc(numLoop);
    }

    void setStopCalcAndDraw(bool isStopCalcAndDraw)
    {
        SetStopCalcAndDraw(isStopCalcAndDraw);
    }

    bool isStopCalcAndDraw() const
    {
        return IsStopCalcAndDraw();
    }

    void setStopCalc(bool isStopCalc)
    {
        SetStopCalc(isStopCalc);
    }

    bool isStopCalc() const
    {
        return IsStopCalc();
    }

    void setStopDraw(bool isStopDraw)
    {
        SetStopDraw(isStopDraw);
    }

    bool isStopDraw() const
    {
        return IsStopDraw();
    }

    void setStopDraw(u32 emitterID, bool isStopDraw)
    {
        SetStopDraw(emitterID, isStopDraw);
    }

    void setRandomSeed(u32 seed)
    {
        SetRandomSeed(seed);
    }

    u32 getRuntimeUserData() const
    {
        return GetRuntimeUserData();
    }

    void setRuntimeUserData(u32 data)
    {
        SetRuntimeUserData(data);
    }

    void updateEmitterFromResInfo()
    {
        UpdateEmitterFromResInfo();
    }
};
static_assert(sizeof(EmitterSet) == sizeof(nw::eft::EmitterSet));

} }

#endif // SEAD_EMITTER_SET_H_
