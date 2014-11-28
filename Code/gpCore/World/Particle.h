#pragma once
#include "gpCore/World/EntityBase.h"

class GP_CoreAPI gpParticleEntity : public gpEntityBase
{
public:
    gpParticleEntity() : gpEntityBase(gpEntityType::Particle) {}
};

EZ_FORCE_INLINE void gpUpdateStats(const ezStringView sStatName, const gpParticleEntity& Particle)
{
    gpUpdateStats(sStatName, gpPhysicalPropertiesOf(Particle));
}
