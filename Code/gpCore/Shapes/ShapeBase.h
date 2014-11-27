#pragma once
#include "gpCore/Shapes/ShapeType.h"

struct gpShapeBase
{
public:
    gpShapeBase(gpShapeType Type) : m_Type(Type) {}

    // Data
    //////////////////////////////////////////////////////////////////////////
    const gpShapeType m_Type;
};

EZ_FORCE_INLINE gpShapeType gpTypeOf(const gpShapeBase& shape) { return shape.m_Type; }
