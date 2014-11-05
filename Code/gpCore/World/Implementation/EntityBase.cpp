#include "gpCore/PCH.h"
#include "gpCore/World/EntityBase.h"


gpEntityBase::gpEntityBase(gpEntityType TheType) :
    m_Type(TheType),
    m_pWorld(nullptr)
{
}

gpEntityBase::~gpEntityBase()
{
}

void gpGetStats(ezStringBuilder& out_Stats, const gpEntityBase* pEntity)
{
    auto p = pEntity->GetProperties();
    out_Stats.Format("position = {%3.3f, %3.3f, %3.3f} linearVelocity = {%3.3f, %3.3f, %3.3f}",
                     p->m_Position.x, p->m_Position.y, p->m_Position.z,
                     p->m_LinearVelocity.x, p->m_LinearVelocity.y, p->m_LinearVelocity.z);
}
