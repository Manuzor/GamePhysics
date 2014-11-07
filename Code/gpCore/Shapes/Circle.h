#pragma once

class gpCircleShape
{
public:
    EZ_DECLARE_POD_TYPE();

    gpCircleShape();
    gpCircleShape(gpScalar fRadius);

private:
    gpScalar m_fRadius;
};

#include "gpCore/Shapes/Implementation/Circle.inl"