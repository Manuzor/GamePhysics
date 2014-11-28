#pragma once
#include "gpCore/World/EntityBase.h"

class gpRenderExtractor;
class gpForceFieldEntity;

class GP_CoreAPI gpWorld
{
public:
    gpWorld(const char* szName);
    gpWorld(const gpWorld&) = delete;
    void operator=(const gpWorld&) = delete;
    ~gpWorld();

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

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend const ezString& gpNameOf(const gpWorld& world);
    friend gpVec3& gpGravityOf(gpWorld& world);
    friend GP_CoreAPI ezResult gpAddEntityTo(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI ezResult gpRemoveEntityFrom(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI void gpClearSimulatedEntities(gpWorld& world);
    friend GP_CoreAPI void gpClearForceFields(gpWorld& world);
    friend GP_CoreAPI gpEntityDrawInfo& gpDrawInfoOf(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI void gpCollectGarbageOf(gpWorld& world);
    friend GP_CoreAPI void gpStepSimulationOf(gpWorld& world, ezTime dt);
    friend gpEntityDrawInfo*& gpDefaultDrawInfoPtrOf(gpWorld& world);
    friend GP_CoreAPI void gpExtractRenderDataOf(const gpWorld& world, gpRenderExtractor* pExtractor);
    template<typename Type>
    friend Type* gpCreateEntityIn(gpWorld& world);
};

EZ_FORCE_INLINE const ezString& gpNameOf(const gpWorld& world) { return world.m_sName; }
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
GP_CoreAPI void gpCollectGarbageOf(gpWorld& world);
GP_CoreAPI void gpStepSimulationOf(gpWorld& world, ezTime dt);

template<typename Type>
EZ_FORCE_INLINE Type* gpCreateEntityIn(gpWorld& world)
{
    EZ_PROFILE(world.m_ProfilingId_CreateEntity);
    auto pEntity = EZ_DEFAULT_NEW(Type);
    world.InsertCreatedEntity(pEntity);
    return pEntity;
}

EZ_FORCE_INLINE gpEntityDrawInfo*& gpDefaultDrawInfoPtrOf(gpWorld& world) { return world.m_pEntityDrawInfoDefault; }
EZ_FORCE_INLINE gpEntityDrawInfo*  gpDefaultDrawInfoPtrOf(const gpWorld& world)
{
    return gpDefaultDrawInfoPtrOf(const_cast<gpWorld&>(world));
}
GP_CoreAPI void gpExtractRenderDataOf(const gpWorld& world, gpRenderExtractor* pExtractor);
