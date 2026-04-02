#pragma once

#include "Minimal.h"
#include "EngineFactory.h"

int Init(FEngine* Engine, HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR CmdLine, int CmdShow) 
{
	int ReturnValue = Engine->PreInit(
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

	return ReturnValue;
}

void Tick(FEngine* Engine) 
{
	Engine->Tick();
}

int Exit(FEngine* Engine) 
{
	int ReturnValue = Engine->PreExit();
	ReturnValue = Engine->Exit();
	ReturnValue = Engine->PostExit();
	return ReturnValue;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR CmdLine, int CmdShow)
{
	int ReturnValue = 0;

	if (FEngine* Engine = FEngineFactory::CreateEngine())
	{
		ReturnValue = Init(Engine, hInstance, hPrevInstance, CmdLine, CmdShow);

		while (true) 
		{
			Tick(Engine);
		}

		ReturnValue = Exit(Engine);
	}
	else {
		ReturnValue = 1;
	}
	
	return ReturnValue;
}