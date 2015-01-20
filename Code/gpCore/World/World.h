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

    /// \remark Don't call this directly, use gpNew<gpEntity>(...) instead!
    template<typename... Args>
    gpEntityBase* _NewEntity(Args&&... args)
    {
        /// \todo Use better memory management for entities.
        auto pEntity = EZ_DEFAULT_NEW(gpEntityBase)(args...);
        InsertCreatedEntity(pEntity);
        return pEntity;
    }

private:
    ezString m_sName;
    ezProfilingId m_ProfilingId_Simulation;
    ezProfilingId m_ProfilingId_CreateEntity;
    ezProfilingId m_ProfilingId_Extraction;

    gpLinearAcceleration m_Gravity = gpLinearAcceleration(gpVec3::ZeroVector());
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
    friend gpLinearAcceleration& gpGravityOf(gpWorld& world);
    friend GP_CoreAPI ezResult gpAddEntityTo(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI ezResult gpRemoveEntityFrom(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI void gpClearSimulatedEntities(gpWorld& world);
    friend GP_CoreAPI void gpClearForceFields(gpWorld& world);
    friend GP_CoreAPI gpEntityDrawInfo& gpDrawInfoOf(gpWorld& world, gpEntityBase& entity);
    friend GP_CoreAPI void gpCollectGarbageOf(gpWorld& world);
    friend GP_CoreAPI void gpStepSimulationOf(gpWorld& world, gpTime dt);
    friend gpEntityDrawInfo*& gpDefaultDrawInfoPtrOf(gpWorld& world);
    friend GP_CoreAPI void gpExtractRenderDataOf(const gpWorld& world, gpRenderExtractor* pExtractor);
    template<typename Type>
    friend Type* gpCreateEntity(gpWorld& world);
};

EZ_FORCE_INLINE const ezString& gpNameOf(const gpWorld& world) { return world.m_sName; }
EZ_FORCE_INLINE       gpLinearAcceleration& gpGravityOf(      gpWorld& world) { return world.m_Gravity; }
EZ_FORCE_INLINE const gpLinearAcceleration& gpGravityOf(const gpWorld& world) { return gpGravityOf(const_cast<gpWorld&>(world)); }

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
GP_CoreAPI void gpStepSimulationOf(gpWorld& world, gpTime dt);

template<typename Type>
EZ_FORCE_INLINE Type* gpCreateEntity(gpWorld& world)
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

namespace gpInternal
{
    template<>
    struct gpTypeAllocator<gpEntityBase>
    {
        template<typename... Args>
        gpEntityBase* New(gpWorld& world, Args&&... args)
        {
            return world._NewEntity(args...);
        }
    };

    /// \note Delete & friends are not implemented on purpose.
}
