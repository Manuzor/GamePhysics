#include <Core/Application/Application.h>

class gpApplication : public ezApplication
{
public:

    virtual void AfterEngineInit() override;

    virtual void BeforeEngineShutdown() override;

    virtual ApplicationExecution Run() override;

private:

};
