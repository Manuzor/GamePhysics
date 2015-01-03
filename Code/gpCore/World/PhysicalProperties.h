#pragma once

struct gpPhysicalProperties
{
    gpTransform    m_Transform       = gpIdentityTransform();
    gpVelocity m_LinearVelocity  = gpVelocity(gpVec3{ 0, 0, 0 });
    gpVelocity m_AngularVelocity = gpVelocity(gpVec3{ 0, 0, 0 });
    gpScalar       m_fGravityFactor  = 1.0f;
    gpMass         m_Mass            = gpMass(1.0f);
    gpMass         m_InverseMass     = gpMass(1.0f);
};

EZ_FORCE_INLINE       gpTransform& gpTransformOf(      gpPhysicalProperties& props) { return props.m_Transform; }
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpPhysicalProperties& props) { return props.m_Transform; }

EZ_FORCE_INLINE       gpDisplacement& gpPositionOf(      gpPhysicalProperties& props) { return gpPositionOf(gpTransformOf(props)); }
EZ_FORCE_INLINE const gpDisplacement& gpPositionOf(const gpPhysicalProperties& props) { return gpPositionOf(gpTransformOf(props)); }

EZ_FORCE_INLINE       gpMat3& gpRotationOf(      gpPhysicalProperties& props) { return gpRotationOf(gpTransformOf(props)); }
EZ_FORCE_INLINE const gpMat3& gpRotationOf(const gpPhysicalProperties& props) { return gpRotationOf(gpTransformOf(props)); }

EZ_FORCE_INLINE       gpVelocity& gpLinearVelocityOf(      gpPhysicalProperties& props) { return props.m_LinearVelocity; }
EZ_FORCE_INLINE const gpVelocity& gpLinearVelocityOf(const gpPhysicalProperties& props) { return props.m_LinearVelocity; }

EZ_FORCE_INLINE       gpVelocity& gpAngularVelocityOf(      gpPhysicalProperties& props) { return props.m_AngularVelocity; }
EZ_FORCE_INLINE const gpVelocity& gpAngularVelocityOf(const gpPhysicalProperties& props) { return props.m_AngularVelocity; }

EZ_FORCE_INLINE       gpScalar& gpGravityFactorOf(      gpPhysicalProperties& props) { return props.m_fGravityFactor; }
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpPhysicalProperties& props) { return props.m_fGravityFactor; }

EZ_FORCE_INLINE       gpMassSyncer gpMassOf(      gpPhysicalProperties& props) { return { props.m_Mass, props.m_InverseMass }; }
EZ_FORCE_INLINE const gpMass&      gpMassOf(const gpPhysicalProperties& props) { return props.m_Mass; }

EZ_FORCE_INLINE       gpMassSyncer gpInverseMassOf(      gpPhysicalProperties& props) { return { props.m_InverseMass, props.m_Mass }; }
EZ_FORCE_INLINE const gpMass&      gpInverseMassOf(const gpPhysicalProperties& props) { return props.m_InverseMass; }

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props);
