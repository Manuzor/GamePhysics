#pragma once

class MassUnit
{
    gpScalar m_fValue;

    explicit MassUnit(gpScalar fValue) : m_fValue(fValue) {}

    // Friends
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    // NOTE: For some reason, I cannot define the body of this friend function in-line, it won't compile!
    inline friend
    MassUnit Mass(gpScalar fValue);

    inline friend
    gpScalar ValueOf(MassUnit m) { return m.m_fValue; }
};

inline MassUnit Mass(gpScalar fValue) { return MassUnit(fValue); }
