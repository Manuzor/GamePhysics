#pragma once
#include "gpCore/World/EntityBase.h"

struct gpShapeBase;

struct GP_CoreAPI gpRigidBody : public gpEntityBase
{
    gpRigidBody() : gpEntityBase(gpEntityType::RigidBody) {}

    // Data
    //////////////////////////////////////////////////////////////////////////
    gpShapeBase* m_pShape = nullptr;
};

EZ_FORCE_INLINE       gpShapeBase*& gpShapePtrOf(      gpRigidBody& rigidBody) { return rigidBody.m_pShape; }
EZ_FORCE_INLINE const gpShapeBase*  gpShapePtrOf(const gpRigidBody& rigidBody) { return rigidBody.m_pShape; }

/// Assumes that the rigid body has a valid shape.
EZ_FORCE_INLINE       gpShapeBase& gpShapeOf(      gpRigidBody& rigidBody) { EZ_ASSERT(gpShapePtrOf(rigidBody), ""); return Deref(gpShapePtrOf(rigidBody)); }
EZ_FORCE_INLINE const gpShapeBase& gpShapeOf(const gpRigidBody& rigidBody) { EZ_ASSERT(gpShapePtrOf(rigidBody), ""); return Deref(gpShapePtrOf(rigidBody)); }

EZ_FORCE_INLINE void gpUpdateStats(const ezStringView sStatName, const gpRigidBody& RigidBody)
{
    gpUpdateStats(sStatName, gpPhysicalPropertiesOf(RigidBody));
}
