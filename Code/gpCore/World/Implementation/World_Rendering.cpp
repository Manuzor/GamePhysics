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
                    const gpParticleEntity* pParticle,
                    const gpEntityDrawInfo* pDrawInfo)
{
    EZ_ASSERT(pDrawInfo, "Need a valid draw info!");

    if (ezMath::IsZero(pDrawInfo->m_Color.a))
        return;

    auto pData = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pData->m_Color = pDrawInfo->m_Color;

    auto pProps = pParticle->GetProperties();
    pData->m_Position = pProps->m_Position;
    pData->m_fPointSize = pDrawInfo->m_fScale;

    // If alpha > 0 and speed > 0
    if (!ezMath::IsZero(pDrawInfo->m_LinearVelocityColor.a)
        && !pProps->m_LinearVelocity.IsZero())
    {
        auto pVel = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
        pVel->m_Start = pProps->m_Position;
        pVel->m_End = pProps->m_Position + pProps->m_LinearVelocity;
        pVel->m_Color = pDrawInfo->m_LinearVelocityColor;
        pVel->m_WingAngle = pDrawInfo->m_LinearVelocityArrowWingAngle;
        pVel->m_fWingLength = pDrawInfo->m_fLinearVelocityArrowWingLength;
    }
}

static void Extract(gpRenderExtractor* pExtractor,
                    const gpForceFieldEntity* pForceField,
                    const gpEntityDrawInfo* pDrawInfo)
{
    EZ_ASSERT(pDrawInfo, "Need a valid draw info!");

    if (ezMath::IsZero(pDrawInfo->m_Color.a))
        return;

    auto pData = pExtractor->AllocateRenderData<gpDrawData::Circle>();
    pData->m_OutlineColor.a = 0.0f; // no outline
    pData->m_FillColor = pDrawInfo->m_Color;
    pData->m_FillColor.a *= 0.1f;
    pData->m_uiNumLineSegments = 20;

    auto pProps = pForceField->GetProperties();
    pData->m_Position = pProps->m_Position;
    pData->m_fRadius = pForceField->GetRadius();
}

static void Extract(gpRenderExtractor* pExtractor,
                    const gpPhysicalProperties* pProps,
                    const gpShapeBase* pShape,
                    const gpEntityDrawInfo* pDrawInfo)
{
    switch(pShape->GetType())
    {
    case gpShapeType::Circle:
    {
        auto pCircle = pExtractor->AllocateRenderData<gpDrawData::Circle>();
        pCircle->m_Position = pProps->m_Position;
        pCircle->m_fRadius = static_cast<const gpCircleShape*>(pShape)->GetRadius();

        pCircle->m_uiNumLineSegments = 20;
        pCircle->m_OutlineColor = pDrawInfo->m_Color;
        pCircle->m_FillColor = pDrawInfo->m_Color;
        pCircle->m_FillColor.a *= 0.5f;
    }
        break;
    default:
        GP_NotImplemented;
        break;
    }
}

static void Extract(gpRenderExtractor* pExtractor,
                    const gpRigidBody* pRigidBody,
                    const gpEntityDrawInfo* pDrawInfo)
{
    Extract(pExtractor, pRigidBody->GetProperties(), pRigidBody->GetShape(), pDrawInfo);
}

void gpWorld::ExtractRenderingData(gpRenderExtractor* pExtractor) const
{
    EZ_PROFILE(m_ProfilingId_Extraction);

    ezStringBuilder sbStatName;

    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_CreatedEntities[i];
        if(pEntity == nullptr || pEntity->GetWorld() != this)
            continue;
        const gpEntityDrawInfo* pEntityDrawInfo = m_pEntityDrawInfoDefault;
        auto FindResult = m_EntityDrawInfos.Find(pEntity);
        if(FindResult.IsValid())
            pEntityDrawInfo = &FindResult.Value();

        auto EntityType = pEntity->GetType();

        sbStatName.Format("%s/%s", m_sName.GetData(), pEntity->GetName().GetData());
        ezStats::SetStat(sbStatName, EntityType.ToString(EntityType));

        switch(EntityType)
        {
        case gpEntityType::Particle:
        {
            auto pParticle = static_cast<gpParticleEntity*>(pEntity);
            Extract(pExtractor, pParticle, pEntityDrawInfo);
            gpUpdateStats(sbStatName, *pParticle);
            break;
        }
        case gpEntityType::ForceField:
        {
            auto pForceField = static_cast<gpForceFieldEntity*>(pEntity);
            Extract(pExtractor, pForceField, pEntityDrawInfo);
            gpUpdateStats(sbStatName, *pForceField);
            break;
        }
        case gpEntityType::RigidBody:
        {
            auto pRigidBody = static_cast<gpRigidBody*>(pEntity);
            Extract(pExtractor, pRigidBody, pEntityDrawInfo);
            gpUpdateStats(sbStatName, *pRigidBody);
            break;
        }
        default:
            GP_NotImplemented;
            break;
        }
    }
}
