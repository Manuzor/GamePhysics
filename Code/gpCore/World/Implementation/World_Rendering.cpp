#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/Utilities/EzMathExtensions.h"

#include "gpCore/World/World.h"
#include "gpCore/World/Entity.h"
#include "gpCore/World/ForceField.h"

#include "gpCore/Shapes/ShapeBase.h"
#include "gpCore/Shapes/Circle.h"
#include "gpCore/Shapes/Polygon.h"

// Extract a particle
static void Extract(gpRenderExtractor* pExtractor,
                    const gpPhysicalProperties& props,
                    const gpEntityDrawInfo& drawInfo)
{
    if (ezMath::IsZero(drawInfo.m_Color.a))
        return;

    auto pData = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pData->m_Color = drawInfo.m_Color;

    pData->m_Position = gpValueOf(gpPositionOf(props));
    pData->m_fPointSize = drawInfo.m_fScale;

    // If alpha > 0 and speed > 0
    if (!ezMath::IsZero(drawInfo.m_LinearVelocityColor.a)
        && !gpValueOf(gpLinearVelocityOf(props)).IsZero())
    {
        auto pVel = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
        pVel->m_Start = gpValueOf(gpPositionOf(props));
        pVel->m_End = pVel->m_Start + gpValueOf(gpLinearVelocityOf(props));
        pVel->m_Color = drawInfo.m_LinearVelocityColor;
        pVel->m_WingAngle = drawInfo.m_LinearVelocityArrowWingAngle;
        pVel->m_fWingLength = drawInfo.m_fLinearVelocityArrowWingLength;
    }
}

// Extract a shape
static void Extract(gpRenderExtractor* pExtractor,
                    const gpPhysicalProperties& props,
                    const gpShapeBase& shape,
                    const gpEntityDrawInfo& drawInfo)
{
    switch(gpTypeOf(shape))
    {
    case gpShapeType::Circle:
    {
        auto pCircle = pExtractor->AllocateRenderData<gpDrawData::Circle>();
        pCircle->m_Position = gpValueOf(gpPositionOf(props));
        pCircle->m_fRadius = gpRadiusOf(static_cast<const gpCircleShape&>(shape));

        pCircle->m_uiNumLineSegments = 20;
        pCircle->m_OutlineColor = drawInfo.m_Color;
        pCircle->m_FillColor = drawInfo.m_Color;
        pCircle->m_FillColor.a *= 0.5f;
    }
        break;
    case gpShapeType::Polygon:
    {
        auto& polygon = static_cast<const gpPolygonShape&>(shape);
        auto pPolyData = pExtractor->AllocateRenderData<gpDrawData::Polygon>();

        pPolyData->m_Transform = gpTransformOf(props);
        pPolyData->m_Vertices = gpVerticesOf(polygon);
        pPolyData->m_OutlineColor = drawInfo.m_Color;
        pPolyData->m_FillColor = drawInfo.m_Color;
        pPolyData->m_FillColor.a *= 0.5f;

        break;
    }
    default:
        GP_NotImplemented;
        break;
    }
}

// Extract a force field
static void Extract(gpRenderExtractor* pExtractor,
                    const gpForceFieldEntity& forceField,
                    const gpEntityDrawInfo& drawInfo)
{
    if (ezMath::IsZero(drawInfo.m_Color.a))
        return;

    auto pData = pExtractor->AllocateRenderData<gpDrawData::Circle>();
    pData->m_OutlineColor.a = 0.0f; // no outline
    pData->m_FillColor = drawInfo.m_Color;
    pData->m_FillColor.a *= 0.1f;
    pData->m_uiNumLineSegments = 20;

    pData->m_Position = gpValueOf(gpPositionOf(forceField));
    pData->m_fRadius = gpRadiusOf(forceField);
}

void gpExtractRenderDataOf(const gpWorld& world, gpRenderExtractor* pExtractor)
{
    EZ_PROFILE(world.m_ProfilingId_Extraction);

    ezStringBuilder sbStatName;

    for (ezUInt32 i = 0; i < world.m_ForceFields.GetCount(); ++i)
    {
        auto pForceField = world.m_ForceFields[i];
        if (pForceField == nullptr)
            continue;

        auto& forceField = Deref(pForceField);
        const auto* pDrawInfo = gpDefaultDrawInfoPtrOf(world);
        auto findResult = world.m_EntityDrawInfos.Find(pForceField);
        if (findResult.IsValid())
            pDrawInfo = &findResult.Value();

        EZ_ASSERT(pDrawInfo, "");
        Extract(pExtractor, forceField, Deref(pDrawInfo));
    }

    for (ezUInt32 i = 0; i < world.m_SimulatedEntities.GetCount(); ++i)
    {
        auto pEntity = world.m_SimulatedEntities[i];
        if (pEntity == nullptr || gpWorldPtrOf(Deref(pEntity)) != AddressOf(world))
            continue;

        auto& entity = Deref(pEntity);
        const auto* pEntityDrawInfo = gpDefaultDrawInfoPtrOf(world);
        auto FindResult = world.m_EntityDrawInfos.Find(AddressOf(entity));
        if(FindResult.IsValid())
            pEntityDrawInfo = &FindResult.Value();

        EZ_ASSERT(pEntityDrawInfo, "");

        auto pShape = gpShapePtrOf(entity);

        if (pShape)
        {
            // This is a rigid body.
            Extract(pExtractor, gpPhysicalPropertiesOf(entity), Deref(pShape), Deref(pEntityDrawInfo));
        }
        else
        {
            // This is a particle.
            Extract(pExtractor, gpPhysicalPropertiesOf(entity), Deref(pEntityDrawInfo));
        }
    }
}
