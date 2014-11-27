#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>

#include "gpCore/World/ForceField.h"

void gpUpdateStats(const ezStringView sStatName, const gpForceFieldEntity& ForceField)
{
    {
        ezStringBuilder sbStatName;
        ezStringBuilder sbStatValue;

        // Radius
        sbStatName.Format("%s/Radius", sStatName.GetData());
        sbStatValue.Format("%f", gpRadiusOf(ForceField));
        ezStats::SetStat(sbStatName, sbStatValue);

        // Force
        sbStatName.Format("%s/Force", sStatName.GetData());
        sbStatValue.Format("%f", gpForceOf(ForceField));
        ezStats::SetStat(sbStatName, sbStatValue);
    }
    gpUpdateStats(sStatName, gpPhysicalPropertiesOf(ForceField));
}
