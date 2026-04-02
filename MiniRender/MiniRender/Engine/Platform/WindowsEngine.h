#pragma once

#include "../Core/Engine.h"

class FWindowsEngine : public FEngine {

public:
	virtual int PreInit(
#if defined(_WIN32)
		FWinCommandParameters InParams
#elif defined(__linux__)
		FLinuxCommandParameters InParams
#elif defined(__APPLE__)
		FAppleCommandParameters InParams
#endif
	);

	virtual int Init();
	virtual int PostInit();

	virtual void Tick();

	virtual int PreExit();
	virtual int Exit();
	virtual int PostExit();
};