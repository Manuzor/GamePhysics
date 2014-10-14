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

    virtual void AfterEngineInit() override;
    virtual void BeforeEngineShutdown() override;
    virtual ApplicationExecution Run() override;

    void WindowEventHandler(Ptr<gpWindow::EventData> pEventData);

private:
    void SetupInput();

    void UpdateInput(ezTime dt);

    void RenderFrame();

private:
    Ptr<ezAllocatorBase> m_pMainAllocator;
    Ptr<gpWindow> m_pWindow;
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;
    ezTime m_LastUpdate;

    bool m_bQuit;

    GLuint m_uiVertexArrayID;
    GLuint m_uiVertexBufferID;

    ezStaticArray<ezVec3, 3> m_VertexBufferData;
};
