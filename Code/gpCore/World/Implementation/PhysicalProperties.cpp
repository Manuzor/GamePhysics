#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/World/PhysicalProperties.h"

void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props)
{
    ezStringBuilder sbStatName;
    ezStringBuilder sbStatValue;

    // Position
    sbStatName.Format("%s/Position", sStatName.GetData());
    const auto& position = gpValueOf(gpPositionOf(Props));
    sbStatValue.Format("{%f, %f, %f}", position.x,
                                       position.y,
                                       position.z);
    ezStats::SetStat(sbStatName, sbStatValue);

    // Linear Velocity
    sbStatName.Format("%s/LinearVelocity", sStatName.GetData());
    const auto& linearVelocity = gpValueOf(gpLinearVelocityOf(Props));
    sbStatValue.Format("{%f, %f, %f}", linearVelocity.x, linearVelocity.y, linearVelocity.z);
    ezStats::SetStat(sbStatName, sbStatValue);

    // Mass
    sbStatName.Format("%s/Mass", sStatName.GetData());
    sbStatValue.Format("%f", gpValueOf(gpMassOf(Props)));
    ezStats::SetStat(sbStatName, sbStatValue);

    // Gravity Factor
    sbStatName.Format("%s/GravityFactor", sStatName.GetData());
    sbStatValue.Format("%f", gpGravityFactorOf(Props));
    ezStats::SetStat(sbStatName, sbStatValue);
}
