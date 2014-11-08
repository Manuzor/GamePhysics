#include "gpCore/PCH.h"
#include "gpCore/World/RigidBody.h"

gpRigidBody::gpRigidBody() :
    gpEntityBase(gpEntityType::RigidBody),
    m_pShape(nullptr)
{
}
