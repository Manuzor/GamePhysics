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

static void ExtractParticleData(gpRenderExtractor* pExtractor,
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

static void ExtractForceFieldData(gpRenderExtractor* pExtractor,
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

static void ExtractShapeData(gpRenderExtractor* pExtractor,
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

static void ExtractRigidBodyData(gpRenderExtractor* pExtractor,
                                 const gpRigidBody* pRigidBody,
                                 const gpEntityDrawInfo* pDrawInfo)
{
    ExtractShapeData(pExtractor, pRigidBody->GetProperties(), pRigidBody->GetShape(), pDrawInfo);
}

static void ExtractEntity(gpRenderExtractor* pExtractor, gpEntityBase* pEntity, const gpEntityDrawInfo* pDrawInfo)
{
    switch(pEntity->GetType())
    {
    case gpEntityType::Particle:
        ExtractParticleData(pExtractor,
                            static_cast<gpParticleEntity*>(pEntity),
                            pDrawInfo);
        break;
    case gpEntityType::ForceField:
        ExtractForceFieldData(pExtractor,
                                static_cast<gpForceFieldEntity*>(pEntity),
                                pDrawInfo);
        break;
    case gpEntityType::RigidBody:
        ExtractRigidBodyData(pExtractor,
                             static_cast<gpRigidBody*>(pEntity),
                             pDrawInfo);
        break;
    default:
        GP_NotImplemented;
        break;
    }
}

void gpWorld::ExtractRenderingData(gpRenderExtractor* pExtractor) const
{
    ezStringBuilder sbStatName;
    sbStatName.Format("%s/", m_sName.GetData());

    ezStringBuilder sbToRecord;

    for (ezUInt32 i = 0; i < m_CreatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_CreatedEntities[i];
        if(pEntity == nullptr || pEntity->GetWorld() != this)
            continue;
        const gpEntityDrawInfo* pEntityDrawInfo = m_pEntityDrawInfoDefault;
        auto FindResult = m_EntityDrawInfos.Find(pEntity);
        if(FindResult.IsValid())
            pEntityDrawInfo = &FindResult.Value();

        ExtractEntity(pExtractor, pEntity, pEntityDrawInfo);

        // Collect stats
        sbToRecord.Clear();
        gpGetStats(sbToRecord, pEntity);
        sbStatName.Append(pEntity->m_sName.GetData());
        ezStats::SetStat(sbStatName.GetData(), sbToRecord.GetData());
        sbStatName.Shrink(0, pEntity->m_sName.GetCharacterCount());
    }
}
