#pragma once
#include "gp/World/EntityType.h"

class gpWorld;

class gpEntityBase : public ezRefCounted
{
    friend gpWorld;
public:
    gpEntityBase(gpEntityType TheType);
    ~gpEntityBase();

    EZ_FORCE_INLINE gpVec3 GetPosition() const { return m_Position; }
    EZ_FORCE_INLINE void SetPosition(const gpVec3& NewPosition) { m_Position = NewPosition; }

    EZ_FORCE_INLINE gpVec3 GetLinearVelocity() const { return m_LinearVelocity; }
    EZ_FORCE_INLINE void SetLinearVelocity(const gpVec3& NewVelocity) { m_LinearVelocity = NewVelocity; }

    EZ_FORCE_INLINE gpWorld* GetWorld() { return m_pWorld; }
    EZ_FORCE_INLINE const gpWorld* GetWorld() const { return m_pWorld; }

    EZ_FORCE_INLINE ezString& GetName() { return m_sName; }
    EZ_FORCE_INLINE const ezString& GetName() const { return m_sName; }
    EZ_FORCE_INLINE void SetName(const ezString& sNewName) { m_sName = sNewName; }
    EZ_FORCE_INLINE void SetName(const char* szNewName) { m_sName = szNewName; }

private:
    const gpEntityType m_Type;
    gpWorld* m_pWorld;

    gpVec3 m_Position;
    gpVec3 m_LinearVelocity;
    ezString m_sName;
};

struct gpEntityDrawInfo
{
    ezColor m_Color = { 1, 1, 1, 1 };
};
