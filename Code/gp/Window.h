#pragma once
#include <System/Window/Window.h>

class gpRenderer;

class gpWindow : public ezWindow
{
    friend gpRenderer;
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
    gpWindow(const char* szTitle);
    ~gpWindow();

    ezResult Initialize();

    ezResult CreateGraphicsContext();
    ezResult DestroyGraphicsContext();

    void PresentFrame();

    ezSizeU32 GetResolution() const;

    virtual void OnResizeMessage(const ezSizeU32& newWindowSize) override;
    virtual void OnClickCloseMessage() override;

    EZ_FORCE_INLINE void AddEventHandler(Event::Handler handler) { m_WindowEvent.AddEventHandler(handler); }
    EZ_FORCE_INLINE void RemoveEventHandler(Event::Handler handler) { m_WindowEvent.RemoveEventHandler(handler); }

    EZ_FORCE_INLINE static ezCVarInt* GetWidthCVar() { return &s_iWidth; }
    EZ_FORCE_INLINE static ezCVarInt* GetHeightCVar() { return &s_iHeight; }

private:
    ezResult CreateContextOpenGL();
    ezResult DestroyContextOpenGL();

private:
    Event m_WindowEvent;

    HDC m_hDeviceContext;
    HGLRC m_hRenderContext;

    static ezCVarInt s_iWidth;
    static ezCVarInt s_iHeight;
};
