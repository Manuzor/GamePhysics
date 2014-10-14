#include "gp/PCH.h"
#include "gp/Application.h"

#include <TestFramework/Framework/TestFramework.h>
#include <TestFramework/Utilities/TestSetup.h>

ezInt32 gpApplication::RunTests()
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
