#pragma once
#include "gpCore/Application/ApplicationBase.h"

class gpWorld;
class gpRenderExtractor;
class gpEntity;

class gpForceFieldsApp : public gpApplicationBase
{
public:
    gpForceFieldsApp(const char* szTitle) : gpApplicationBase(szTitle), m_pWorld(nullptr), m_pPlayer(nullptr) {}

    virtual void AfterEngineInit() override;
    virtual void BeforeEngineShutdown() override;
    virtual ApplicationExecution Run() override;

    void Update(ezTime dt);

private:
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;
    ezTime m_LastUpdate;
    gpWorld* m_pWorld;

    gpEntity* m_pPlayer;

    static ezCVarFloat s_fPlayerMaxSpeed;

private:
    void CreateForceFields();
    void CreatePlayer();
    bool CanSpawnPlayer();
    void SpawnAndFreezePlayer();
    void UnfreezePlayer();
    void DespawnPlayer();

    void ResetWorld();

    void EnableVelocityDataExtraction(bool bEnabled);
    void ExtractVelocityData(gpRenderExtractor* pExtractor);
};
