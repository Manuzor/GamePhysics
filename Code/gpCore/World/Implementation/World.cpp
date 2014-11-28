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
    gpClear(Deref(this));

    ezProfilingSystem::DeleteId(m_ProfilingId_Extraction);
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

ezResult gpAddEntityTo(gpWorld& world, gpEntityBase& entity)
{
    if(gpWorldPtrOf(entity) != nullptr)
    {
        EZ_ASSERT(gpWorldPtrOf(entity) == &world,
                  "The entity you tried to add to this world already exists in another world.");
        return EZ_FAILURE;
    }

    gpAddReferenceTo(entity);
    gpWorldPtrOf(entity) = &world;

    switch(gpTypeOf(entity))
    {
    case gpEntityType::ForceField:
    {
        auto& forceField = static_cast<gpForceFieldEntity&>(entity);
        world.m_ForceFields.PushBack(AddressOf(forceField));
        break;
    }
    default:
        world.m_SimulatedEntities.PushBack(AddressOf(entity));
        break;
    }


    // Update stats
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", gpNameOf(world).GetData(), gpNameOf(entity).GetData());
        ezStats::SetStat(sbStatName, "");
    }

    return EZ_SUCCESS;
}

ezResult gpRemoveEntityFrom(gpWorld& world, gpEntityBase& entity)
{
    if (gpWorldPtrOf(entity) != &world)
    {
        EZ_ASSERT(gpWorldPtrOf(entity) == nullptr,
                  "The entity you tried to remove from this world exists in another world!");
        return EZ_FAILURE;
    }

    switch(gpTypeOf(entity))
    {
    case gpEntityType::ForceField:
    {
        auto& forceField = static_cast<gpForceFieldEntity&>(entity);
        world.m_ForceFields.RemoveSwap(AddressOf(forceField));
        break;
    }
    default:
        world.m_SimulatedEntities.RemoveSwap(AddressOf(entity));
        break;
    }

    // Remove draw data if it exists
    {
        auto it = world.m_EntityDrawInfos.Find(AddressOf(entity));
        if(it.IsValid())
            world.m_EntityDrawInfos.Erase(it);
    }

    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", gpNameOf(world).GetData(), gpNameOf(entity).GetData());
        ezStats::RemoveStat(sbStatName);
    }

    gpWorldPtrOf(entity) = nullptr;
    gpReleaseReferenceTo(entity);

    return EZ_SUCCESS;
}

void gpClearSimulatedEntities(gpWorld& world)
{
    while(!world.m_SimulatedEntities.IsEmpty())
    {
        gpRemoveEntityFrom(world, Deref(world.m_SimulatedEntities.PeekBack()));
    }
}

void gpClearForceFields(gpWorld& world)
{
    while(!world.m_ForceFields.IsEmpty())
    {
        gpRemoveEntityFrom(world, Deref(world.m_ForceFields.PeekBack()));
    }
}

gpEntityDrawInfo& gpDrawInfoOf(gpWorld& world, gpEntityBase& entity)
{
    return world.m_EntityDrawInfos[AddressOf(entity)];
}

void gpCollectGarbageOf(gpWorld& world)
{
    for (ezUInt32 i = 0; i < world.m_CreatedEntities.GetCount(); ++i)
    {
        auto& pEntity = world.m_CreatedEntities[i];
        if (pEntity && !pEntity->IsReferenced())
        {
            EZ_DEFAULT_DELETE(pEntity);
        }
    }
}

void gpStepSimulationOf(gpWorld& world, ezTime dt)
{
    EZ_PROFILE(world.m_ProfilingId_Simulation);

    auto fDeltaSeconds = (float)dt.GetSeconds();
    for(ezUInt32 i = 0; i < world.m_SimulatedEntities.GetCount(); ++i)
    {
        auto& entity = Deref(world.m_SimulatedEntities[i]);

        gpVec3 F = gpGravityOf(world);
        AccumulateForces(F, gpPhysicalPropertiesOf(entity), gpGetConstView(world.m_ForceFields));

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
