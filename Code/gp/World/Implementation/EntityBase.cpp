#include "gp/PCH.h"
#include "gp/World/EntityBase.h"


gpEntityBase::gpEntityBase(gpEntityType TheType) :
    m_Type(TheType),
    m_pWorld(nullptr),
    m_Position(0.0f),
    m_LinearVelocity(0.0f)
{
}

gpEntityBase::~gpEntityBase()
{
}

void gpGetStats(ezStringBuilder& out_Stats, const gpEntityBase* pEntity)
{
    auto pos = pEntity->GetPosition();
    auto vel = pEntity->GetLinearVelocity();
    out_Stats.Format("position = {%3.3f, %3.3f, %3.3f} linearVelocity = {%3.3f, %3.3f, %3.3f}",
                     pos.x, pos.y, pos.z,
                     vel.x, vel.y, vel.z);
}
