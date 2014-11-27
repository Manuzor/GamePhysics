#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/Utilities/EzMathExtensions.h"

#include "gpCore/World/World.h"
#include "gpCore/World/EntityBase.h"
#include "gpCore/World/Particle.h"
#include "gpCore/World/ForceField.h"
#include "gpCore/World/RigidBody.h"

#include "gpCore/Shapes/ShapeBase.h"
#include "gpCore/Shapes/Circle.h"

static void Extract(gpRenderExtractor* pExtractor,
                    const gpParticleEntity& particle,
                    const gpEntityDrawInfo& drawInfo)
{
    if (ezMath::IsZero(drawInfo.m_Color.a))
        return;

    auto pData = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pData->m_Color = drawInfo.m_Color;

    pData->m_Position = gpPositionOf(particle);
    pData->m_fPointSize = drawInfo.m_fScale;

    // If alpha > 0 and speed > 0
    if (!ezMath::IsZero(drawInfo.m_LinearVelocityColor.a)
        && !gpLinearVelocityOf(particle).IsZero())
    {
        auto pVel = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
        pVel->m_Start = gpPositionOf(particle);
        pVel->m_End = gpPositionOf(particle) + gpLinearVelocityOf(particle);
        pVel->m_Color = drawInfo.m_LinearVelocityColor;
        pVel->m_WingAngle = drawInfo.m_LinearVelocityArrowWingAngle;
        pVel->m_fWingLength = drawInfo.m_fLinearVelocityArrowWingLength;
    }
}

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

    pData->m_Position = gpPositionOf(forceField);
    pData->m_fRadius = gpRadiusOf(forceField);
}

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
        pCircle->m_Position = gpPositionOf(props);
        pCircle->m_fRadius = gpRadiusOf(static_cast<const gpCircleShape&>(shape));

        pCircle->m_uiNumLineSegments = 20;
        pCircle->m_OutlineColor = drawInfo.m_Color;
        pCircle->m_FillColor = drawInfo.m_Color;
        pCircle->m_FillColor.a *= 0.5f;
    }
        break;
    default:
        GP_NotImplemented;
        break;
    }
}

static void Extract(gpRenderExtractor* pExtractor,
                    const gpRigidBody& rigidBody,
                    const gpEntityDrawInfo& drawInfo)
{
    Extract(pExtractor, gpPhysicalPropertiesOf(rigidBody), Deref(gpShapePtrOf(rigidBody)), drawInfo);
}

void gpWorld::ExtractRenderingData(gpRenderExtractor* pExtractor) const
{
    EZ_PROFILE(m_ProfilingId_Extraction);

    ezStringBuilder sbStatName;

    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        if (m_CreatedEntities[i] == nullptr || gpWorldPtrOf(Deref(m_CreatedEntities[i])) != this)
            continue;

        auto& entity = Deref(m_CreatedEntities[i]);
        const auto* pEntityDrawInfo = m_pEntityDrawInfoDefault;
        auto FindResult = m_EntityDrawInfos.Find(AddressOf(entity));
        if(FindResult.IsValid())
            pEntityDrawInfo = &FindResult.Value();

        auto EntityType = gpTypeOf(entity);

        sbStatName.Format("%s/%s", m_sName.GetData(), gpNameOf(entity).GetData());
        ezStats::SetStat(sbStatName, EntityType.ToString(EntityType));

        EZ_ASSERT(pEntityDrawInfo, "");

        switch(EntityType)
        {
        case gpEntityType::Particle:
        {
            const auto& particle = static_cast<const gpParticleEntity&>(entity);
            Extract(pExtractor, particle, Deref(pEntityDrawInfo));
            gpUpdateStats(sbStatName, particle);
            break;
        }
        case gpEntityType::ForceField:
        {
            const auto& forceField = static_cast<const gpForceFieldEntity&>(entity);
            Extract(pExtractor, forceField, Deref(pEntityDrawInfo));
            gpUpdateStats(sbStatName, forceField);
            break;
        }
        case gpEntityType::RigidBody:
        {
            const auto& rigidBody = static_cast<const gpRigidBody&>(entity);
            Extract(pExtractor, rigidBody, Deref(pEntityDrawInfo));
            gpUpdateStats(sbStatName, rigidBody);
            break;
        }
        default:
            GP_NotImplemented;
            break;
        }
    }
}
