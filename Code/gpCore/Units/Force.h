#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Acceleration.h"

/// \brief Force => Mass times Acceleration
class gpForceUnit
{
    gpVec3 m_Value;

    gpForceUnit(const gpVec3& f) : m_Value{ f } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE friend
    gpForceUnit gpForce(const gpVec3& Value){ return gpForceUnit{ Value }; }

    /// Constructor F = m * a
    EZ_FORCE_INLINE friend
    gpForceUnit gpForce(gpMassUnit m, const gpAccelerationUnit& a){ return gpForce(gpValueOf(m) * gpValueOf(a)); }

    EZ_FORCE_INLINE friend       gpVec3& gpValueOf(      gpForceUnit& f) { return f.m_Value; }
    EZ_FORCE_INLINE friend const gpVec3& gpValueOf(const gpForceUnit& f) { return f.m_Value; }

public:
    gpForceUnit() {}
};
