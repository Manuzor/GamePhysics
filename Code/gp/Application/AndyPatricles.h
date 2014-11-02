#pragma once
#include "gp/Application/ApplicationBase.h"

class gpWorld;

class gpAndyParticlesApp : public gpApplicationBase
{
public:
    gpAndyParticlesApp(const char* szTitle) : gpApplicationBase(szTitle), m_pWorld(nullptr) {}

    virtual void AfterEngineInit() override;
    virtual void BeforeEngineShutdown() override;
    virtual ApplicationExecution Run() override;

private:
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;
    ezTime m_LastUpdate;
    gpWorld* m_pWorld;

private:
    void PopulateWorld();
};
