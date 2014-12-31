#pragma once

class gpMassUnit
{
    gpScalar m_fValue;

    explicit gpMassUnit(gpScalar fValue) : m_fValue(fValue) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    // NOTE: For some reason, I cannot define the body of this friend function in-line, it won't compile!
    inline friend
    gpMassUnit gpMass(gpScalar fValue);

    inline friend
    gpScalar gpValueOf(gpMassUnit m) { return m.m_fValue; }
};

inline gpMassUnit gpMass(gpScalar fValue) { return gpMassUnit(fValue); }
