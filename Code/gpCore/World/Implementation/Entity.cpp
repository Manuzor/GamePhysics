#include "gpCore/PCH.h"
#include "gpCore/World/Entity.h"

#include <Foundation/Communication/GlobalEvent.h>

void gpGetStats(ezStringBuilder& out_Stats, const gpEntity& entity)
{
    const auto& vPos = gpValueOf(gpPositionOf(entity));
    const auto& vVel = gpValueOf(gpLinearVelocityOf(entity));
    out_Stats.Format("position = {%3.3f, %3.3f, %3.3f} linearVelocity = {%3.3f, %3.3f, %3.3f}",
                     vPos.x, vPos.y, vPos.z,
                     vVel.x, vVel.y, vVel.z);
}

ezDynamicArray<gpEntity*> g_entities;

gpEntity* gpInternal::gpTypeAllocator<gpEntity>::New()
{
    auto pEntity = EZ_DEFAULT_NEW(gpEntity);
    g_entities.PushBack(pEntity);
    return pEntity;
}

EZ_ON_GLOBAL_EVENT(GP_Core_GarbageCollectionEvent)
{
    for (ezUInt32 i = 0; i < g_entities.GetCount(); ++i)
    {
        auto& pEntity = g_entities[i];
        if (pEntity && !pEntity->IsReferenced())
        {
            EZ_DEFAULT_DELETE(pEntity); // This call also nulls pEntity
        }
    }

    /// \todo Clean up g_entities using the following algorithm:
    ///       1) Sort it, so all nullptr entries are at the back. Make sure to count the nullptr instances.
    ///       2) Set count of g_entities to originalSize - numNullPtrs
}
