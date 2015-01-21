#pragma once
#include "gpCore/Shapes/Shape.h"
#include "gpCore/World/PhysicalProperties.h"

class gpWorld;

/// \note By default, every entity is a particle, i.e. has a gpShape of gpShapeType::Point.
///       Turn it into a rigid body by setting the shape to something else than gpShape::Point().
class GP_CoreAPI gpEntity : public ezRefCounted
{
private: // Data
    gpWorld* m_pWorld = nullptr;
    ezScopedRefPointer<gpShape> m_pShape = nullptr;

    gpPhysicalProperties m_PhysicalProperties;
    ezString m_sName;

public: // Accessors / Friends
    EZ_FORCE_INLINE friend       gpWorld*& gpWorldPtrOf(      gpEntity& entity) { return entity.m_pWorld; }
    EZ_FORCE_INLINE friend const gpWorld*  gpWorldPtrOf(const gpEntity& entity) { return entity.m_pWorld; }

    EZ_FORCE_INLINE friend       ezString& gpNameOf(      gpEntity& entity) { return entity.m_sName; }
    EZ_FORCE_INLINE friend const ezString& gpNameOf(const gpEntity& entity) { return entity.m_sName; }

    EZ_FORCE_INLINE friend       gpPhysicalProperties& gpPhysicalPropertiesOf(      gpEntity& entity) { return entity.m_PhysicalProperties; }
    EZ_FORCE_INLINE friend const gpPhysicalProperties& gpPhysicalPropertiesOf(const gpEntity& entity) { return entity.m_PhysicalProperties; }

    EZ_FORCE_INLINE friend ezScopedRefPointer<gpShape>& gpShapePtrOf(      gpEntity& entity) { return entity.m_pShape; }
    EZ_FORCE_INLINE friend const gpShape*               gpShapePtrOf(const gpEntity& entity) { return entity.m_pShape; }

    /// \remark Assumes a valid shape ptr is set
    EZ_FORCE_INLINE friend gpShape& gpShapeOf(gpEntity& entity)
    {
        EZ_ASSERT(entity.m_pShape, "");
        return Deref(entity.m_pShape);
    }

    /// \remark Assumes a valid shape ptr is set
    EZ_FORCE_INLINE friend const gpShape& gpShapeOf(const gpEntity& entity)
    {
        EZ_ASSERT(entity.m_pShape, "");
        return Deref(entity.m_pShape);
    }

public: // Construction
    gpEntity() : m_pShape(gpShape::Point()) {}

    gpEntity(const gpEntity&)       = delete;
    gpEntity(gpEntity&&)            = delete;
    void operator=(const gpEntity&) = delete;
};

struct gpEntityDrawInfo
{
    ezColor m_Color = { 1, 1, 1, 1 };
    gpScalar m_fScale = 4.0f;
    ezColor m_LinearVelocityColor = { 0, 1, 1, 0.333f };
    ezAngle m_LinearVelocityArrowWingAngle = ezAngle::Degree(30);
    gpScalar m_fLinearVelocityArrowWingLength = 10.0f;
};

struct gpEntityNameComparer
{
    bool Less(const gpEntity& lhs, const gpEntity& rhs) const
    {
        return ezStringUtils::Compare(gpNameOf(lhs).GetData(), gpNameOf(rhs).GetData()) < 0;
    }

    bool Less(gpEntity*& lhs, gpEntity*& rhs) const
    {
        return Less(Deref(lhs), Deref(rhs));
    }

    bool Less(const gpEntity*& lhs, const gpEntity*& rhs) const
    {
        return Less(Deref(lhs), Deref(rhs));
    }

    bool Less(const gpEntity*const & lhs, const gpEntity*const & rhs) const
    {
        return Less(Deref(lhs), Deref(rhs));
    }
};

// Allocation
//////////////////////////////////////////////////////////////////////////
namespace gpInternal
{
    template<>
    struct GP_CoreAPI gpTypeAllocator<gpEntity>
    {
        static gpEntity* New();
    };
}

// Utility functions
//////////////////////////////////////////////////////////////////////////
void gpGetStats(ezStringBuilder& out_Stats, const gpEntity& entity);

EZ_FORCE_INLINE gpTransform& gpTransformOf(      gpEntity& entity)
{
    return gpTransformOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpEntity& entity)
{
    return gpTransformOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpDisplacement& gpPositionOf(gpEntity& entity)
{
    return gpPositionOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpDisplacement& gpPositionOf(const gpEntity& entity)
{
    return gpPositionOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpOrientation& gpRotationOf(gpEntity& entity)
{
    return gpRotationOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpOrientation& gpRotationOf(const gpEntity& entity)
{
    return gpRotationOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpLinearVelocity& gpLinearVelocityOf(gpEntity& entity)
{
    return gpLinearVelocityOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpLinearVelocity& gpLinearVelocityOf(const gpEntity& entity)
{
    return gpLinearVelocityOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpAngularMomentum& gpAngularMomentumOf(gpEntity& entity)
{
    return gpAngularMomentumOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpAngularMomentum& gpAngularMomentumOf(const gpEntity& entity)
{
    return gpAngularMomentumOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpInverseInertia& gpInverseInertiaOf(gpEntity& entity)
{
    return gpInverseInertiaOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpInverseInertia& gpInverseInertiaOf(const gpEntity& entity)
{
    return gpInverseInertiaOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpScalar& gpGravityFactorOf(gpEntity& entity)
{
    return gpGravityFactorOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpEntity& entity)
{
    return gpGravityFactorOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpMassSyncer gpMassOf(gpEntity& entity)
{
    return gpMassOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpMass gpMassOf(const gpEntity& entity)
{
    return gpMassOf(gpPhysicalPropertiesOf(entity));
}

EZ_FORCE_INLINE gpMassSyncer gpInverseMassOf(gpEntity& entity)
{
    return gpInverseMassOf(gpPhysicalPropertiesOf(entity));
}
EZ_FORCE_INLINE const gpMass gpInverseMassOf(const gpEntity& entity)
{
    return gpInverseMassOf(gpPhysicalPropertiesOf(entity));
}

// Algorithms
//////////////////////////////////////////////////////////////////////////

EZ_FORCE_INLINE void gpApplyLinearImpulseTo(gpEntity& entity, const gpLinearVelocity& v)
{
    gpLinearVelocityOf(entity) = gpLinearVelocityOf(entity) + v;
}

EZ_FORCE_INLINE void gpApplyLinearImpulseTo(gpEntity& entity,
                                            const gpLinearVelocity& v,
                                            const gpDisplacement& applicationPosition)
{
    gpLinearVelocityOf(entity) = gpLinearVelocityOf(entity) + v;
}

EZ_FORCE_INLINE void gpApplyForceTo(gpEntity& entity, const gpForce& Force, gpTime dt)
{
    gpMass m = gpMassOf(entity);
    auto a = Force / m;
    auto v = a * dt;
    gpApplyLinearImpulseTo(entity, v);
}

EZ_FORCE_INLINE void gpApplyForceTo(gpEntity& entity,
                                    const gpForce& force,
                                    gpTime dt,
                                    const gpDisplacement& applicationPosition)
{
    auto& rotation = gpValueOf(gpRotationOf(entity));

    // Application point in world coordinates as pseudo-vector.
    auto P = rotation * gpValueOf(applicationPosition);
    // Force pseudo-vector in world coordinates.
    auto F = rotation * gpValueOf(force);

    // Torque as pseudo-vector
    auto result = P.Cross(F);

    if (gpIsZero(result))
        return;

    gpAngularMomentumOf(entity) += gpTorque(result) * dt;
}
