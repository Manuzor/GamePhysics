#include "gp/PCH.h"
#include "gp/Application.h"

void gpApplication::AfterEngineInit()
{
    // TODO Implement me.
}

void gpApplication::BeforeEngineShutdown()
{
    // TODO Implement me.
}
ezApplication::ApplicationExecution gpApplication::Run()
{
    printf("Hello World!");
    return Quit;
}
