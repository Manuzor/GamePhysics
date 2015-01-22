#pragma once
#include "gpCore/Application/ApplicationBase.h"

class gpWorld;
class gpRenderExtractor;
class gpEntity;

class gpParticlesApp : public gpApplicationBase
{
public:
    gpParticlesApp(const char* szTitle) : gpApplicationBase(szTitle), m_pWorld(nullptr), m_pCurrentParticle(nullptr) {}

    virtual void AfterEngineInit() override;
    virtual void BeforeEngineShutdown() override;
    virtual ApplicationExecution Run() override;

    void Update(ezTime dt);

private:
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;
    ezTime m_LastUpdate;
    gpWorld* m_pWorld;

    gpEntity* m_pCurrentParticle;

private:
    void PopulateWorld();
    void AddNewParticle(const gpDisplacement& Position);
    void ExtractVelocityData(gpRenderExtractor* pExtractor);
};
