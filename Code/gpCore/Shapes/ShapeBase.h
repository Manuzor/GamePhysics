#pragma once
#include "gpCore/Shapes/ShapeType.h"

class gpShapeBase
{
public:
    gpShapeBase(gpShapeType Type) : m_Type(Type) {}
    gpShapeBase(const gpShapeBase&) = delete;
    void operator=(const gpShapeBase&) = delete;

private:
    // Data
    //////////////////////////////////////////////////////////////////////////
    const gpShapeType m_Type;

    // Friends
    //////////////////////////////////////////////////////////////////////////
    friend gpShapeType gpTypeOf(const gpShapeBase& shape);
};

EZ_FORCE_INLINE gpShapeType gpTypeOf(const gpShapeBase& shape) { return shape.m_Type; }
