#include "gp/PCH.h"
#include "gp/Application/ApplicationBase.h"

#include <TestFramework/Framework/TestFramework.h>
#include <TestFramework/Utilities/TestSetup.h>

ezInt32 gpApplicationBase::RunTests()
{
    ezTestSetup::InitTestFramework("GamePhysicsTests",
                                   "Game Physics Engine Tests",
                                   GetArgumentCount(),
                                   GetArgumentsArray());

    auto iFailedTests = ezTestSetup::RunTests(GetArgumentCount(),
                                              const_cast<char**>(GetArgumentsArray()));

    ezTestSetup::DeInitTestFramework();
    return iFailedTests;
}
