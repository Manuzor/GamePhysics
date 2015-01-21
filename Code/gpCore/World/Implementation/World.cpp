#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>
#include <Foundation/Communication/GlobalEvent.h>
#include <Foundation/Algorithm/Sorting.h>

#include "gpCore/Utilities/EzMathExtensions.h"

#include "gpCore/World/World.h"
#include "gpCore/World/Entity.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/Utilities/View.h"

#include "gpCore/Algorithm/CollisionDetection.h"
#include "gpCore/Algorithm/CollisionResponse.h"

gpWorld::gpWorld(const char* szName) :
    m_sName(szName)
{
    m_SimulatedEntities.Reserve(64);
    m_CollidingBodies.Reserve(64);

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

static void IntegrateEtities(ezArrayPtr<gpEntity*> entities,
                             ezArrayPtr<const gpForceFieldEntity*> forceFields,
                             const gpLinearAcceleration& gravity,
                             gpTime dt)
{
    for (ezUInt32 i = 0; i < entities.GetCount(); ++i)
    {
        auto& entity = Deref(entities[i]);

        // Linear movement
        //////////////////////////////////////////////////////////////////////////
        gpLinearAcceleration linAcceleration(gpZero);
        AccumulateForces(linAcceleration, entity, forceFields);
        linAcceleration = (linAcceleration + gravity) * gpGravityFactorOf(entity);

        if(!gpIsZero(linAcceleration))
        {
            // v += a * dt
            gpLinearVelocityOf(entity) += linAcceleration * dt;
        }

        // x += v * dt
        gpPositionOf(entity) += gpLinearVelocityOf(entity) * dt;

        // Angular Movement
        //////////////////////////////////////////////////////////////////////////

        if (gpTypeOf(gpShapeOf(entity)) == gpShapeType::Point)
        {
            // No need to simulate angular movement for particles.
            continue;
        }

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

template<typename Container>
static void DetectCollision(ezArrayPtr<gpEntity*> entities, Container& out_collidingEntities)
{
    for (ezUInt32 i = 0; i < entities.GetCount(); ++i)
    {
        auto pLeft = entities[i];

        for (ezUInt32 j = i + 1; j < entities.GetCount(); ++j)
        {
            auto pRight = entities[j];

            if (gpAreColliding(Deref(pLeft), Deref(pRight)))
            {
                gpNeedsCollisionResponse(Deref(pLeft))  = true;
                gpNeedsCollisionResponse(Deref(pRight)) = true;
                out_collidingEntities.PushBack({pLeft, pRight});
            }
        }
    }
}

void gpStepSimulationOf(gpWorld& world, gpTime dt)
{
    EZ_PROFILE(world.m_ProfilingId_Simulation);

    // If the number of the simulated entities changed, we have to sort the array.
    if (world.m_SimulatedEntities.GetCount() != world.m_numEntitiesDuringLastSimulation)
    {
        ezSorting::QuickSort(world.m_SimulatedEntities, gpEntityNameComparer());
        world.m_numEntitiesDuringLastSimulation = world.m_SimulatedEntities.GetCount();
    }

    IntegrateEtities(world.m_SimulatedEntities, gpGetConstView(world.m_ForceFields), gpGravityOf(world), dt);

    DetectCollision(world.m_SimulatedEntities, world.m_CollidingBodies);

    // Resolve collisions between bodies in world.m_CollidingBodies...
    for (ezUInt32 i = 0; i < world.m_CollidingBodies.GetCount(); ++i)
    {
        auto& pair  = world.m_CollidingBodies[i];
        auto& left  = Deref(gpFirstOf(pair));
        auto& right = Deref(gpSecondOf(pair));

        bool oneOfTheBodiesWasResolved = !gpNeedsCollisionResponse(left) || !gpNeedsCollisionResponse(right);
        if (oneOfTheBodiesWasResolved)
        {
            // Suppose body A collides with bodies B and C in the same simulation phase,
            // then we resolve for either A-B or A-C, and ignore the other collision
            // in the hopes that it was automagically resolved or will be resolved in the next simulation step.
            continue;
        }

        gpResolveCollision(left, right);
        gpNeedsCollisionResponse(left)  = false;
        gpNeedsCollisionResponse(right) = false;
    }

    world.m_CollidingBodies.Clear();
}
