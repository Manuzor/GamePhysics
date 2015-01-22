#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>
#include <Foundation/Communication/GlobalEvent.h>

#include "gpCore/World/ForceField.h"
#include "gpCore/Algorithm/CollisionDetection.h"

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
        sbStatValue.Format("%f", gpForceFactorOf(ForceField));
        ezStats::SetStat(sbStatName, sbStatValue);
    }
    gpUpdateStats(sStatName, gpPhysicalPropertiesOf(ForceField));
}

bool gpAffects(const gpForceFieldEntity& forceField, const gpEntity& entity)
{
    return gpAreColliding(gpTransformOf(forceField), gpShapeOf(forceField),
                          gpTransformOf(entity), gpShapeOf(entity));
}

namespace
{
    ezDynamicArray<gpForceFieldEntity*> g_forceFields;
}

gpForceFieldEntity* gpTypeAllocator<gpForceFieldEntity>::New()
{
    auto pForceField = EZ_DEFAULT_NEW(gpForceFieldEntity);
    g_forceFields.PushBack(pForceField);
    return pForceField;
}

void gpHandleUnreferencedObject(gpForceFieldEntity*& pForceField)
{
    EZ_ASSERT(g_forceFields.Contains(pForceField), "Invalid or double free.");
    g_forceFields.RemoveSwap(pForceField);
    EZ_DEFAULT_DELETE(pForceField);
}
