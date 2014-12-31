#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/Utilities/EzMathExtensions.h"

#include "gpCore/Algorithm/Integrate.h"

#include "gpCore/World/World.h"
#include "gpCore/World/EntityBase.h"
#include "gpCore/World/Particle.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/Utilities/View.h"

gpWorld::gpWorld(const char* szName) :
    m_sName(szName),
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

static void AccumulateForces(gpAccelerationUnit& out_Force,
                             const gpVec3& vEntityPos,
                             ezArrayPtr<const gpForceFieldEntity*> ForceFields)
{
    for (ezUInt32 i = 0; i < ForceFields.GetCount(); ++i)
    {
        auto& forceField = Deref(ForceFields[i]);
        auto bIsAffected = gpContains(forceField, vEntityPos);
        if (!bIsAffected)
            continue;
        auto vDir = gpPositionOf(forceField) - vEntityPos;
        if (vDir.NormalizeIfNotZero().Succeeded())
        {
            out_Force = out_Force + gpAcceleration(vDir * gpForceFactorOf(forceField));
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
        const gpMassUnit& mass = gpMassOf(entity);

        // Linear movement
        //////////////////////////////////////////////////////////////////////////
        auto F = (mass * gpGravityFactorOf(entity)) * gpGravityOf(world);
        if(!ezMath::IsZero(gpValueOf(F).GetLengthSquared()))
        {
            // Coming from: F = m * a
            // => a = F / m
            auto vLinearAcceleration = F / mass;

            AccumulateForces(vLinearAcceleration, gpPositionOf(entity), gpGetConstView(world.m_ForceFields));

            // v += a * dt
            gpLinearVelocityOf(entity) = gpLinearVelocityOf(entity) + (vLinearAcceleration * dt);
        }

        // x += v * dt
        auto newPosition = gpDisplacement(gpPositionOf(entity)) + (gpLinearVelocityOf(entity) * dt);
        gpPositionOf(entity) = gpValueOf(newPosition);

        // Angular Movement
        //////////////////////////////////////////////////////////////////////////

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
