#include "gpCore/PCH.h"
#include "gpCore/World/Particle.h"

gpParticleEntity::gpParticleEntity() :
    gpEntityBase(gpEntityType::Particle)
{
}

void gpUpdateStats(const ezStringView sStatName, const gpParticleEntity& Particle)
{
    gpUpdateStats(sStatName, *Particle.GetProperties());
}
