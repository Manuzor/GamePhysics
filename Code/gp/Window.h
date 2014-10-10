#pragma once
#include <System/Window/Window.h>

class gpWindow : public ezWindow
{
public:

    enum EventReason
    {
        ClickClose,
    };

    struct EventData
    {
        EventReason m_Reason;
    };

    using Event = ezEvent<EventData*>;
public:
    gpWindow();
    ~gpWindow();

    ezResult CreateGraphicsContext();
    ezResult DestroyGraphicsContext();

    void PresentFrame();

    ezSizeU32 GetResolution() const;

    virtual void OnResizeMessage(const ezSizeU32& newWindowSize) override;
    virtual void OnClickCloseMessage() override;

    EZ_FORCE_INLINE void AddEventHandler(Event::Handler handler) { m_WindowEvent.AddEventHandler(handler); }
    EZ_FORCE_INLINE void RemoveEventHandler(Event::Handler handler) { m_WindowEvent.RemoveEventHandler(handler); }

private:
    ezResult CreateContextOpenGL();
    ezResult DestroyContextOpenGL();

private:
    Event m_WindowEvent;

    HDC m_hDeviceContext;
    HGLRC m_hRenderContext;
};
