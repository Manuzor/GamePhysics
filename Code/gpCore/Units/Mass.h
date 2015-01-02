#pragma once
#include "gpCore/Utilities/ScalarSyncer.h"

class gpMassUnit
{
    gpScalar m_fValue;

    explicit gpMassUnit(gpScalar fValue) : m_fValue(fValue) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    // NOTE: For some reason, I cannot define the body of this friend function in-line, it won't compile!
    EZ_FORCE_INLINE friend
    gpMassUnit gpMass(gpScalar fValue);

    EZ_FORCE_INLINE friend       gpScalar& gpValueOf(      gpMassUnit& m) { return m.m_fValue; }
    EZ_FORCE_INLINE friend const gpScalar& gpValueOf(const gpMassUnit& m) { return m.m_fValue; }

public:
    gpMassUnit() {}
};

EZ_FORCE_INLINE gpMassUnit gpMass(gpScalar fValue) { return gpMassUnit(fValue); }

using gpMassSyncer = gpScalarSyncerTemplate<gpMassUnit>;
