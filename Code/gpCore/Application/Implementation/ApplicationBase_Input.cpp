#include "gpCore/PCH.h"

#include <Foundation/Time/Clock.h>
#include <Core/Input/InputManager.h>

#include "gpCore/Application/ApplicationBase.h"
#include "gpCore/Window.h"

void gpApplicationBase::RegisterInputAction(const char* szInputSet,
                                            const char* szInputAction,
                                            const char* szKey1,
                                            const char* szKey2,
                                            const char* szKey3)
{
  ezInputActionConfig cfg;

  cfg = ezInputManager::GetInputActionConfig(szInputSet, szInputAction);
  cfg.m_bApplyTimeScaling = true;

  if (szKey1 != nullptr)     cfg.m_sInputSlotTrigger[0] = szKey1;
  if (szKey2 != nullptr)     cfg.m_sInputSlotTrigger[1] = szKey2;
  if (szKey3 != nullptr)     cfg.m_sInputSlotTrigger[2] = szKey3;

  ezInputManager::SetInputActionConfig(szInputSet, szInputAction, cfg, true);
}

void gpApplicationBase::SetupInput()
{
    m_pWindow->GetInputDevice()->SetClipMouseCursor(false);
    m_pWindow->GetInputDevice()->SetShowMouseCursor(true);
    m_pWindow->GetInputDevice()->SetMouseSpeed(ezVec2(0.002f));

    RegisterInputAction("Main", "Quit", ezInputSlot_KeyEscape);
    RegisterInputAction("Main", "ClipMouse", ezInputSlot_KeyC);
}

void gpApplicationBase::UpdateInput(ezTime dt)
{
    ezInputManager::Update(dt);

    if (ezInputManager::GetInputActionState("Main", "Quit") == ezKeyState::Pressed)
    {
        m_bQuit = true;
        return;
    }

    if (ezInputManager::GetInputActionState("Main", "ClipMouse") == ezKeyState::Pressed)
    {
        auto pInput = m_pWindow->GetInputDevice();
        auto bDoClip = !pInput->GetClipMouseCursor();
        pInput->SetClipMouseCursor(bDoClip);

        if(bDoClip)
            ezLog::Info("Mouse cursor will be clipped. Click inside the window.");
        else
            ezLog::Info("Mouse cursor is no longer clipped.");
    }
}
