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
        {
            ezFoundation::GetDefaultAllocator()->Deallocate(pEntity);
        }
    }
}

ezResult gpWorld::AddEntity(gpEntityBase* pEntity)
{
    if(pEntity->m_pWorld != nullptr)
    {
        EZ_ASSERT(pEntity->m_pWorld == this,
                  "The entity you tried to add to this world already exists in another world.");
        return EZ_FAILURE;
    }

    pEntity->AddRef();
    m_SimulatedEntities.PushBack(pEntity);
    return EZ_SUCCESS;
}

ezResult gpWorld::RemoveEntity(gpEntityBase* pEntity)
{
    if (pEntity->m_pWorld != this)
    {
        EZ_ASSERT(pEntity->m_pWorld == nullptr,
                  "The entity you tried to remove from this world exists in another world!");
        return EZ_FAILURE;
    }

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
        pEntity->m_LinearVelocity = pEntity->GetLinearVelocity() + m_Gravity * fDeltaSeconds;
        pEntity->m_Position = pEntity->GetPosition() + pEntity->m_LinearVelocity * fDeltaSeconds;
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
