#pragma once

/// Keeps a scalar and its inverse in sync
template<typename ScalarType>
class gpScalarSyncerTemplate
{
public:
    // Data
    //////////////////////////////////////////////////////////////////////////
    ScalarType& m_fData;
    ScalarType& m_fInverseData;

public:
    gpScalarSyncerTemplate(ScalarType& fData, ScalarType& fInverseData)
        : m_fData(fData)
        , m_fInverseData(fInverseData)
    {
    }

    void operator =(ScalarType fData) { m_fData = fData; m_fInverseData = gpInvert(fData); }
    operator ScalarType() const { return m_fData; }
};

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator +(const T& lhs, const gpScalarSyncerTemplate<ScalarType>& fScalar) { return lhs + fScalar.m_fData; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator +(const gpScalarSyncerTemplate<ScalarType>& fScalar, const T& rhs) { return fScalar.m_fData + rhs; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator -(const T& lhs, const gpScalarSyncerTemplate<ScalarType>& fScalar) { return lhs - fScalar.m_fData; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator -(const gpScalarSyncerTemplate<ScalarType>& fScalar, const T& rhs) { return fScalar.m_fData - rhs; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator *(const T& lhs, const gpScalarSyncerTemplate<ScalarType>& fScalar) { return lhs * fScalar.m_fData; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator *(const gpScalarSyncerTemplate<ScalarType>& fScalar, const T& rhs) { return fScalar.m_fData * rhs; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator /(const T& lhs, const gpScalarSyncerTemplate<ScalarType>& fScalar) { return lhs * fScalar.m_fInverseData; }

template<typename T, typename ScalarType> EZ_FORCE_INLINE
T operator /(const gpScalarSyncerTemplate<ScalarType>& fScalar, const T& rhs) { return fScalar.m_fInverseData * rhs; }

using gpScalarSyncer = gpScalarSyncerTemplate<gpScalar>;
