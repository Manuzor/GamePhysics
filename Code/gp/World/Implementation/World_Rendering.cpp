#include "gp/PCH.h"
#include "gp/World/World.h"
#include "gp/World/EntityBase.h"
#include "gp/World/Particle.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/RenderExtractor.h"

static void ExtractParticleData(gpRenderExtractor* pExtractor,
                                gpParticleEntity* pParticle,
                                const gpEntityDrawInfo* pDrawInfo)
{
    auto pData = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pData->m_Position = pParticle->GetPosition();
    pData->m_fPointSize = 3;
    if(pDrawInfo)
    {
        pData->m_Color = pDrawInfo->m_Color;
    }
}

void gpWorld::ExtractRenderingData(gpRenderExtractor* pExtractor) const
{
    for (ezUInt32 i = 0; i < m_SimulatedEntities.GetCount(); ++i)
    {
        auto pEntity = m_SimulatedEntities[i];
        const gpEntityDrawInfo* pEntityDrawInfo = nullptr;
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
        auto pos = pEntity->GetPosition();
        auto vel = pEntity->GetLinearVelocity();
        ezLog::Info("%s: p{%3.3f, %3.3f, %3.3f} v{%3.3f, %3.3f, %3.3f}",
                    pEntity->GetName().GetData(),
                    pos.x, pos.y, pos.z,
                    vel.x, vel.y, vel.z);
    }
}
