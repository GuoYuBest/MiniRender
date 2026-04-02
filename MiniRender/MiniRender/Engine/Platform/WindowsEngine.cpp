
#include "WindowsEngine.h"

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
	return 0;
}

int FWindowsEngine::Init()
{
	return 0;
}

int FWindowsEngine::PostInit()
{
	return 0;
}

void FWindowsEngine::Tick()
{
}

int FWindowsEngine::PreExit()
{
	return 0;
}

int FWindowsEngine::Exit()
{
	return 0;
}

int FWindowsEngine::PostExit()
{
	return 0;
}