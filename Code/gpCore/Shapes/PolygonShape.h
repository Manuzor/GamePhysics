#pragma once

#include <gpCore/Shapes/ShapeBase.h>

/// \brief Used to create a gpShape as an arbitrary polygon shape.
/// \example gpVec3 vertices[] = { {0, 0, 0}, {1, 1, 1}, /* ... */ };
///          auto pPolygon = gpNew<gpPolygonShape>(vertices);
class gpPolygonShape : public gpShape
{
public: // Data Types
    using VertexContainer = ezHybridArray<gpVec3, 8>;

private: // Data
    VertexContainer m_vertices;

public: // Friends
    /// \brief Returns all vertices of the polygon shape.
    friend VertexContainer& gpVerticesOf(gpShape& polygon);

    /// \brief Returns all vertices of the polygon shape.
    friend const VertexContainer& gpVerticesOf(const gpShape& polygon);

    /// \brief Converts any polygon to a box with 4 vertices.
    friend void gpConvertToBox(gpShape& polygon, const gpVec3& halfExtends);

public: // Construction
    gpPolygonShape() : gpShape(gpShapeType::Polygon) {}
};

#include <gpCore/Shapes/Implementation/Polygon.inl>
