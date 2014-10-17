#pragma once

class gpCircleShape
{
public:

    gpCircleShape();
    gpCircleShape(gpScalar fRadius);

private:
    gpScalar m_fRadius;
};

#include "gp/Shapes/Implementation/Circle.inl"
