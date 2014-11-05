#pragma once
#include "gp/World/EntityBase.h"

class gpRenderExtractor;

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

    ezResult AddEntity(gpEntityBase* pEntity);
    ezResult RemoveEntity(gpEntityBase* pEntity);

    gpEntityDrawInfo& GetEntityDrawInfo(gpEntityBase* pEntity);

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

    gpVec3 m_Gravity;
    ezDynamicArray<gpEntityBase*> m_CreatedEntities;
    ezDynamicArray<gpEntityBase*> m_SimulatedEntities;
    ezMap<gpEntityBase*, gpEntityDrawInfo> m_EntityDrawInfos;
    gpEntityDrawInfo m_EntityDrawInfo_HardDefault;
    gpEntityDrawInfo* m_pEntityDrawInfoDefault;

private:
    void InsertCreatedEntity(gpEntityBase* pEntity);
};
