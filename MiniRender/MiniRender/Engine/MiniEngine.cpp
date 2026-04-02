#pragma once

#include "Minimal.h"
#include "EngineFactory.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR CmdLine, int CmdShow)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		ReturnValue = Engine->PreInit(
#if defined(_WIN32)
			FWinCommandParameters(hInstance, hPrevInstance, CmdLine, CmdShow)
#elif defined(__linux__)
			FLinuxCommandParameters()
#elif defined(__APPLE__)
			FAppleCommandParameters()
#endif
		);

		ReturnValue = Engine->Init();
		ReturnValue = Engine->PostInit();

		while (true) 
		{
			Engine->Tick();
		}

		ReturnValue = Engine->PreExit();
		ReturnValue = Engine->Exit();
		ReturnValue = Engine->PostExit();

		ReturnValue = 0;
	}
	else {
		ReturnValue = 1;
	}
	
	return ReturnValue;
}