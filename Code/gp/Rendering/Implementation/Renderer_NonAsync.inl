
void gpRenderer::Initialize()
{
    if (IsInitialized())
        return;

    s_bInitialized = true;
}

void gpRenderer::Shutdown()
{
    if (!IsInitialized())
        return;

    s_bInitialized = false;
}
