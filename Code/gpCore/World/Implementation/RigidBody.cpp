#include "gpCore/PCH.h"
#include "gpCore/World/RigidBody.h"

gpRigidBody::gpRigidBody() :
    gpEntityBase(gpEntityType::RigidBody),
    m_pShape(nullptr)
{
}

void gpUpdateStats(const ezStringView sStatName, const gpRigidBody& RigidBody)
{
    gpUpdateStats(sStatName, *RigidBody.GetProperties());
}
