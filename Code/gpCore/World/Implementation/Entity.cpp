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

gpEntity* gpTypeAllocator<gpEntity>::New()
{
    auto pEntity = EZ_DEFAULT_NEW(gpEntity);
    g_entities.PushBack(pEntity);
    return pEntity;
}

void gpHandleUnreferencedObject(gpEntity*& pEntity)
{
    EZ_ASSERT(g_entities.Contains(pEntity), "Invalid or double free.");
    g_entities.RemoveSwap(pEntity);
    EZ_DEFAULT_DELETE(pEntity);
}
