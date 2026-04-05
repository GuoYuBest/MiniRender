#include "WindowsEngine.h"
#include "MessageProcess.h"
#include "../Config/RenderConfig.h"

FWindowsEngine::FWindowsEngine()
    : M4XQualityLevels(0)
    , bMSAA4XEnabled(false)
    , BackBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM)
    , DepthStencilFormat(DXGI_FORMAT_D24_UNORM_S8_UINT)
{
    FRenderConfig* Config = FRenderConfig::GetRenderConfig();

    for (int i = 0; i < Config->SwapChainCount; i++)
    {
        SwapChainBuffer.push_back(ComPtr<ID3D12Resource>());
    }
}

int FWindowsEngine::PreInit(
#if defined(_WIN32)
    FWinCommandParameters InParams
#elif defined(__linux__)
	FLinuxCommandParameters InParams
#elif defined(__APPLE__)
	FAppleCommandParameters InParams
#endif
)
{
    constexpr char LogPath[] = "../log";
    init_log_system(LogPath);
    Engine_Log("Log Init");
    return 0;
}

int FWindowsEngine::Init(FWinCommandParameters InParams)
{
    Engine_Log("Windows Init Success!");

    if (InitWindows(InParams))
    {
    }

    if (InitDirect3D())
    {
    }

    return 0;
}

int FWindowsEngine::PostInit()
{
    FRenderConfig* Config = FRenderConfig::GetRenderConfig();
    for (ComPtr<ID3D12Resource>& Buffer : SwapChainBuffer)
    {
        Buffer.Reset();
    }
    DepthStencilBuffer.Reset();
    SwapChain->ResizeBuffers(
        Config->SwapChainCount,
        Config->ScreenWidth,
        Config->ScreenHeight,
        BackBufferFormat,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
    );
    RTVDescSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    CD3DX12_CPU_DESCRIPTOR_HANDLE HeapHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
    for (int idx = 0; idx < Config->SwapChainCount; idx++)
    {
        SwapChain->GetBuffer(idx, IID_PPV_ARGS(&SwapChainBuffer[idx]));
        D3dDevice->CreateRenderTargetView(SwapChainBuffer[idx].Get(), nullptr, HeapHandle);
        HeapHandle.Offset(1, RTVDescSize);
    }

    D3D12_RESOURCE_DESC ResDesc;
    ResDesc.Width = Config->ScreenWidth;
    ResDesc.Height = Config->ScreenHeight;
    ResDesc.Alignment = 0;
    ResDesc.MipLevels = 1;
    ResDesc.DepthOrArraySize = 1;
	ResDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	ResDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	ResDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
    ResDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
    ResDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    ResDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;

    D3D12_CLEAR_VALUE ClearVal;
    ClearVal.DepthStencil.Depth = 1.0f;
    ClearVal.DepthStencil.Stencil = 0;
    ClearVal.Format = DepthStencilFormat;

    D3D12_HEAP_PROPERTIES Head = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    D3dDevice->CreateCommittedResource(
        &Head,
        D3D12_HEAP_FLAG_NONE,
        &ResDesc,
        D3D12_RESOURCE_STATE_COMMON,
        &ClearVal, 
        IID_PPV_ARGS(DepthStencilBuffer.GetAddressOf()));

    D3D12_DEPTH_STENCIL_VIEW_DESC DSVDesc;
    DSVDesc.Format = DepthStencilFormat;
    DSVDesc.Texture2D.MipSlice = 0;
    DSVDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
    DSVDesc.Flags = D3D12_DSV_FLAG_NONE;
    D3dDevice->CreateDepthStencilView(
        DepthStencilBuffer.Get(),
        &DSVDesc,
        DSVHeap->GetCPUDescriptorHandleForHeapStart());

    D3D12_RESOURCE_BARRIER Barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        DepthStencilBuffer.Get(),
        D3D12_RESOURCE_STATE_COMMON,
        D3D12_RESOURCE_STATE_DEPTH_WRITE
    );
    GraphicsCommandList->ResourceBarrier(1, &Barrier);
    GraphicsCommandList->Close();

    ID3D12CommandList* CommandLists[] = { GraphicsCommandList.Get() };
    CommandQueue->ExecuteCommandLists(_countof(CommandLists), CommandLists);

    Engine_Log("Windows PostInit Success!");
    return 0;
}

void FWindowsEngine::Tick()
{
}

int FWindowsEngine::PreExit()
{
    Engine_Log("Windows PreExit Success!");

    return 0;
}

int FWindowsEngine::Exit()
{
    Engine_Log("Windows Exit Success!");

    return 0;
}

int FWindowsEngine::PostExit()
{
    Engine_Log("Windows PostExit Success!");
    FRenderConfig::Destory();
    return 0;
}

bool FWindowsEngine::InitWindows(FWinCommandParameters InParams)
{
    WNDCLASSEX WinClass;
    WinClass.cbSize = sizeof(WNDCLASSEX);
    WinClass.cbClsExtra = 0;
    WinClass.cbWndExtra = 0;
    WinClass.hbrBackground = nullptr;
    WinClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    WinClass.hIcon = nullptr;
    WinClass.hIconSm = nullptr;
    WinClass.hInstance = InParams.Instance;
    WinClass.lpszClassName = L"MiniRender";
    WinClass.lpszMenuName = nullptr;
    WinClass.style = CS_VREDRAW | CS_HREDRAW;
    WinClass.lpfnWndProc = WinProc;

    ATOM RegisterAtom = RegisterClassEx(&WinClass);
    if (!RegisterAtom)
    {
        Engine_Log_Error("Windows RegisterClassEx Failed!");

        MessageBox(nullptr, L"RegisterClassEx Failed!", L"Error", MB_OK);
    }

    FRenderConfig* Config = FRenderConfig::GetRenderConfig();
    RECT Rect = {0, 0, Config->ScreenWidth, Config->ScreenHeight};
    AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);

    int WinWidth = Rect.right - Rect.left;
    int WinHeight = Rect.bottom - Rect.top;

    MainWindowsHandle = CreateWindowEx(
        NULL,
        L"MiniRender",
        L"MiniRender",
        WS_OVERLAPPEDWINDOW
        , 100, 100,
        WinWidth, WinHeight,
        nullptr,
        nullptr,
        InParams.Instance,
        nullptr);

    if (!MainWindowsHandle)
    {
        MessageBox(nullptr, L"CreateWindowEx Failed!", L"Error", MB_OK);
        Engine_Log_Error("Windows CreateWindowEx Failed!");
        return false;
    }

    ShowWindow(MainWindowsHandle, SW_SHOW);
    UpdateWindow(MainWindowsHandle);

    Engine_Log("Windows InitWindows Success!");
}

bool FWindowsEngine::InitDirect3D()
{
    ComPtr<ID3D12Debug> D3D12Debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12Debug))))
    {
        D3D12Debug->EnableDebugLayer();
    }
    
    ANALYZE_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));

    HRESULT D3dDeviceResult = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
    if (FAILED(D3dDeviceResult))
    {
        ComPtr<IDXGIAdapter> WarpAdapter;
        ANALYZE_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WarpAdapter)));
        ANALYZE_HRESULT(D3D12CreateDevice(WarpAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice)));
    }

    ANALYZE_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));

    D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
    QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    ANALYZE_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue)));

    ANALYZE_HRESULT(D3dDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

    HRESULT CmdResult = D3dDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        CommandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(GraphicsCommandList.GetAddressOf()));

    if (FAILED(CmdResult))
    {
        Engine_Log_Error("Error = %i", (int)CmdResult);
    }

    GraphicsCommandList->Close();

    // Set MSAA
    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
    QualityLevels.SampleCount = 4;
    QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    QualityLevels.NumQualityLevels = 0;
    ANALYZE_HRESULT(D3dDevice->CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &QualityLevels,
        sizeof(QualityLevels)));
    M4XQualityLevels = QualityLevels.NumQualityLevels;

    // 交换链
    SwapChain.Reset();
    DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
    FRenderConfig* Config = FRenderConfig::GetRenderConfig();
    SwapChainDesc.BufferDesc.Height = Config->ScreenHeight;
    SwapChainDesc.BufferDesc.Width = Config->ScreenWidth;
    SwapChainDesc.BufferDesc.RefreshRate.Numerator = Config->RefreshRate;
    SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    SwapChainDesc.BufferCount = Config->SwapChainCount;
    SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    SwapChainDesc.OutputWindow = MainWindowsHandle;
    SwapChainDesc.Windowed = true;
    SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    SwapChainDesc.BufferDesc.Format = BackBufferFormat;

    SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
    SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;

    CmdResult = DXGIFactory->CreateSwapChain(CommandQueue.Get(), &SwapChainDesc, SwapChain.GetAddressOf());
    if (FAILED(CmdResult))
    {
        Engine_Log_Error("Error = %i", (int)CmdResult);
    }

    // Create RTV
    D3D12_DESCRIPTOR_HEAP_DESC RTVDesc;
    RTVDesc.NumDescriptors = Config->SwapChainCount;
    RTVDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    RTVDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    RTVDesc.NodeMask = 0;
    ANALYZE_HRESULT(D3dDevice->CreateDescriptorHeap(&RTVDesc, IID_PPV_ARGS(RTVHeap.GetAddressOf())));

    // Create DSV
    D3D12_DESCRIPTOR_HEAP_DESC DSVDesc;
    DSVDesc.NumDescriptors = 1;
    DSVDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    DSVDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    DSVDesc.NodeMask = 0;
    ANALYZE_HRESULT(D3dDevice->CreateDescriptorHeap(&DSVDesc, IID_PPV_ARGS(DSVHeap.GetAddressOf())));

    /*GraphicsCommandList->ResourceBarrier(
        1,
        &D3D12_RESOURCE_BARRIER::Transition(DSVDesc));*/
    return false;
}
