#include "gpCore/PCH.h"
#include "gpCore/Shapes.h"

#include <Foundation/Communication/GlobalEvent.h>

namespace
{
    gpPointShape g_point;
    ezDynamicArray<gpPolygonShape*> g_polygons;
    ezDynamicArray<gpSphereShape*>  g_spheres;
}

gpPointShape* gpTypeAllocator<gpPointShape>::New() { return AddressOf(g_point); }

gpSphereShape* gpTypeAllocator<gpSphereShape>::New(gpScalar radius)
{
    auto pShere = EZ_DEFAULT_NEW(gpSphereShape);
    gpRadiusOf(Deref(pShere)) = radius;
    g_spheres.PushBack(pShere);
    return pShere;
}

// [missing] polygon New() function

gpBoxShape* gpTypeAllocator<gpBoxShape>::New(const gpVec3& halfExtents)
{
    auto pBox = EZ_DEFAULT_NEW(gpBoxShape);
    gpConvertToBox(Deref(pBox), halfExtents);
    g_polygons.PushBack(pBox);
    return pBox;
}

void gpReleaseReferenceTo(gpShape& shape)
{
    ezLog::Success("Releasing reference to shape type!");
    shape.ReleaseRef();
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
