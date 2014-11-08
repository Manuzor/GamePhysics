#pragma once
#include "gpCore/World/EntityBase.h"

class GP_CoreAPI gpParticleEntity : public gpEntityBase
{
public:
    gpParticleEntity();
private:
};

GP_CoreAPI void gpUpdateStats(const ezStringView sStatName, const gpParticleEntity& Particle);
