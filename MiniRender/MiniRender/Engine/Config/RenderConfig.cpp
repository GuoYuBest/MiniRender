#include "RenderConfig.h"

FRenderConfig* FRenderConfig::RenderConfig;

FRenderConfig::FRenderConfig()
    : ScreenWidth(1280), ScreenHeight(720), RefreshRate(60), SwapChainCount(2)
{
}

FRenderConfig* FRenderConfig::GetRenderConfig()
{
    if (!RenderConfig)
    {
        return new FRenderConfig();
    }
    return RenderConfig;
}

void FRenderConfig::Destory()
{
    if (RenderConfig)
    {
        delete RenderConfig;
        RenderConfig = nullptr;
    }
}
