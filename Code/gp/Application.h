#pragma once
#include <Core/Application/Application.h>
#include <Foundation/Memory/Allocator.h>
#include <Foundation/Time/DefaultTimeStepSmoothing.h>

#include "gp/Window.h"

class gpApplication : public ezApplication
{
public:
    gpApplication();
    ~gpApplication();

    virtual void BeforeEngineInit() override final;
    virtual void AfterEngineInit() override final;
    virtual void BeforeEngineShutdown() override final;
    virtual ApplicationExecution Run() override final;

    void WindowEventHandler(gpWindow::EventData* pEventData);

private:
    ezInt32 RunTests();

    void SetupInput();

    void UpdateInput(ezTime dt);

    void RenderFrame();

private:
    ezAllocatorBase* m_pMainAllocator;
    gpWindow* m_pWindow;
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;
    ezTime m_LastUpdate;

    bool m_bQuit;

    GLuint m_uiVertexArrayID;
    GLuint m_uiVertexBufferID;
};
