#include "gpCore/PCH.h"

#include <Foundation/Utilities/Stats.h>
#include <Foundation/Communication/GlobalEvent.h>

#include "gpCore/World/ForceField.h"
#include "gpCore/Dynamics/CollisionDetection.h"

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

ezDynamicArray<gpForceFieldEntity*> g_forceFields;

gpForceFieldEntity* gpInternal::gpTypeAllocator<gpForceFieldEntity>::New()
{
    auto pForceField = EZ_DEFAULT_NEW(gpForceFieldEntity);
    g_forceFields.PushBack(pForceField);
    return pForceField;
}

EZ_ON_GLOBAL_EVENT(GP_Core_GarbageCollectionEvent)
{
    for (ezUInt32 i = 0; i < g_forceFields.GetCount(); ++i)
    {
        auto& pForceField = g_forceFields[i];
        if (pForceField && !pForceField->IsReferenced())
        {
            EZ_DEFAULT_DELETE(pForceField); // This call also nulls pEntity
        }
    }

    /// \todo Clean up g_forceFields using the following algorithm:
    ///       1) Sort it, so all nullptr entries are at the back. Make sure to count the nullptr instances.
    ///       2) Set count of g_forceFields to originalSize - numNullPtrs
}
