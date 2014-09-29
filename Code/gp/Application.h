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

    void WindowEventHandler(gpWindow::EventData* pEventData);

private:
    ezAllocatorBase* m_pMainAllocator;
    gpWindow* m_pWindow;
    ezDefaultTimeStepSmoothing m_TimeStepSmoother;

    bool m_bQuit;
};
