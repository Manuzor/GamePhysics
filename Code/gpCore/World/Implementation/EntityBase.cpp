#include "gpCore/PCH.h"
#include "gpCore/World/EntityBase.h"

void gpGetStats(ezStringBuilder& out_Stats, const gpEntityBase& entity)
{
    const auto& vPos = gpPositionOf(entity);
    const auto& vVel = gpLinearVelocityOf(entity);
    out_Stats.Format("position = {%3.3f, %3.3f, %3.3f} linearVelocity = {%3.3f, %3.3f, %3.3f}",
                     vPos.x, vPos.y, vPos.z,
                     vVel.x, vVel.y, vVel.z);

}
