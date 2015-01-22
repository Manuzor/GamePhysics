#pragma once

#include <gpCore/Shapes/ShapeBase.h>

/// \brief Used to create a gpShape as a sphere shape.
/// \example auto pSphere = gpNew<gpSphere>(/* radius = */ 1.0f);
class gpSphereShape : public gpShape
{
private: // Data
    gpScalar m_radius;

public: // Friends
    /// \brief Read-Write access to the radius of the \a sphere.
    friend gpScalar& gpRadiusOf(gpShape& sphere);

    /// \brief Read-Only access to the radius of the \a sphere.
    friend gpScalar gpRadiusOf(const gpShape& sphere);

public: // Construction
    gpSphereShape() : gpShape(gpShapeType::Sphere) {}
};

/// \brief Template specialization which takes care of creating sphere shapes.
template<>
struct GP_CoreAPI gpTypeAllocator<gpSphereShape>
{
    /// \brief Creates a sphere with the given radius
    static gpSphereShape* New(gpScalar radius);
};

// Sphere implementation details.
#include <gpCore/Shapes/Implementation/Sphere.inl>
