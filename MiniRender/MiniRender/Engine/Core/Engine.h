#pragma once

#if defined(_WIN32)
#include "CommandParameters.h"
#endif //

class FEngine {
public:
	virtual int PreInit(
#if defined(_WIN32)
		FWinCommandParameters InParams
#elif defined(__linux__)
		FLinuxCommandParameters InParams
#elif defined(__APPLE__)
		FAppleCommandParameters InParams
#endif
	) = 0;

	virtual int Init() = 0;
	virtual int PostInit() = 0;

	virtual void Tick() = 0;

	virtual int PreExit() = 0;
	virtual int Exit() = 0;
	virtual int PostExit() = 0;
};