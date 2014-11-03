
void gpRenderer::Initialize()
{
    if (IsInitialized())
        return;

    {
        auto pWidth = static_cast<ezCVarInt*>(ezCVar::FindCVarByName("Window.Width"));
        EZ_ASSERT(pWidth, "Need global CVar Window.Width!");
        s_Resolution.width = pWidth->GetValue();
        pWidth->m_CVarEvents.AddEventHandler(WidthChangedCallback);
    }

    {
        auto pHeight = static_cast<ezCVarInt*>(ezCVar::FindCVarByName("Window.Height"));
        EZ_ASSERT(pHeight, "Need global CVar Window.Height!");
        s_Resolution.height = pHeight->GetValue();
        pHeight->m_CVarEvents.AddEventHandler(HeightChangedCallback);
    }

    s_bInitialized = true;
}

void gpRenderer::Shutdown()
{
    if (!IsInitialized())
        return;

    {
        auto pWidth = static_cast<ezCVarInt*>(ezCVar::FindCVarByName("Window.Width"));
        EZ_ASSERT(pWidth, "Need global CVar Window.Width!");
        pWidth->m_CVarEvents.RemoveEventHandler(WidthChangedCallback);
    }

    {
        auto pHeight = static_cast<ezCVarInt*>(ezCVar::FindCVarByName("Window.Height"));
        EZ_ASSERT(pHeight, "Need global CVar Window.Height!");
        pHeight->m_CVarEvents.RemoveEventHandler(HeightChangedCallback);
    }

    s_bInitialized = false;
}
