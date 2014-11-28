#pragma once
#include "gpCore/World/EntityBase.h"

class gpRenderExtractor;
struct gpForceFieldEntity;

class GP_CoreAPI gpWorld
{
public:
    gpWorld(const char* szName);
    gpWorld(const gpWorld&) = delete;
    void operator=(const gpWorld&) = delete;
    ~gpWorld();

    template<typename Type>
    Type* CreateEntity()
    {
        EZ_PROFILE(m_ProfilingId_CreateEntity);
        auto pEntity = EZ_DEFAULT_NEW(Type);
        InsertCreatedEntity(pEntity);
        return pEntity;
    }

    gpVec3 GetGravity() const { return m_Gravity; }
    void SetGravity(const gpVec3& NewGravity) { m_Gravity = NewGravity; }

    void StepSimulation(ezTime dt);

    void CollectGarbage();

    void ExtractRenderingData(gpRenderExtractor* pExtractor) const;

    EZ_FORCE_INLINE void SetEntityDrawInfoDefault(gpEntityDrawInfo* pDrawInfo) { m_pEntityDrawInfoDefault = pDrawInfo; }
    EZ_FORCE_INLINE gpEntityDrawInfo* GetEntityDrawInfoDefault() { return m_pEntityDrawInfoDefault; }

private:
    ezString m_sName;
    ezProfilingId m_ProfilingId_Simulation;
    ezProfilingId m_ProfilingId_CreateEntity;
    ezProfilingId m_ProfilingId_Extraction;

    gpVec3 m_Gravity;
    ezDynamicArray<gpForceFieldEntity*> m_ForceFields;
    ezDynamicArray<gpEntityBase*> m_CreatedEntities;
    ezDynamicArray<gpEntityBase*> m_SimulatedEntities;
    ezMap<gpEntityBase*, gpEntityDrawInfo> m_EntityDrawInfos;
    gpEntityDrawInfo m_EntityDrawInfo_HardDefault;
    gpEntityDrawInfo* m_pEntityDrawInfoDefault;

private:
    void InsertCreatedEntity(gpEntityBase* pEntity);

    friend const ezString& gpNameOf(const gpWorld& world);
    friend gpVec3& gpGravityOf(gpWorld& world);
    friend GP_CoreAPI ezResult gpAddEntityTo(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI ezResult gpRemoveEntityFrom(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI void gpClearSimulatedEntities(gpWorld& world);
    friend GP_CoreAPI void gpClearForceFields(gpWorld& world);
    friend GP_CoreAPI gpEntityDrawInfo& gpDrawInfoOf(gpWorld& world, gpEntityBase& entity);
};

EZ_FORCE_INLINE const ezString& gpNameOf(const gpWorld& world) { return world.m_sName; }
EZ_FORCE_INLINE const ezString& gpNameOf(const gpWorld* pWorld)
{
    EZ_ASSERT(pWorld, "");
    return gpNameOf(Deref(pWorld));
}
EZ_FORCE_INLINE       gpVec3& gpGravityOf(      gpWorld& world) { return world.m_Gravity; }
EZ_FORCE_INLINE const gpVec3& gpGravityOf(const gpWorld& world) { return gpGravityOf(const_cast<gpWorld&>(world)); }

GP_CoreAPI ezResult gpAddEntityTo(gpWorld& world, gpEntityBase& entity);
GP_CoreAPI ezResult gpRemoveEntityFrom(gpWorld& world, gpEntityBase& entity);
GP_CoreAPI void gpClearSimulatedEntities(gpWorld& world);
GP_CoreAPI void gpClearForceFields(gpWorld& world);
EZ_FORCE_INLINE void gpClear(gpWorld& world)
{
    gpClearSimulatedEntities(world);
    gpClearForceFields(world);
}
GP_CoreAPI gpEntityDrawInfo& gpDrawInfoOf(gpWorld& world, gpEntityBase& entity);
