#include "gpCore/PCH.h"
#include "gpCore/Allocation.h"

#include <Foundation/Communication/GlobalEvent.h>

void gpTriggerGarbageCollection()
{
    EZ_LOG_BLOCK("Garbage Collection");
    ezLog::Dev("Triggering");
    ezGlobalEvent::Broadcast("gpCore_GarbageCollectionEvent");
}
