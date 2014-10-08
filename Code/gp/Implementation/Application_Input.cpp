#include "gp/PCH.h"
#include "gp/Application.h"
#include "gp/Window.h"

#include <Foundation/Logging/ConsoleWriter.h>
#include <Foundation/Logging/VisualStudioWriter.h>
#include <Foundation/Time/Clock.h>
#include <Core/Input/InputManager.h>

static void RegisterInputAction(const char* szInputSet, const char* szInputAction, const char* szKey1, const char* szKey2 = nullptr, const char* szKey3 = nullptr)
{
  ezInputActionConfig cfg;

  cfg = ezInputManager::GetInputActionConfig(szInputSet, szInputAction);
  cfg.m_bApplyTimeScaling = true;

  if (szKey1 != nullptr)     cfg.m_sInputSlotTrigger[0] = szKey1;
  if (szKey2 != nullptr)     cfg.m_sInputSlotTrigger[1] = szKey2;
  if (szKey3 != nullptr)     cfg.m_sInputSlotTrigger[2] = szKey3;

  ezInputManager::SetInputActionConfig(szInputSet, szInputAction, cfg, true);
}

void gpApplication::SetupInput()
{
    m_pWindow->GetInputDevice()->SetClipMouseCursor(true);
    m_pWindow->GetInputDevice()->SetShowMouseCursor(true);
    m_pWindow->GetInputDevice()->SetMouseSpeed(ezVec2(0.002f));

    RegisterInputAction("Main", "Quit", ezInputSlot_KeyEscape);
}

void gpApplication::UpdateInput(ezTime dt)
{
    ezInputManager::Update(dt);

    if (ezInputManager::GetInputActionState("Main", "Quit") == ezKeyState::Pressed)
    {
        m_bQuit = true;
        return;
    }
}
