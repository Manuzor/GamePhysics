#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>
#include <Foundation/Communication/GlobalEvent.h>

#include "gpCore/Utilities/EzMathExtensions.h"

#include "gpCore/Algorithm/Integrate.h"

#include "gpCore/World/World.h"
#include "gpCore/World/Entity.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/Utilities/View.h"

#include "gpCore/Dynamics/CollisionDetection.h"

gpWorld::gpWorld(const char* szName) :
    m_sName(szName),
    m_pEntityDrawInfoDefault(&m_EntityDrawInfo_HardDefault)
{
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
    gpClear(self);

    ezProfilingSystem::DeleteId(m_ProfilingId_Extraction);
    ezProfilingSystem::DeleteId(m_ProfilingId_CreateEntity);
    ezProfilingSystem::DeleteId(m_ProfilingId_Simulation);
}

ezResult gpAddTo(gpWorld& world, gpEntity& entity)
{
    if(gpWorldPtrOf(entity) != nullptr)
    {
        EZ_ASSERT(gpWorldPtrOf(entity) == &world,
                  "The entity you tried to add to this world already exists in another world.");
        return EZ_FAILURE;
    }

    gpAddReferenceTo(entity);
    gpWorldPtrOf(entity) = &world;

    world.m_SimulatedEntities.PushBack(AddressOf(entity));

    // Update stats
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", gpNameOf(world).GetData(), gpNameOf(entity).GetData());
        ezStats::SetStat(sbStatName, "");
    }

    return EZ_SUCCESS;
}

ezResult gpRemoveFrom(gpWorld& world, gpEntity& entity)
{
    if (gpWorldPtrOf(entity) != &world)
    {
        EZ_ASSERT(gpWorldPtrOf(entity) == nullptr,
                  "The entity you tried to remove from this world exists in another world!");
        return EZ_FAILURE;
    }

    world.m_SimulatedEntities.RemoveSwap(AddressOf(entity));

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

ezResult gpAddTo(gpWorld& world, gpForceFieldEntity& forceField)
{
    if(gpWorldPtrOf(forceField) != nullptr)
    {
        EZ_ASSERT(gpWorldPtrOf(forceField) == &world,
                  "The entity you tried to add to this world already exists in another world.");
        return EZ_FAILURE;
    }

    gpAddReferenceTo(forceField);
    gpWorldPtrOf(forceField) = &world;

    world.m_ForceFields.PushBack(AddressOf(forceField));

    // Update stats
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", gpNameOf(world).GetData(), gpNameOf(forceField).GetData());
        ezStats::SetStat(sbStatName, "");
    }

    return EZ_SUCCESS;
}

ezResult gpRemoveFrom(gpWorld& world, gpForceFieldEntity& forceField)
{
    if (gpWorldPtrOf(forceField) != &world)
    {
        EZ_ASSERT(gpWorldPtrOf(forceField) == nullptr,
                  "The entity you tried to remove from this world exists in another world!");
        return EZ_FAILURE;
    }

    world.m_ForceFields.RemoveSwap(AddressOf(forceField));

    // Remove draw data if it exists
    {
        auto it = world.m_EntityDrawInfos.Find(AddressOf(forceField));
        if(it.IsValid())
            world.m_EntityDrawInfos.Erase(it);
    }

    // Remove stat
    {
        ezStringBuilder sbStatName;
        sbStatName.Format("%s/%s", gpNameOf(world).GetData(), gpNameOf(forceField).GetData());
        ezStats::RemoveStat(sbStatName);
    }

    gpWorldPtrOf(forceField) = nullptr;
    gpReleaseReferenceTo(forceField);

    return EZ_SUCCESS;
}

void gpClearSimulatedEntities(gpWorld& world)
{
    while(!world.m_SimulatedEntities.IsEmpty())
    {
        gpRemoveFrom(world, Deref(world.m_SimulatedEntities.PeekBack()));
    }
}

void gpClearForceFields(gpWorld& world)
{
    while(!world.m_ForceFields.IsEmpty())
    {
        gpRemoveFrom(world, Deref(world.m_ForceFields.PeekBack()));
    }
}

gpEntityDrawInfo& gpDrawInfoOf(gpWorld& world, gpEntity& entity)
{
    return world.m_EntityDrawInfos[AddressOf(entity)];
}

static void AccumulateForces(gpLinearAcceleration& out_Force,
                             const gpEntity& entity,
                             ezArrayPtr<const gpForceFieldEntity*> ForceFields)
{
    for (ezUInt32 i = 0; i < ForceFields.GetCount(); ++i)
    {
        auto& forceField = Deref(ForceFields[i]);
        if (!gpAreColliding(forceField, entity))
            continue;
        auto vDir = gpValueOf(gpPositionOf(forceField) - gpPositionOf(entity));
        if (vDir.NormalizeIfNotZero().Succeeded())
        {
            out_Force = out_Force + gpLinearAcceleration(vDir * gpForceFactorOf(forceField));
        }
    }
}

void gpStepSimulationOf(gpWorld& world, gpTime dt)
{
    EZ_PROFILE(world.m_ProfilingId_Simulation);

    for(ezUInt32 i = 0; i < world.m_SimulatedEntities.GetCount(); ++i)
    {
        auto& entity = Deref(world.m_SimulatedEntities[i]);

        // Linear movement
        //////////////////////////////////////////////////////////////////////////
        gpLinearAcceleration linAcceleration(gpZero);
        AccumulateForces(linAcceleration, entity, gpGetConstView(world.m_ForceFields));
        linAcceleration = (linAcceleration + gpGravityOf(world)) * gpGravityFactorOf(entity);

        if(!gpIsZero(linAcceleration))
        {
            // v += a * dt
            gpLinearVelocityOf(entity) += linAcceleration * dt;
        }

        // x += v * dt
        gpPositionOf(entity) += gpLinearVelocityOf(entity) * dt;

        // Angular Movement
        //////////////////////////////////////////////////////////////////////////

        auto& A          = gpValueOf(gpRotationOf(entity));
        auto& invI       = gpInverseInertiaOf(entity);
        auto transposedA = gpTransposeOf(A);
        auto wInvI       = gpInverseInertia((gpValueOf(A) * gpValueOf(invI)) * gpValueOf(transposedA));

        // Angular velocity
        auto w = wInvI * gpAngularMomentumOf(entity);

        auto w1 = gpX(w);
        auto w2 = gpY(w);
        auto w3 = gpZ(w);
        // This matrix is used to apply a column-wise cross product to another matrix.
        gpMat3 boxedW(0.0f,  -w3,   w2,
                        w3, 0.0f,  -w1,
                       -w2,   w1, 0.0f);

        auto newA = A + ((boxedW * (gpScalar)gpValueOf(dt)) * A);
        gpOrthogonalize(newA);
        gpRotationOf(entity) = gpOrientation(newA);
    }
}
