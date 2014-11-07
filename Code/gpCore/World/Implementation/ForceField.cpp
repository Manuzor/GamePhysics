#include "gpCore/PCH.h"
#include "gpCore/World/ForceField.h"

gpForceFieldEntity::gpForceFieldEntity(float fForce, float fRadius) :
    gpEntityBase(gpEntityType::ForceField),
    m_fForce(fForce),
    m_Area(fRadius)
{
}
