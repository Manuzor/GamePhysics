#pragma once
#include "gpCore/World/EntityBase.h"

struct GP_CoreAPI gpParticleEntity : public gpEntityBase
{
    gpParticleEntity() : gpEntityBase(gpEntityType::Particle) {}
};

EZ_FORCE_INLINE void gpUpdateStats(const ezStringView sStatName, const gpParticleEntity& Particle)
{
    gpUpdateStats(sStatName, gpPhysicalPropertiesOf(Particle));
}
