#include "gpCoreTests/PCH.h"

#include <Core/Application/Application.h>
#include <TestFramework/Framework/TestFramework.h>
#include <TestFramework/Utilities/TestSetup.h>

class gpCoreTestsApp : public ezApplication
{
public:

    virtual void AfterEngineInit() override
    {

        ezTestSetup::InitTestFramework("CoreTests",
                                       "Game Physics Core Tests",
                                       GetArgumentCount(),
                                       GetArgumentsArray());
    }

    virtual void BeforeEngineShutdown() override
    {
        ezTestSetup::DeInitTestFramework();
    }

    virtual ApplicationExecution Run() override
    {

        auto iFailedTests = ezTestSetup::RunTests(GetArgumentCount(),
                                                  const_cast<char**>(GetArgumentsArray()));
        SetReturnCode(iFailedTests);
        return Quit;
    }
};

EZ_CONSOLEAPP_ENTRY_POINT(gpCoreTestsApp);
