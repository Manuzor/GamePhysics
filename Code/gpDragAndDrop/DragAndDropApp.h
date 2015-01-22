#pragma once
#include "gpCore/Application/ApplicationBase.h"

class gpDragAndDropApp : public gpApplicationBase
{
public:
    gpDragAndDropApp(const char* szTitle) : gpApplicationBase(szTitle) {}

    virtual void AfterEngineInit() override;
    virtual void BeforeEngineShutdown() override;
    virtual ApplicationExecution Run() override;

private:
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;
    ezTime m_LastUpdate;
};
