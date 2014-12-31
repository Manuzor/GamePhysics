#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Acceleration => Distance per Squared-Time
/// \example m/(s*s) (meter per second per second)
class gpAccelerationUnit
{
    gpVec3 m_Value;

    gpAccelerationUnit(const gpVec3& Value) : m_Value{ Value } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    gpAccelerationUnit gpAcceleration(const gpVec3& Value) { return gpAccelerationUnit{ Value }; }

    /// Constructor
    EZ_FORCE_INLINE friend
    gpAccelerationUnit gpAcceleration(const gpDisplacementUnit& d, gpTimeUnit t)
    {
        return gpAccelerationUnit{ gpValueOf(d) * (gpScalar)ezMath::Invert(gpValueOf(t) * gpValueOf(t)) };
    }

    EZ_FORCE_INLINE friend
    const gpVec3& gpValueOf(const gpAccelerationUnit& a) { return a.m_Value; }
};
