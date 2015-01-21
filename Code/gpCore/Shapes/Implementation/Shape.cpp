#include "gpCore/PCH.h"
#include "gpCore/Shapes/Shape.h"

gpShapeBase* gpShapeBase::NewSphere(gpScalar radius)
{
    auto pBox = gpNew<gpInternal::gpCircleShapeLayout>();
    gpRadiusOf(Deref(pBox)) = radius;
    return pBox;
}

gpShapeBase* gpShapeBase::NewBox(const gpVec3& halfExtents)
{
    auto pBox = gpNew<gpInternal::gpPolygonShapeLayout>();
    gpConvertToBox(Deref(pBox), halfExtents);
    return pBox;
}
