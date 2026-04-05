#pragma once

#include "../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
    FWindowsEngine();

    int PreInit(
#if defined(_WIN32)
        FWinCommandParameters InParams
#elif defined(__linux__)
		FLinuxCommandParameters InParams
#elif defined(__APPLE__)
		FAppleCommandParameters InParams
#endif
    ) override;

    int Init(FWinCommandParameters InParams) override;
    int PostInit() override;

    void Tick() override;

    int PreExit() override;
    int Exit() override;
    int PostExit() override;

private:
    bool InitWindows(FWinCommandParameters InParams);

    bool InitDirect3D();

protected:
    ComPtr<IDXGIFactory4> DXGIFactory;
    ComPtr<ID3D12Device> D3dDevice;
    ComPtr<ID3D12Fence> Fence;

    ComPtr<ID3D12CommandQueue> CommandQueue;
    ComPtr<ID3D12CommandAllocator> CommandAllocator;
    ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;

    ComPtr<IDXGISwapChain> SwapChain;

    ComPtr<ID3D12DescriptorHeap> RTVHeap;
    ComPtr<ID3D12DescriptorHeap> DSVHeap;

    vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
    ComPtr<ID3D12Resource> DepthStencilBuffer;

    HWND MainWindowsHandle;
    UINT M4XQualityLevels;
    bool bMSAA4XEnabled;
    DXGI_FORMAT BackBufferFormat;
    DXGI_FORMAT DepthStencilFormat;
    UINT RTVDescSize;
};
