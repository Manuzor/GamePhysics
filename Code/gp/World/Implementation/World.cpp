#include "gp/PCH.h"
#include "gp/World/World.h"
#include "gp/World/EntityBase.h"
#include "gp/World/Particle.h"

gpWorld::gpWorld() :
    m_Gravity(0, 0, 0)
{
    m_CreatedEntities.Reserve(64);
    m_SimulatedEntities.Reserve(64);
}

gpWorld::~gpWorld()
{
    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_CreatedEntities[i];
        if(pEntity)
            ezFoundation::GetDefaultAllocator()->Deallocate(pEntity);
    }
}

ezResult gpWorld::AddEntity(gpEntityBase* pEntity)
{
    if (m_SimulatedEntities.Contains(pEntity))
        return EZ_FAILURE;
    pEntity->AddRef();
    m_SimulatedEntities.PushBack(pEntity);
    return EZ_SUCCESS;
}

ezResult gpWorld::RemoveEntity(gpEntityBase* pEntity)
{
    if (!m_SimulatedEntities.Contains(pEntity))
        return EZ_FAILURE;
    m_SimulatedEntities.RemoveSwap(pEntity);
    pEntity->ReleaseRef();
    return EZ_SUCCESS;
}

gpEntityDrawInfo& gpWorld::GetEntityDrawInfo(gpEntityBase* pEntity)
{
    return m_EntityDrawInfos[pEntity];
}

void gpWorld::StepSimulation(ezTime dt)
{
    auto fDeltaSeconds = (float)dt.GetSeconds();
    for(ezUInt32 i = 0; i < m_SimulatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_SimulatedEntities[i];
        auto Velocity = pEntity->GetLinearVelocity() + m_Gravity * fDeltaSeconds;
        auto Position = pEntity->GetPosition() + Velocity * fDeltaSeconds;
        pEntity->SetPosition(Position);
    }
}

void gpWorld::InsertCreatedEntity(gpEntityBase* pEntity)
{
    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        if (m_CreatedEntities[i] == nullptr)
        {
            m_CreatedEntities.Insert(pEntity, i);
            return;
        }
    }

    m_CreatedEntities.PushBack(pEntity);
}

void gpWorld::CollectGarbage()
{
    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        auto& pEntity = m_CreatedEntities[i];
        if (pEntity && !pEntity->IsReferenced())
        {
            EZ_DEFAULT_DELETE(pEntity);
        }
    }
}
