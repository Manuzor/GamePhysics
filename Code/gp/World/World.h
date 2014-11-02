#pragma once
#include "gp/World/EntityBase.h"

class gpRenderExtractor;

class gpWorld
{
public:
    gpWorld();
    gpWorld(const gpWorld&) = delete;
    void operator=(const gpWorld&) = delete;
    ~gpWorld();

    template<typename Type>
    Type* CreateEntity()
    {
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

private:
    gpVec3 m_Gravity;
    ezDynamicArray<gpEntityBase*> m_CreatedEntities;
    ezDynamicArray<gpEntityBase*> m_SimulatedEntities;
    ezMap<gpEntityBase*, gpEntityDrawInfo> m_EntityDrawInfos;

private:
    void InsertCreatedEntity(gpEntityBase* pEntity);
};
