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

ezResult gpWorld::AddEntity(gpEntityBase& entity)
{
    if(gpWorldPtrOf(entity) != nullptr)
    {
        EZ_ASSERT(gpWorldPtrOf(entity) == this,
                  "The entity you tried to add to this world already exists in another world.");
        return EZ_FAILURE;
    }

    switch(gpTypeOf(entity))
    {
    case gpEntityType::ForceField:
        DoAddForceField(static_cast<gpForceFieldEntity&>(entity));
        break;
    default:
        DoAddSimulatedEntity(entity);
        break;
    }

    return EZ_SUCCESS;
}

void gpWorld::DoAddSimulatedEntity(gpEntityBase& entity)
{
    gpAddReference(entity);
    gpWorldPtrOf(entity) = this;

    m_SimulatedEntities.PushBack(AddressOf(entity));

    // Add stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), gpNameOf(entity).GetData());
        ezStats::SetStat(sbStatName.GetData(), "");
    }
}

void gpWorld::DoAddForceField(gpForceFieldEntity& forceField)
{
    gpAddReference(forceField);
    gpWorldPtrOf(forceField) = this;

    m_ForceFields.PushBack(AddressOf(forceField));

    // Add stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), gpNameOf(forceField).GetData());
        ezStats::SetStat(sbStatName.GetData(), "");
    }
}

ezResult gpWorld::RemoveEntity(gpEntityBase& entity)
{
    if (gpWorldPtrOf(entity) != this)
    {
        EZ_ASSERT(gpWorldPtrOf(entity) == nullptr,
                  "The entity you tried to remove from this world exists in another world!");
        return EZ_FAILURE;
    }

    switch(gpTypeOf(entity))
    {
    case gpEntityType::ForceField:
        DoRemoveForceField(static_cast<gpForceFieldEntity&>(entity));
        break;
    default:
        DoRemoveSimulatedEntity(entity);
        break;
    }

    return EZ_SUCCESS;
}

void gpWorld::DoRemoveSimulatedEntity(gpEntityBase& entity)
{
    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), gpNameOf(entity).GetData());
        ezStats::RemoveStat(sbStatName.GetData());
    }

    // Remove draw data if it exists
    {
        auto it = m_EntityDrawInfos.Find(AddressOf(entity));
        if(it.IsValid())
            m_EntityDrawInfos.Erase(it);
    }

    m_SimulatedEntities.RemoveSwap(AddressOf(entity));

    gpWorldPtrOf(entity) = nullptr;
    gpReleaseReference(entity);
}

void gpWorld::DoRemoveForceField(gpForceFieldEntity& forceField)
{
    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", m_sName.GetData(), gpNameOf(forceField).GetData());
        ezStats::RemoveStat(sbStatName.GetData());
    }

    // Remove draw data if it exists
    {
        auto it = m_EntityDrawInfos.Find(AddressOf(forceField));
        if(it.IsValid())
            m_EntityDrawInfos.Erase(it);
    }

    m_ForceFields.RemoveSwap(AddressOf(forceField));

    gpWorldPtrOf(forceField) = nullptr;
    gpReleaseReference(forceField);
}

void gpWorld::ClearSimulatedEntities()
{
    while(!m_SimulatedEntities.IsEmpty())
    {
        DoRemoveSimulatedEntity(Deref(m_SimulatedEntities.PeekBack()));
    }
}

void gpWorld::ClearForceFields()
{
    while(!m_ForceFields.IsEmpty())
    {
        DoRemoveForceField(Deref(m_ForceFields.PeekBack()));
    }
}

gpEntityDrawInfo& gpWorld::GetEntityDrawInfo(gpEntityBase& entity)
{
    return m_EntityDrawInfos[AddressOf(entity)];
}

static void AccumulateForces(gpVec3& out_Force, const gpPhysicalProperties& props , ezArrayPtr<const gpForceFieldEntity*> ForceFields)
{
    GP_OnScopeExit { out_Force *= gpGravityFactorOf(props); };

    if (ezMath::IsEqual(gpGravityFactorOf(props), 0.0f))
        return;

    for (ezUInt32 i = 0; i < ForceFields.GetCount(); ++i)
    {
        auto& forceField = Deref(ForceFields[i]);
        auto bIsAffected = gpContains(forceField, gpPositionOf(props));
        if (!bIsAffected)
            continue;
        auto vDir = gpPositionOf(forceField) - gpPositionOf(props);
        if (vDir.NormalizeIfNotZero().Succeeded())
        {
            out_Force += vDir * gpForceOf(forceField);
        }
    }
}

void gpWorld::StepSimulation(ezTime dt)
{
    EZ_PROFILE(m_ProfilingId_Simulation);

    auto fDeltaSeconds = (float)dt.GetSeconds();
    for(ezUInt32 i = 0; i < m_SimulatedEntities.GetCount(); ++i)
    {
        auto& entity = Deref(m_SimulatedEntities[i]);

        gpVec3 F = m_Gravity;
        AccumulateForces(F, gpPhysicalPropertiesOf(entity), gpGetConstView(m_ForceFields));

        gpLinearVelocityOf(entity) = gpLinearVelocityOf(entity) + F * fDeltaSeconds;
        gpPositionOf(entity) = gpPositionOf(entity) + gpLinearVelocityOf(entity) * fDeltaSeconds;
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
