#include "gp/PCH.h"
#include "gp/World/EntityBase.h"


gpEntityBase::gpEntityBase(gpEntityType TheType) :
    m_Type(TheType),
    m_Position(0.0f),
    m_LinearVelocity(0.0f)
{
}

gpEntityBase::~gpEntityBase()
{
}
