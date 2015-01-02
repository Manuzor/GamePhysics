#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"
#include "gpCore/Units/Velocity.h"
#include "gpCore/Units/Acceleration.h"
#include "gpCore/Units/Force.h"

// Scaling
//////////////////////////////////////////////////////////////////////////

// temp
#define GP_DefineUnitScalingOperators(TypeScalarTypeName, TheTypeName, TheConstructorName)                                                   \
EZ_FORCE_INLINE TheTypeName operator +(const TheTypeName& d1, const TheTypeName& d2)    { return TheConstructorName(gpValueOf(d1) + gpValueOf(d2)); } \
EZ_FORCE_INLINE TheTypeName operator -(const TheTypeName& d1, const TheTypeName& d2)    { return TheConstructorName(gpValueOf(d1) - gpValueOf(d2)); } \
EZ_FORCE_INLINE TheTypeName operator *(const TheTypeName& d, TypeScalarTypeName factor) { return TheConstructorName(gpValueOf(d) * factor); }         \
EZ_FORCE_INLINE TheTypeName operator *(TypeScalarTypeName factor, const TheTypeName& d) { return d * factor; }                                        \
EZ_FORCE_INLINE TheTypeName operator /(const TheTypeName& d, TypeScalarTypeName factor) { return d * ezMath::Invert(factor); }

GP_DefineUnitScalingOperators(gpScalar, gpMassUnit,         gpMass)
GP_DefineUnitScalingOperators(gpScalar, gpDisplacementUnit, gpDisplacement)
GP_DefineUnitScalingOperators(gpScalar, gpVelocityUnit,     gpVelocity)
GP_DefineUnitScalingOperators(gpScalar, gpAccelerationUnit, gpAcceleration)
GP_DefineUnitScalingOperators(gpScalar, gpForceUnit,        gpForce)

#undef GP_DefineUnitScalingOperators

EZ_FORCE_INLINE gpTimeUnit operator *(gpTimeUnit lhs, gpTimeUnit rhs) { return gpTimeUnit::Seconds(lhs.GetSeconds() * rhs.GetSeconds()); }
EZ_FORCE_INLINE gpTimeUnit operator *(const gpTimeUnit& d, gpScalar factor) { return gpTime(gpValueOf(d) * factor); }
EZ_FORCE_INLINE gpTimeUnit operator *(gpScalar factor, const gpTimeUnit& d) { return d * factor; }
EZ_FORCE_INLINE gpTimeUnit operator /(gpTimeUnit lhs, gpTimeUnit rhs) { return gpTimeUnit::Seconds(lhs.GetSeconds() / rhs.GetSeconds()); }
EZ_FORCE_INLINE gpTimeUnit operator /(const gpTimeUnit& d, gpScalar factor) { return d * ezMath::Invert(factor); }

// Equality
//////////////////////////////////////////////////////////////////////////

// temp
#define GP_DefineUnitEqualityOperator(TheTypeName, TheScalarType)                                           \
EZ_FORCE_INLINE bool gpAreEqual(const TheTypeName& lhs,                                                     \
                                const TheTypeName& rhs,                                                     \
                                TheScalarType epsilon = ezMath::BasicType<TheScalarType>::DefaultEpsilon()) \
{                                                                                                           \
    return gpAreEqual(gpValueOf(lhs), gpValueOf(rhs), epsilon);                                             \
}

GP_DefineUnitEqualityOperator(gpMassUnit, gpScalar)
GP_DefineUnitEqualityOperator(gpTimeUnit, double)
GP_DefineUnitEqualityOperator(gpDisplacementUnit, gpScalar)
GP_DefineUnitEqualityOperator(gpVelocityUnit, gpScalar)
GP_DefineUnitEqualityOperator(gpAccelerationUnit, gpScalar)
GP_DefineUnitEqualityOperator(gpForceUnit, gpScalar)

#undef GP_DefineUnitEqualityOperator

// Unique Algorithms
//////////////////////////////////////////////////////////////////////////

/// \brief Construct a DisplacementUnit from a VelocityUnit and a given delta-time
/// \example d = v * (dt / t)
inline gpDisplacementUnit operator *(const gpVelocityUnit& v, gpTimeUnit dt)
{
    return gpDisplacement(gpValueOf(v) * (gpScalar)gpValueOf(dt));
}
inline gpDisplacementUnit operator *(gpTimeUnit dt, const gpVelocityUnit& v) { return v * dt; }

inline gpVelocityUnit operator *(const gpAccelerationUnit& a, gpTimeUnit dt)
{
    return gpVelocity(gpValueOf(a) * (gpScalar)gpValueOf(dt));
}
inline gpVelocityUnit operator *(gpTimeUnit dt, const gpAccelerationUnit& a) { return a * dt; }

inline gpForceUnit operator *(gpMassUnit m, const gpAccelerationUnit& a) { return gpForce(m, a); }
inline gpForceUnit operator *(const gpAccelerationUnit& a, gpMassUnit m) { return m * a; }
inline gpAccelerationUnit operator /(const gpForceUnit& f, gpMassUnit m)
{
    return gpAcceleration(gpValueOf(f) * ezMath::Invert(gpValueOf(m)));
}

EZ_FORCE_INLINE gpMassUnit gpInvert(gpMassUnit m) { return gpMass(gpInvert(gpValueOf(m))); }

// Convenience operations
//////////////////////////////////////////////////////////////////////////

// temp
#define GP_DefineVec3WrapperFunctions(TheScalarType, TheTypeName, TheConstructorName, TheZeroConstructorName)                                              \
EZ_FORCE_INLINE TheTypeName TheConstructorName(TheScalarType x, TheScalarType y, TheScalarType z) { return TheConstructorName(gpVec3(x, y, z)); }          \
EZ_FORCE_INLINE TheTypeName TheConstructorName(TheScalarType xyz) { return TheConstructorName(gpVec3(xyz)); }                                              \
EZ_FORCE_INLINE TheTypeName TheZeroConstructorName(TheScalarType x, TheScalarType y, TheScalarType z) { return TheConstructorName(gpVec3::ZeroVector()); } \
EZ_FORCE_INLINE void gpSet(TheTypeName& d, TheScalarType x, TheScalarType y, TheScalarType z) { gpValueOf(d).Set(x, y, z); }                               \
EZ_FORCE_INLINE void gpSet(TheTypeName& d, TheScalarType xyz) { gpValueOf(d).Set(xyz); }                                                                   \
EZ_FORCE_INLINE void gpSetZero(TheTypeName& d) { gpValueOf(d).SetZero(); }                                                                                 \
EZ_FORCE_INLINE TheScalarType& gpX(      TheTypeName& d) { return gpValueOf(d).x; }                                                                        \
EZ_FORCE_INLINE TheScalarType  gpX(const TheTypeName& d) { return gpValueOf(d).x; }                                                                        \
EZ_FORCE_INLINE TheScalarType& gpY(      TheTypeName& d) { return gpValueOf(d).y; }                                                                        \
EZ_FORCE_INLINE TheScalarType  gpY(const TheTypeName& d) { return gpValueOf(d).y; }                                                                        \
EZ_FORCE_INLINE TheScalarType  gpZ(const TheTypeName& d) { return gpValueOf(d).z; }                                                                        \
EZ_FORCE_INLINE TheScalarType& gpZ(      TheTypeName& d) { return gpValueOf(d).z; }

GP_DefineVec3WrapperFunctions(gpScalar, gpDisplacementUnit, gpDisplacement, gpZeroDisplacement)
GP_DefineVec3WrapperFunctions(gpScalar, gpVelocityUnit, gpVelocity, gpZeroVelocity)
GP_DefineVec3WrapperFunctions(gpScalar, gpAccelerationUnit, gpAcceleration, gpZeroAcceleration)
GP_DefineVec3WrapperFunctions(gpScalar, gpForceUnit, gpForce, gpZeroForce)

#undef GP_DefineVec3WrapperFunctions
