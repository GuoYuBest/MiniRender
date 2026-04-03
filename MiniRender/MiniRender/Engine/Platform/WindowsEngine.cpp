
#include "WindowsEngine.h"
#include "MessageProcess.h"

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
	const char LogPath[] = "../log";
	init_log_system(LogPath);
	Engine_Log("Log Init");
	
	if (InitWindows(InParams))
	{
		
	}

	Engine_Log("Windows PreInit Success!");
	
	return 0;
}

int FWindowsEngine::Init()
{
	Engine_Log("Windows Init Success!");

	return 0;
}

int FWindowsEngine::PostInit()
{
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

	RECT Rect = {0,0,1280,720};
	AdjustWindowRect(&Rect, WS_OVERLAPPEDWINDOW, false);

	int WinWidth = Rect.right - Rect.left;
	int WinHeight = Rect.bottom - Rect.top;

	HWND Window = CreateWindowEx(
		NULL,
		L"MiniRender",
		L"MiniRender",
		WS_OVERLAPPEDWINDOW
		,100,100,
		WinWidth, WinHeight,
		nullptr,
		nullptr,
		InParams.Instance,
		nullptr);

	if (!Window)
	{
		MessageBox(nullptr, L"CreateWindowEx Failed!", L"Error", MB_OK);
		Engine_Log_Error("Windows CreateWindowEx Failed!");
		return false;
	}

	ShowWindow(Window, SW_SHOW);
	UpdateWindow(Window);

	Engine_Log("Windows InitWindows Success!");
}