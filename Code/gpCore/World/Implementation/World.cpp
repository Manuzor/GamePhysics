#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/World/World.h"
#include "gpCore/World/EntityBase.h"
#include "gpCore/World/Particle.h"

gpWorld::gpWorld(const char* szName) :
    m_sName(szName),
    m_Gravity(0.0f, 0.0f, 0.0f),
    m_pEntityDrawInfoDefault(&m_EntityDrawInfo_HardDefault)
{
    m_CreatedEntities.Reserve(64);
    m_SimulatedEntities.Reserve(64);

    ezStringBuilder sbProfilingName;
    sbProfilingName.Format("%s/%s", m_sName.GetData(), "Simulation");
    m_ProfilingId_Simulation = ezProfilingSystem::CreateId(sbProfilingName.GetData());
    sbProfilingName.Format("%s/%s", m_sName.GetData(), "CreateEntity");
    m_ProfilingId_CreateEntity = ezProfilingSystem::CreateId(sbProfilingName.GetData());
}

gpWorld::~gpWorld()
{
    ezProfilingSystem::DeleteId(m_ProfilingId_CreateEntity);
    ezProfilingSystem::DeleteId(m_ProfilingId_Simulation);

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
    pEntity->m_pWorld = this;
    m_SimulatedEntities.PushBack(pEntity);

    // Add stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), pEntity->m_sName.GetData());
        ezStats::SetStat(sbStatName.GetData(), "<Added>");
    }
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

    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), pEntity->m_sName.GetData());
        ezStats::RemoveStat(sbStatName.GetData());
    }

    // Remove draw data.
    {
        auto it = m_EntityDrawInfos.Find(pEntity);
        if(it.IsValid())
            m_EntityDrawInfos.Erase(it);
    }

    m_SimulatedEntities.RemoveSwap(pEntity);
    pEntity->m_pWorld = nullptr;
    pEntity->ReleaseRef();
    return EZ_SUCCESS;
}

gpEntityDrawInfo& gpWorld::GetEntityDrawInfo(gpEntityBase* pEntity)
{
    return m_EntityDrawInfos[pEntity];
}

void gpWorld::StepSimulation(ezTime dt)
{
    EZ_PROFILE(m_ProfilingId_Simulation);

    auto fDeltaSeconds = (float)dt.GetSeconds();
    for(ezUInt32 i = 0; i < m_SimulatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_SimulatedEntities[i];
        auto pProps = pEntity->GetProperties();
        auto G = m_Gravity * pProps->m_fGravityFactor;
        pProps->m_LinearVelocity = pProps->m_LinearVelocity + G * fDeltaSeconds;
        pProps->m_Position = pProps->m_Position + pProps->m_LinearVelocity * fDeltaSeconds;
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
