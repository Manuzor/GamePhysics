#include "gpCore/PCH.h"
#include "gpCore/Shapes/Shape.h"

#include <Foundation/Communication/GlobalEvent.h>

namespace
{
    ezDynamicArray<gpInternal::gpPolygonShapeLayout*> g_polygons;
    ezDynamicArray<gpInternal::gpCircleShapeLayout*>  g_spheres;
}

gpShapeBase* gpShapeBase::NewSphere(gpScalar radius)
{
    auto pShere = gpNew<gpInternal::gpCircleShapeLayout>();
    gpRadiusOf(Deref(pShere)) = radius;
    g_spheres.PushBack(pShere);
    return pShere;
}

gpShapeBase* gpShapeBase::NewBox(const gpVec3& halfExtents)
{
    auto pBox = gpNew<gpInternal::gpPolygonShapeLayout>();
    gpConvertToBox(Deref(pBox), halfExtents);
    g_polygons.PushBack(pBox);
    return pBox;
}

gpShapeBase* gpShapeBase::Point()
{
    static gpShapeBase point(gpShapeType::Point);
    return AddressOf(point);
}

EZ_ON_GLOBAL_EVENT(GP_Core_GarbageCollectionEvent)
{
    for (ezUInt32 i = 0; i < g_spheres.GetCount(); ++i)
    {
        auto pSphere = g_spheres[i];
        if (!gpIsReferenced(pSphere))
        {
            gpDelete(pSphere);
        }
    }

    for (ezUInt32 i = 0; i < g_polygons.GetCount(); ++i)
    {
        auto pPolygon = g_polygons[i];
        if (!gpIsReferenced(pPolygon))
        {
            gpDelete(pPolygon);
        }
    }
}
