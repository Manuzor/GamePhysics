#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Acceleration.h"

/// \brief Force => Mass times Acceleration
class gpForce
{
    gpVec3 m_Value;

    gpForce(const gpVec3& f) : m_Value{ f } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE friend
    gpForce gpForce(const gpVec3& Value){ return gpForce{ Value }; }

    /// Constructor F = m * a
    EZ_FORCE_INLINE friend
    gpForce gpForce(gpMass m, const gpAcceleration& a){ return gpForce(gpValueOf(m) * gpValueOf(a)); }

    EZ_FORCE_INLINE friend       gpVec3& gpValueOf(      gpForce& f) { return f.m_Value; }
    EZ_FORCE_INLINE friend const gpVec3& gpValueOf(const gpForce& f) { return f.m_Value; }

public:
    gpForce() {}
};
