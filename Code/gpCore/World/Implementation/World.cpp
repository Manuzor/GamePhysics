#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/Utilities/EzMathExtensions.h"

#include "gpCore/World/World.h"
#include "gpCore/World/EntityBase.h"
#include "gpCore/World/Particle.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/Utilities/View.h"

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
    sbProfilingName.Format("%s/%s", m_sName.GetData(), "Extraction");
    m_ProfilingId_Extraction = ezProfilingSystem::CreateId(sbProfilingName.GetData());
}

gpWorld::~gpWorld()
{
    ClearWorld();

    ezProfilingSystem::DeleteId(m_ProfilingId_CreateEntity);
    ezProfilingSystem::DeleteId(m_ProfilingId_Simulation);

    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_CreatedEntities[i];
        if(pEntity)
        {
            EZ_ASSERT(!pEntity->IsReferenced(), "Someone did not release their reference!");
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

    switch(pEntity->GetType())
    {
    case gpEntityType::ForceField:
        DoAddForceField(static_cast<gpForceFieldEntity*>(pEntity));
        break;
    default:
        DoAddSimulatedEntity(pEntity);
        break;
    }

    return EZ_SUCCESS;
}

void gpWorld::DoAddSimulatedEntity(gpEntityBase* pEntity)
{
    pEntity->AddRef();
    pEntity->m_pWorld = this;

    m_SimulatedEntities.PushBack(pEntity);

    // Add stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), pEntity->m_sName.GetData());
        ezStats::SetStat(sbStatName.GetData(), "");
    }
}

void gpWorld::DoAddForceField(gpForceFieldEntity* pForceField)
{
    pForceField->AddRef();
    pForceField->m_pWorld = this;

    m_ForceFields.PushBack(pForceField);

    // Add stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), pForceField->m_sName.GetData());
        ezStats::SetStat(sbStatName.GetData(), "");
    }
}

ezResult gpWorld::RemoveEntity(gpEntityBase* pEntity)
{
    if (pEntity->m_pWorld != this)
    {
        EZ_ASSERT(pEntity->m_pWorld == nullptr,
                  "The entity you tried to remove from this world exists in another world!");
        return EZ_FAILURE;
    }

    switch(pEntity->GetType())
    {
    case gpEntityType::ForceField:
        DoRemoveForceField(static_cast<gpForceFieldEntity*>(pEntity));
        break;
    default:
        DoRemoveSimulatedEntity(pEntity);
        break;
    }

    return EZ_SUCCESS;
}

void gpWorld::DoRemoveSimulatedEntity(gpEntityBase* pEntity)
{
    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), pEntity->m_sName.GetData());
        ezStats::RemoveStat(sbStatName.GetData());
    }

    // Remove draw data if it exists
    {
        auto it = m_EntityDrawInfos.Find(pEntity);
        if(it.IsValid())
            m_EntityDrawInfos.Erase(it);
    }

    m_SimulatedEntities.RemoveSwap(pEntity);

    pEntity->m_pWorld = nullptr;
    pEntity->ReleaseRef();
}

void gpWorld::DoRemoveForceField(gpForceFieldEntity* pForceField)
{
    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), pForceField->m_sName.GetData());
        ezStats::RemoveStat(sbStatName.GetData());
    }

    // Remove draw data if it exists
    {
        auto it = m_EntityDrawInfos.Find(pForceField);
        if(it.IsValid())
            m_EntityDrawInfos.Erase(it);
    }

    m_ForceFields.RemoveSwap(pForceField);

    pForceField->m_pWorld = nullptr;
    pForceField->ReleaseRef();
}

void gpWorld::ClearSimulatedEntities()
{
    while(!m_SimulatedEntities.IsEmpty())
    {
        DoRemoveSimulatedEntity(m_SimulatedEntities.PeekBack());
    }
}

void gpWorld::ClearForceFields()
{
    while(!m_ForceFields.IsEmpty())
    {
        DoRemoveForceField(m_ForceFields.PeekBack());
    }
}

gpEntityDrawInfo& gpWorld::GetEntityDrawInfo(gpEntityBase* pEntity)
{
    return m_EntityDrawInfos[pEntity];
}

static void AccumulateForces(gpVec3& out_Force, const gpPhysicalProperties* pProps , ezArrayPtr<const gpForceFieldEntity*> ForceFields)
{
    GP_OnScopeExit { out_Force *= pProps->m_fGravityFactor; };

    if (ezMath::IsEqual(pProps->m_fGravityFactor, 0.0f))
        return;

    for (ezUInt32 i = 0; i < ForceFields.GetCount(); ++i)
    {
        auto pForceField = ForceFields[i];
        auto bIsAffected = pForceField->Contains(pProps->m_Position);
        if (!bIsAffected)
            continue;
        auto Dir = pForceField->GetProperties()->m_Position - pProps->m_Position;
        if (Dir.NormalizeIfNotZero().Succeeded())
        {
            out_Force += Dir * pForceField->GetForce();
        }
    }
}

void gpWorld::StepSimulation(ezTime dt)
{
    EZ_PROFILE(m_ProfilingId_Simulation);

    auto fDeltaSeconds = (float)dt.GetSeconds();
    for(ezUInt32 i = 0; i < m_SimulatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_SimulatedEntities[i];
        auto pProps = pEntity->GetProperties();

        gpVec3 F = m_Gravity;
        AccumulateForces(F, pProps, gpGetConstView(m_ForceFields));

        pProps->m_LinearVelocity = pProps->m_LinearVelocity + F * fDeltaSeconds;
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
