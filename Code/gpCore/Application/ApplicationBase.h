#pragma once
#include <Core/Application/Application.h>
#include <Foundation/Memory/Allocator.h>
#include <Foundation/Time/DefaultTimeStepSmoothing.h>

#include "gpCore/Window.h"

class gpRenderer;
class gpRenderExtractor;

/// \brief Provides some basic functionality that can be used by sub-classes.
class GP_CoreAPI gpApplicationBase : public ezApplication
{
public:
    gpApplicationBase(const char* szTitle);
    ~gpApplicationBase();

    // Input
    //////////////////////////////////////////////////////////////////////////
    /// \brief Sets some basic input stuff (e.g. register Quit action).
    void SetupInput();

    /// \brief Updates input and checks if some basic input action states (e.g. Quit)
    virtual void UpdateInput(ezTime dt);

    /// \brief Maps an input action (e.g. Quit) to one or more keys (e.g. Escape).
    void RegisterInputAction(const char* szInputSet,
                             const char* szInputAction,
                             const char* szKey1,
                             const char* szKey2 = nullptr,
                             const char* szKey3 = nullptr);

    // Window
    //////////////////////////////////////////////////////////////////////////
    void SetupWindow();

    // Logging
    //////////////////////////////////////////////////////////////////////////
    /// \brief Registers logging handlers.
    void SetupLogging();

    // Misc
    //////////////////////////////////////////////////////////////////////////
    /// \brief Logs some basic system information (host OS, etc).
    void LogSystemInformation();

    /// \brief Cleans everything up that was created during setup.
    void Cleanup();

    // Rendering
    //////////////////////////////////////////////////////////////////////////
    /// \brief Instanciates the render extractor.
    void SetupRendering();

    /// \brief Triggers an extraction and renders the data.
    void RenderFrame();

private:

    void WindowEventHandler(gpWindow::EventData* pEventData);

protected:
    const char* m_szTitle;
    gpWindow* m_pWindow;
    bool m_bQuit;

private:
    bool m_bRegisteredLogging;
};
