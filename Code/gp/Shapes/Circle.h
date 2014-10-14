#pragma once

template<typename ScalarType>
class gpCircleShapeTemplate
{
public:

    gpCircleShapeTemplate();
    gpCircleShapeTemplate(ScalarType fRadius);

private:
    ScalarType m_fRadius;
};

#include "gp/Shapes/Implementation/Circle.inl"

using gpCircleShape = gpCircleShapeTemplate<gpScalar>;
