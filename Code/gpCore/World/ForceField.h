#pragma once
#include "gpCore/World/EntityBase.h"
#include "gpCore/Shapes/Circle.h"

class GP_CoreAPI gpForceFieldEntity : public gpEntityBase
{
public:
    gpForceFieldEntity(float fForce, float fRadius);
private:

    float m_fForce;
    gpCircleShape m_Area;
};
