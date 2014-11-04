#pragma once
#include "gp/World/EntityType.h"

class gpWorld;

struct gpEntityProperties
{
    gpVec3 m_Position = { 0, 0, 0 };
    gpVec3 m_LinearVelocity = { 0, 0, 0 };
    gpScalar m_fGravityFactor = 1.0f;
};

class gpEntityBase : public ezRefCounted
{
    friend gpWorld;
public:
    gpEntityBase(gpEntityType TheType);
    ~gpEntityBase();

    /// \brief Gets the physical entity properties of this entity.
    /// \remarks Use this non-const version to set the individual properties.
    EZ_FORCE_INLINE gpEntityProperties* GetProperties() { return &m_Properties; }

    /// \brief Gets a read-only view on the physical entity properties of this entity.
    EZ_FORCE_INLINE const gpEntityProperties* GetProperties() const { return &m_Properties; }

    EZ_FORCE_INLINE gpEntityType GetType() const { return m_Type; }

    EZ_FORCE_INLINE gpWorld* GetWorld() { return m_pWorld; }
    EZ_FORCE_INLINE const gpWorld* GetWorld() const { return m_pWorld; }

    EZ_FORCE_INLINE ezString& GetName() { return m_sName; }
    EZ_FORCE_INLINE const ezString& GetName() const { return m_sName; }
    EZ_FORCE_INLINE void SetName(const ezString& sNewName) { m_sName = sNewName; }
    EZ_FORCE_INLINE void SetName(const char* szNewName) { m_sName = szNewName; }

private:
    const gpEntityType m_Type;
    gpWorld* m_pWorld;

    gpEntityProperties m_Properties;
    ezString m_sName;
};

struct gpEntityDrawInfo
{
    ezColor m_Color = { 1, 1, 1, 1 };
    gpScalar m_fScale = 4.0f;
    ezColor m_LinearVelocityColor = { 0, 1, 1, 0.333f };
    gpScalar m_fLinearVelocityArrowWingScale = 10.0f;
};

void gpGetStats(ezStringBuilder& out_Stats, const gpEntityBase* pEntity);
