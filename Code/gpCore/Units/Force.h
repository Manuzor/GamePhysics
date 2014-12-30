#pragma once
#include "gpCore/Units/Mass.h"
#include "gpCore/Units/Acceleration.h"

/// \brief Force => Mass times Acceleration
class ForceUnit
{
    MassUnit m_Mass;
    AccelerationUnit m_Acceleration;

    ForceUnit(MassUnit m, const AccelerationUnit& a) : m_Mass(m), m_Acceleration(a) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor F = m * a
    friend
    ForceUnit Force(MassUnit m, const AccelerationUnit& a);

    EZ_FORCE_INLINE friend
    MassUnit MassOf(const ForceUnit& f) { return f.m_Mass; }

    EZ_FORCE_INLINE friend
    AccelerationUnit AccelerationOf(const ForceUnit& f) { return f.m_Acceleration; }
};

EZ_FORCE_INLINE
ForceUnit Force(MassUnit m, const AccelerationUnit& a) { return { m, a }; }
