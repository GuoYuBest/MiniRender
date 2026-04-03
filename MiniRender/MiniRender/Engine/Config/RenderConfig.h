#pragma once

struct FRenderConfig
{
    FRenderConfig();

    static FRenderConfig* GetRenderConfig();
    static void Destory();

    int ScreenWidth;
    int ScreenHeight;
    int RefreshRate;
    int SwapChainCount;

    static FRenderConfig* RenderConfig;
};
