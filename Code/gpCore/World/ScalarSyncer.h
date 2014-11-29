#pragma once

/// Keeps a scalar and its inverse in sync
class gpScalarSyncer
{
public:
    // Data
    //////////////////////////////////////////////////////////////////////////
    gpScalar& m_fData;
    gpScalar& m_fInverseData;

public:
    gpScalarSyncer(gpScalar& fData, gpScalar& fInverseData)
        : m_fData(fData)
        , m_fInverseData(fInverseData)
    {
    }

    void operator =(gpScalar fData) { m_fData = fData; m_fInverseData = ezMath::Invert(fData); }
    operator gpScalar() const { return m_fData; }
};

template<typename T> EZ_FORCE_INLINE
T operator +(const T& lhs, const gpScalarSyncer& fScalar) { return lhs + fScalar.m_fData; }

template<typename T> EZ_FORCE_INLINE
T operator +(const gpScalarSyncer& fScalar, const T& rhs) { return fScalar.m_fData + rhs; }

template<typename T> EZ_FORCE_INLINE
T operator -(const T& lhs, const gpScalarSyncer& fScalar) { return lhs - fScalar.m_fData; }

template<typename T> EZ_FORCE_INLINE
T operator -(const gpScalarSyncer& fScalar, const T& rhs) { return fScalar.m_fData - rhs; }

template<typename T> EZ_FORCE_INLINE
T operator *(const T& lhs, const gpScalarSyncer& fScalar) { return lhs * fScalar.m_fData; }

template<typename T> EZ_FORCE_INLINE
T operator *(const gpScalarSyncer& fScalar, const T& rhs) { return fScalar.m_fData * rhs; }

template<typename T> EZ_FORCE_INLINE
T operator /(const T& lhs, const gpScalarSyncer& fScalar) { return lhs * fScalar.m_fInverseData; }

template<typename T> EZ_FORCE_INLINE
T operator /(const gpScalarSyncer& fScalar, const T& rhs) { return fScalar.m_fInverseData * rhs; }
