#include "gpCore/PCH.h"
#include "gpCore/Shapes.h"

#include <Foundation/Communication/GlobalEvent.h>

namespace
{
    gpPointShape* GlobalPointInstance()
    {
        static gpPointShape* pPoint = nullptr;

        if (pPoint == nullptr)
        {
            static gpPointShape point;
            gpAddReferenceTo(point);
            pPoint = AddressOf(point);
        }

        return pPoint;
    }

    ezDynamicArray<gpShape*> g_allShapes;
}

gpPointShape* gpTypeAllocator<gpPointShape>::New() { return GlobalPointInstance(); }

gpSphereShape* gpTypeAllocator<gpSphereShape>::New(gpScalar radius)
{
    auto pShere = EZ_DEFAULT_NEW(gpSphereShape);
    gpRadiusOf(Deref(pShere)) = radius;
    g_allShapes.PushBack(pShere);
    return pShere;
}

gpPolygonShape* gpTypeAllocator<gpPolygonShape>::New(ezArrayPtr<gpVec3> vertices)
{
    auto pPolygon = EZ_DEFAULT_NEW(gpPolygonShape);
    gpVerticesOf(Deref(pPolygon)) = vertices;
    g_allShapes.PushBack(pPolygon);
    return pPolygon;
}

gpBoxShape* gpTypeAllocator<gpBoxShape>::New(const gpVec3& halfExtents)
{
    auto pBox = EZ_DEFAULT_NEW(gpBoxShape);
    gpConvertToBox(Deref(pBox), halfExtents);
    g_allShapes.PushBack(pBox);
    return pBox;
}

void gpHandleUnreferencedObject(gpShape*& pShape)
{
    EZ_ASSERT(pShape != GlobalPointInstance(), "The point is not supposed to be unreferenced at all!");
    EZ_ASSERT(g_allShapes.Contains(pShape), "Invalid or double freeing of shape ptr!");

    g_allShapes.RemoveSwap(pShape);
    EZ_DEFAULT_DELETE(pShape);
}
