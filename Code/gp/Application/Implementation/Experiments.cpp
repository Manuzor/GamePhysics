#include "gp/PCH.h"

#include <Core/Input/InputManager.h>

#include "gp/Application/Experiments.h"
#include "gp/Rendering/Rendering.h"
#include "gp/Rendering/RenderExtractor.h"

static void OnRenderExtraction(gpRenderExtractor* pExtractor)
{
    auto dt = ezClock::Get()->GetAccumulatedTime().GetSeconds();
    auto angle = ezAngle::Radian(static_cast<float>(-dt));
    angle.NormalizeRange();

    auto pLine = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
    pLine->m_Start.SetZero();
    pLine->m_End.Set(ezMath::Cos(angle), ezMath::Sin(angle), 0.0f);
    pLine->m_fLineWidth = angle.GetRadian() * 2;
    pLine->m_Color = ezColor(1.0f, 1.0f, 1.0f, 1.0f);

    auto pArrow = pExtractor->AllocateRenderData<gpDrawData::Arrow>();
    pArrow->m_Start.Set(-0.25f, 0.25f, 0.0f);
    pArrow->m_End.Set(-0.75f, 0.75f, 0.0f);
    pArrow->m_fLineWidth = 1;
    pArrow->m_Color = ezColor(0.0f, 1.0f, 1.0f, 1.0f);

    auto pPoint = pExtractor->AllocateRenderData<gpDrawData::Point>();
    pPoint->m_Position.Set(0.5f, 0.5f, 0.0f);
    pPoint->m_Color.SetRGB(ezVec3(1.0f, 0.0f, 0.0f));
    pPoint->m_fPointSize = angle.GetRadian() * 2;

    auto pCircle = pExtractor->AllocateRenderData<gpDrawData::Circle>();
    pCircle->m_Position.Set(-0.5f, -0.5f, 0.0f);
    pCircle->m_FillColor = ezColor(0.0f, 1.0f, 0.0f, 0.75f);
    pCircle->m_OutlineColor = ezColor(0.0f, 0.0f, 0.0f, 1.0f);
    pCircle->m_fOutlineWidth = angle.GetRadian() * 2;
    pCircle->m_fRadius = 0.5f;
    pCircle->m_uiNumLineSegments = 100;

    auto pPoly = pExtractor->AllocateRenderData<gpDrawData::Polygon>();
    pPoly->m_FillColor = ezColor(1.0f, 1.0f, 0.0f, 0.5f);
    pPoly->m_OutlineColor = ezColor(1.0f, 1.0f, 0.0f, 1.0f);
    static gpVec3 vertices[3] =
    {
        gpVec3( 0.00f,  0.00f, 0.00f),
        gpVec3(-0.25f,  0.25f, 0.00f),
        gpVec3(-0.25f, -0.25f, 0.00f),
    };
    pPoly->m_Vertices = vertices;
}

void gpExperimentsApp::AfterEngineInit()
{
    SetupLogging();
    LogSystemInformation();

    {
        EZ_LOG_BLOCK("Initialization");

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

    gpRenderExtractor::AddExtractionListener(gpRenderExtractionListener(OnRenderExtraction));
}

void gpExperimentsApp::BeforeEngineShutdown()
{
    ezStartup::ShutdownEngine();
    Cleanup();
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

    return Continue;
}
