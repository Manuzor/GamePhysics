#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/World/PhysicalProperties.h"

void gpUpdateStats(const ezStringView sStatName, const gpPhysicalProperties& Props)
{
    ezStringBuilder sbStatName;
    ezStringBuilder sbStatValue;

    // Position
    sbStatName.Format("%s/Position", sStatName.GetData());
    sbStatValue.Format("{%f, %f, %f}", Props.m_Position.x, Props.m_Position.y, Props.m_Position.z);
    ezStats::SetStat(sbStatName, sbStatValue);

    // Linear Velocity
    sbStatName.Format("%s/LinearVelocity", sStatName.GetData());
    sbStatValue.Format("{%f, %f, %f}", Props.m_LinearVelocity.x, Props.m_LinearVelocity.y, Props.m_LinearVelocity.z);
    ezStats::SetStat(sbStatName, sbStatValue);

    // Mass
    sbStatName.Format("%s/Mass", sStatName.GetData());
    sbStatValue.Format("%f", Props.m_fGravityFactor);
    ezStats::SetStat(sbStatName, sbStatValue);

    // Gravity Factor
    sbStatName.Format("%s/GravityFactor", sStatName.GetData());
    sbStatValue.Format("%f", Props.m_fGravityFactor);
    ezStats::SetStat(sbStatName, sbStatValue);
}
