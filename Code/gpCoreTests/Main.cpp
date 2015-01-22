#include "gpCoreTests/PCH.h"

#include <Core/Application/Application.h>
#include <TestFramework/Framework/TestFramework.h>
#include <TestFramework/Utilities/TestSetup.h>
#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>

class gpCoreTestsApp : public ezApplication
{
public:

    virtual void AfterEngineInit() override
    {
        ezGlobalLog::AddLogWriter(ezLogWriter::Console::LogMessageHandler);
        ezGlobalLog::AddLogWriter(ezLogWriter::VisualStudio::LogMessageHandler);

        ezLog::Info("Initializing test framework...");
        ezTestSetup::InitTestFramework("CoreTests",
                                       "Game Physics Core Tests",
                                       GetArgumentCount(),
                                       GetArgumentsArray());
        ezLog::Success("Test framework initialized!");
    }

    virtual void BeforeEngineShutdown() override
    {
        ezTestSetup::DeInitTestFramework();
    }

    virtual ApplicationExecution Run() override
    {
        ezLog::Info("Running... (please be patient)");
        auto iFailedTests = ezTestSetup::RunTests(GetArgumentCount(),
                                                  const_cast<char**>(GetArgumentsArray()));
        SetReturnCode(iFailedTests);
        return Quit;
    }
};

EZ_CONSOLEAPP_ENTRY_POINT(gpCoreTestsApp);
