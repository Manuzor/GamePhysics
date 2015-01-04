#pragma once

class gpTransform
{
    gpMat3 m_Rotation;
    gpDisplacement m_Position;

    // Friends
    //////////////////////////////////////////////////////////////////////////

    EZ_FORCE_INLINE friend       gpMat3& gpRotationOf(      gpTransform& t) { return t.m_Rotation; }
    EZ_FORCE_INLINE friend const gpMat3& gpRotationOf(const gpTransform& t) { return t.m_Rotation; }

    EZ_FORCE_INLINE friend       gpDisplacement& gpPositionOf(      gpTransform& t) { return t.m_Position; }
    EZ_FORCE_INLINE friend const gpDisplacement& gpPositionOf(const gpTransform& t) { return t.m_Position; }

public:
    gpTransform() {}

    explicit gpTransform(gpNoInitialization){}

    explicit gpTransform(gpZeroInitialization) :
        m_Rotation(gpMat3::ZeroMatrix()),
        m_Position(gpZero)
    {
    }

    explicit gpTransform(gpIdentityInitialization) :
        m_Rotation(gpMat3::IdentityMatrix()),
        m_Position(gpIdentity)
    {
    }

    gpTransform(const gpMat3& Rotation, const gpDisplacement& Position) :
        m_Rotation(Rotation),
        m_Position(Position)
    {
    }
};

EZ_FORCE_INLINE void gpAssertNotNAN(const gpTransform& t)
{
    EZ_NAN_ASSERT(&gpRotationOf(t));
    EZ_NAN_ASSERT(&gpValueOf(gpPositionOf(t)));
}

EZ_FORCE_INLINE
gpMat4 gpAsMat4(const gpTransform& t) { return gpMat4(gpRotationOf(t), gpValueOf(gpPositionOf(t))); }

/// \brief Copies the rotation and position data from \a Transform into \a out_pData
EZ_FORCE_INLINE
void gpAsArray(const gpTransform& Transform, gpScalar* out_pData, ezMatrixLayout::Enum Layout)
{
    gpAssertNotNAN(Transform);
    if (Layout == ezMatrixLayout::ColumnMajor)
    {
        ezMemoryUtils::Copy(out_pData, &gpRotationOf(Transform).m_fElementsCM[0], 12);
    }
    else
    {
        const auto& Rotation = gpRotationOf(Transform);
        const auto& Position = gpValueOf(gpPositionOf(Transform));

        out_pData[0] = Rotation.Element(0, 0);
        out_pData[1] = Rotation.Element(1, 0);
        out_pData[2] = Rotation.Element(2, 0);
        out_pData[3] = Position.x;

        out_pData[4] = Rotation.Element(0, 1);
        out_pData[5] = Rotation.Element(1, 1);
        out_pData[6] = Rotation.Element(2, 1);
        out_pData[7] = Position.y;

        out_pData[8] = Rotation.Element(0, 2);
        out_pData[9] = Rotation.Element(1, 2);
        out_pData[10]= Rotation.Element(2, 2);
        out_pData[11]= Position.z;
    }
}
