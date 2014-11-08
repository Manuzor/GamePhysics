#pragma once
#include "gpCore/Shapes/ShapeType.h"

class GP_CoreAPI gpShapeBase
{
public:
    gpShapeBase(gpShapeType Type) : m_Type(Type) {}

    gpShapeType GetType() const { return m_Type; }

private:
    const gpShapeType m_Type;
};
