#pragma once

#include <gpCore/Shapes/PolygonShape.h>

/// \brief Used to create a gpPolygonShape as a box shape.
/// \example auto pBox = gpNew<gpBoxShape>(/* halfExtents = */ gpVec3(1, 2, 3));
class gpBoxShape : public gpPolygonShape {};

/// \brief Template specialization which takes care of creating box shapes.
template<>
struct GP_CoreAPI gpTypeAllocator<gpBoxShape>
{
    /// \brief Create a new polygon shape as a box.
    /// \note You can convert an existing polygon to a box using gpConvertToBox.
    static gpBoxShape* New(const gpVec3& halfExtents);

    /// \\brief Forwards to gpTypeAllocator<gpBoxShape>::New(gpVec3 halfExtents).
    EZ_FORCE_INLINE static gpBoxShape* New(gpScalar halfWidth, gpScalar halfHeight, gpScalar halfDepth)
    {
        return New(gpVec3(halfWidth, halfHeight, halfDepth));
    }
};
