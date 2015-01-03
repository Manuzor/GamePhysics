#pragma once
#include "gpCore/Units/Displacement.h"
#include "gpCore/Units/Time.h"

/// \brief Represents d / t, d: displacement, t: time => Distance per Time unit
/// \example m/s (meter per second)
class gpVelocity
{
    gpVec3 m_Value;

    gpVelocity(const gpVec3& Value) : m_Value{ Value } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    EZ_FORCE_INLINE friend
    gpVelocity gpVelocity(const gpVec3& Value) { return gpVelocity{ Value }; }

    /// Constructor
    EZ_FORCE_INLINE friend
    gpVelocity gpVelocity(const gpDisplacement& d, gpTime t)
    {
        return gpVelocity(gpValueOf(d) * (gpScalar)ezMath::Invert(gpValueOf(t)));
    }

    EZ_FORCE_INLINE friend       gpVec3& gpValueOf(      gpVelocity& v) { return v.m_Value; }
    EZ_FORCE_INLINE friend const gpVec3& gpValueOf(const gpVelocity& v) { return v.m_Value; }

public:
    gpVelocity() {}
};
