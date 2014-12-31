#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Represents d / t, d: displacement, t: time => Distance per Time unit
/// \example m/s (meter per second)
class gpVelocityUnit
{
    gpVec3 m_Value;

    gpVelocityUnit(const gpVec3& Value) : m_Value{ Value } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    gpVelocityUnit gpVelocity(const gpVec3& Value) { return gpVelocityUnit{ Value }; }

    /// Constructor
    EZ_FORCE_INLINE friend
    gpVelocityUnit gpVelocity(const gpDisplacementUnit& d, gpTimeUnit t)
    {
        return gpVelocity(gpValueOf(d) * (gpScalar)ezMath::Invert(gpValueOf(t)));
    }

    EZ_FORCE_INLINE friend
    const gpVec3& gpValueOf(const gpVelocityUnit& v) { return v.m_Value; }
};
