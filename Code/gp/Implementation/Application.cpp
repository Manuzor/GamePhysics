#include "gp/PCH.h"
#include "gp/Application.h"

gpApplication::gpApplication() :
    m_pMainAllocator(nullptr)
{
}

gpApplication::~gpApplication()
{
    m_pMainAllocator = nullptr;
}

void gpApplication::AfterEngineInit()
{
    m_pMainAllocator = ezFoundation::GetDefaultAllocator();

    // TODO Implement me.
}

void gpApplication::BeforeEngineShutdown()
{
    m_pMainAllocator = nullptr;

    // TODO Implement me.
}
ezApplication::ApplicationExecution gpApplication::Run()
{
    printf("Hello World!");
    return Quit;
}
