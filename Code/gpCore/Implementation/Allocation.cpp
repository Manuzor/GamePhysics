#include "gpCore/PCH.h"
#include "gpCore/Allocation.h"

#include <Foundation/Communication/GlobalEvent.h>

void gpTriggerGarbageCollection()
{
    ezGlobalEvent::Broadcast(GP_Core_GarbageCollectionEvent);
}
