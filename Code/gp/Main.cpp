#include "gp/PCH.h"
#include <Core/Application/Application.h>

class gpApplication : public ezApplication
{
public:

    virtual void AfterEngineInit() override
    {
        // TODO Implement me.
    }

    virtual void BeforeEngineShutdown() override
    {
        // TODO Implement me.
    }

    virtual ApplicationExecution Run() override
    {
        return Continue;
    }

private:

};

EZ_CONSOLEAPP_ENTRY_POINT(gpApplication);
