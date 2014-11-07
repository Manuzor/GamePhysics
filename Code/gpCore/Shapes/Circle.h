#pragma once

class GP_CoreAPI gpCircleShape
{
public:
    EZ_DECLARE_POD_TYPE();

    gpCircleShape();
    gpCircleShape(gpScalar fRadius);

private:
    gpScalar m_fRadius;
};

#include "gpCore/Shapes/Implementation/Circle.inl"
