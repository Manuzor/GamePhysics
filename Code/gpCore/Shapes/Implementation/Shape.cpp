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
    auto pShere = EZ_DEFAULT_NEW(gpInternal::gpCircleShapeLayout);
    gpRadiusOf(Deref(pShere)) = radius;
    g_spheres.PushBack(pShere);
    return pShere;
}

gpShapeBase* gpShapeBase::NewBox(const gpVec3& halfExtents)
{
    auto pBox = EZ_DEFAULT_NEW(gpInternal::gpPolygonShapeLayout);
    gpConvertToBox(Deref(pBox), halfExtents);
    g_polygons.PushBack(pBox);
    return pBox;
}

gpShapeBase* gpShapeBase::Point()
{
    static gpShapeBase point(gpShapeType::Point);
    return AddressOf(point);
}

EZ_ON_GLOBAL_EVENT(gpCore_GarbageCollectionEvent)
{
    ezUInt32 numCollectedSpheres = 0;
    for (ezUInt32 i = 0; i < g_spheres.GetCount(); ++i)
    {
        auto& pSphere = g_spheres[i];
        if (pSphere && !gpIsReferenced(pSphere))
        {
            EZ_DEFAULT_DELETE(pSphere);
            ++numCollectedSpheres;
        }
    }
    ezLog::Dev("Collected sphere shapes: %u", numCollectedSpheres);

    ezUInt32 numCollectedPolygons = 0;
    for (ezUInt32 i = 0; i < g_polygons.GetCount(); ++i)
    {
        auto& pPolygon = g_polygons[i];
        if (pPolygon && !gpIsReferenced(pPolygon))
        {
            EZ_DEFAULT_DELETE(pPolygon);
            ++numCollectedPolygons;
        }
    }
    ezLog::Dev("Collected polygon shapes: %u", numCollectedPolygons);
}
