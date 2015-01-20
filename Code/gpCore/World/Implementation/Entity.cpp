#include "gpCore/PCH.h"
#include "gpCore/World/Entity.h"

void gpGetStats(ezStringBuilder& out_Stats, const gpEntityBase& entity)
{
    const auto& vPos = gpValueOf(gpPositionOf(entity));
    const auto& vVel = gpValueOf(gpLinearVelocityOf(entity));
    out_Stats.Format("position = {%3.3f, %3.3f, %3.3f} linearVelocity = {%3.3f, %3.3f, %3.3f}",
                     vPos.x, vPos.y, vPos.z,
                     vVel.x, vVel.y, vVel.z);
}
