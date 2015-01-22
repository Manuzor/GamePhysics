#pragma once

#include <gpCore/Shapes/ShapeBase.h>

/// \brief Used to create a gpShape as a point shape.
/// \example auto pPoint = gpNew<gpPointShape>(); // No args.
class gpPointShape : public gpShape
{
public: // Construction
    gpPointShape() : gpShape(gpShapeType::Point) {}
};

/// \brief Template specialization which takes care of creating point shapes.
/// \note There actually exists exactly 1 point shape, so calls to gpNew<gpPointShape>() will always return the same pointer.
template<>
struct GP_CoreAPI gpTypeAllocator<gpPointShape>
{
    /// \brief Creates a point shape.
    static gpPointShape* New();
};
