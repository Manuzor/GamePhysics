#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Acceleration.h"

/// \brief Force => Mass times Acceleration
class ForceUnit
{
    gpVec3 m_Value;

    ForceUnit(const gpVec3& f) : m_Value{ f } {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE friend
    ForceUnit Force(const gpVec3& Value){ return ForceUnit{ Value }; }

    /// Constructor F = m * a
    EZ_FORCE_INLINE friend
    ForceUnit Force(MassUnit m, const AccelerationUnit& a){ return Force(ValueOf(m) * ValueOf(a)); }

    EZ_FORCE_INLINE friend
    const gpVec3& ValueOf(const ForceUnit& f) { return f.m_Value; }
};
