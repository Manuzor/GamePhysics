#pragma once
#include "gpCore/World/ScalarSyncer.h"

struct gpPhysicalProperties
{
#if __INTELLISENSE__
    gpTransform m_Transform;
#else
    gpTransform m_Transform = { gpVec3::ZeroVector(), gpQuaternion::IdentityQuaternion() };
#endif
    gpVec3 m_LinearVelocity = { 0, 0, 0 };
    gpVec3 m_AngularVelocity = { 0, 0, 0 };
    gpScalar m_fGravityFactor = 1.0f;
    gpScalar m_fMass = 1.0f;
    gpScalar m_fInverseMass = 1.0f;
};

EZ_FORCE_INLINE       gpTransform& gpTransformOf(      gpPhysicalProperties& props) { return props.m_Transform; }
EZ_FORCE_INLINE const gpTransform& gpTransformOf(const gpPhysicalProperties& props) { return props.m_Transform; }

EZ_FORCE_INLINE       gpVec3& gpPositionOf(      gpPhysicalProperties& props) { return gpTransformOf(props).m_vPosition; }
EZ_FORCE_INLINE const gpVec3& gpPositionOf(const gpPhysicalProperties& props) { return gpTransformOf(props).m_vPosition; }

EZ_FORCE_INLINE       gpMat3& gpRotationOf(      gpPhysicalProperties& props) { return gpTransformOf(props).m_Rotation; }
EZ_FORCE_INLINE const gpMat3& gpRotationOf(const gpPhysicalProperties& props) { return gpTransformOf(props).m_Rotation; }

EZ_FORCE_INLINE       gpVec3& gpLinearVelocityOf(      gpPhysicalProperties& props) { return props.m_LinearVelocity; }
EZ_FORCE_INLINE const gpVec3& gpLinearVelocityOf(const gpPhysicalProperties& props) { return props.m_LinearVelocity; }

EZ_FORCE_INLINE       gpVec3& gpAngularVelocityOf(      gpPhysicalProperties& props) { return props.m_AngularVelocity; }
EZ_FORCE_INLINE const gpVec3& gpAngularVelocityOf(const gpPhysicalProperties& props) { return props.m_AngularVelocity; }

EZ_FORCE_INLINE       gpScalar& gpGravityFactorOf(      gpPhysicalProperties& props) { return props.m_fGravityFactor; }
EZ_FORCE_INLINE const gpScalar& gpGravityFactorOf(const gpPhysicalProperties& props) { return props.m_fGravityFactor; }

EZ_FORCE_INLINE       gpScalarSyncer gpMassOf(      gpPhysicalProperties& props) { return { props.m_fMass, props.m_fInverseMass }; }
EZ_FORCE_INLINE const gpScalarSyncer gpMassOf(const gpPhysicalProperties& props) { return gpMassOf(const_cast<gpPhysicalProperties&>(props)); }

EZ_FORCE_INLINE       gpScalarSyncer gpInverseMassOf(      gpPhysicalProperties& props) { return { props.m_fInverseMass, props.m_fMass }; }
EZ_FORCE_INLINE const gpScalarSyncer gpInverseMassOf(const gpPhysicalProperties& props) { return gpInverseMassOf(const_cast<gpPhysicalProperties&>(props)); }

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props);
