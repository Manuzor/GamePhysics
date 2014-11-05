#include "gpExperiments/PCH.h"

#include <Core/Input/InputManager.h>

#include "gpExperiments/Application.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/RenderExtractor.h"
#include "gp/World/World.h"
#include "gp/World/Particle.h"

static gpWorld g_World("World");

static void PopulateWorld()
{
    auto pParticle = g_World.CreateEntity<gpParticleEntity>();
    pParticle->SetName("TheParticle");
    pParticle->GetProperties()->m_Position.Set(100, 200, 0);
    EZ_VERIFY(g_World.AddEntity(pParticle).Succeeded(), "");
}

static void Update(ezTime dt)
{

}

static void OnRenderExtraction(gpRenderExtractor* pExtractor)
{
    auto dt = ezClock::Get()->GetAccumulatedTime().GetSeconds();
    auto angle = ezAngle::Radian(static_cast<float>(-dt));
    angle.NormalizeRange();

    auto pLine = pExtractor->AllocateRenderData<gpDrawData::Line>();
    pLine->m_Start.Set(250, 250, 0);
    pLine->m_End = pLine->m_Start + gpVec3(ezMath::Cos(angle), ezMath::Sin(angle), 0.0f) * 200.0f;
    pLine->m_fLineWidth = angle.GetRadian() * 2;
    pLine->m_Color = ezColor(1.0f, 1.0f, 1.0f, 1.0f);

    auto pArrow = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
    pArrow->m_Start.Set(300, 100, 0.0f);
    pArrow->m_End.Set(250, 150, 0.0f);
    pArrow->m_fLineWidth = 1;
    pArrow->m_Color = ezColor(0.0f, 1.0f, 1.0f, 1.0f);

    auto pPoint = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pPoint->m_Position.Set(400, 100, 0.0f);
    pPoint->m_Color.SetRGB(ezVec3(1.0f, 0.0f, 0.0f));
    pPoint->m_fPointSize = angle.GetRadian() * 2;

    auto pCircle = pExtractor->AllocateRenderData<gpDrawData::Circle>();
    pCircle->m_Position.Set(100, 300, 0);
    pCircle->m_FillColor = ezColor(0.0f, 1.0f, 0.0f, 0.75f);
    pCircle->m_OutlineColor = ezColor(0.0f, 0.0f, 0.0f, 1.0f);
    pCircle->m_fOutlineWidth = angle.GetRadian() * 2;
    pCircle->m_fRadius = 50.0f;
    pCircle->m_uiNumLineSegments = 100;

    auto pPoly = pExtractor->AllocateRenderData<gpDrawData::Polygon>();
    pPoly->m_FillColor = ezColor(1.0f, 1.0f, 0.0f, 0.5f);
    pPoly->m_OutlineColor = ezColor(1.0f, 1.0f, 0.0f, 1.0f);
    static gpVec3 vertices[3] =
    {
        gpVec3(0.5f, 0.5f, 0.0f) * 100.0f,
        gpVec3(2.0f, 1.0f, 0.0f) * 100.0f,
        gpVec3(1.0f, 2.0f, 0.0f) * 100.0f,
    };
    pPoly->m_Vertices = vertices;
}

void gpExperimentsApp::AfterEngineInit()
{
    SetupLogging();
    LogSystemInformation();

    {
        EZ_LOG_BLOCK("Initialization");

        ezTelemetry::CreateServer();

        if (ezPlugin::LoadPlugin("ezInspectorPlugin").Failed())
        {
            ezLog::SeriousWarning("Failed to load ezInspectorPlugin.");
        }

        SetupWindow();
        ezStartup::StartupEngine();
        ezClock::SetNumGlobalClocks();
        ezClock::Get()->SetTimeStepSmoothing(AddressOf(m_TimeStepSmoother));
        m_LastUpdate = ezTime::Now();
        SetupInput();
        // Poll once to finish input initialization;
        ezInputManager::PollHardware();
        SetupRendering();
    }

    RunTestsIfEnabled();

    //gpRenderExtractor::AddExtractionListener(gpRenderExtractionListener(OnRenderExtraction));
    gpRenderExtractor::AddExtractionListener(
        gpRenderExtractionListener(&gpWorld::ExtractRenderingData, &g_World));
    PopulateWorld();
}

void gpExperimentsApp::BeforeEngineShutdown()
{
    ezStartup::ShutdownEngine();
    Cleanup();

    ezTelemetry::CloseConnection();
}

ezApplication::ApplicationExecution gpExperimentsApp::Run()
{
    m_pWindow->ProcessWindowMessages();

    if (m_bQuit)
        return Quit;

    ezClock::UpdateAllGlobalClocks();

    const auto tUpdateInterval = ezTime::Seconds(1.0 / 60.0);
    const auto tNow = ezTime::Now();

    bool bInputUpdated = false;

    while(tNow - m_LastUpdate > tUpdateInterval)
    {
        bInputUpdated = true;

        UpdateInput(tUpdateInterval);
        Update(tUpdateInterval);

        m_LastUpdate += tUpdateInterval;
    }

    // Make sure we don't miss any input events.
    if (!bInputUpdated)
    {
        ezInputManager::PollHardware();
    }

    RenderFrame();

    m_pWindow->PresentFrame();

    ezTaskSystem::FinishFrameTasks();

    ezTelemetry::PerFrameUpdate();

    return Continue;
}
