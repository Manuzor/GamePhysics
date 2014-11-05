#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/World/World.h"
#include "gpCore/World/EntityBase.h"
#include "gpCore/World/Particle.h"
#include "gpCore/Rendering/Rendering.h"
#include "gpCore/Rendering/RenderExtractor.h"
#include "gpCore/Utilities/EzMathExtensions.h"

static void ExtractParticleData(gpRenderExtractor* pExtractor,
                                gpParticleEntity* pParticle,
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

void gpWorld::ExtractRenderingData(gpRenderExtractor* pExtractor) const
{
    ezStringBuilder sbStatName;
    sbStatName.Format("%s/", m_sName.GetData());

    ezStringBuilder sbToRecord;

    for (ezUInt32 i = 0; i < m_SimulatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_SimulatedEntities[i];
        const gpEntityDrawInfo* pEntityDrawInfo = m_pEntityDrawInfoDefault;
        auto FindResult = m_EntityDrawInfos.Find(pEntity);
        if(FindResult.IsValid())
            pEntityDrawInfo = &FindResult.Value();
        switch(pEntity->m_Type)
        {
        case gpEntityType::Particle:
            ExtractParticleData(pExtractor,
                                static_cast<gpParticleEntity*>(pEntity),
                                pEntityDrawInfo);
            break;
        case gpEntityType::RigidBody:
            GP_NotImplemented;
            break;
        default:
            break;
        }

        // Collect stats
        sbToRecord.Clear();
        gpGetStats(sbToRecord, pEntity);
        sbStatName.Append(pEntity->m_sName.GetData());
        ezStats::SetStat(sbStatName.GetData(), sbToRecord.GetData());
        sbStatName.Shrink(0, pEntity->m_sName.GetCharacterCount());
    }
}
