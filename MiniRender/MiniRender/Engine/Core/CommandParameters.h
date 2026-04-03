#pragma once

#include "../Minimal.h"

#if defined(_WIN32)
class FWinCommandParameters
{
public:
    FWinCommandParameters(HINSTANCE InInstance, HINSTANCE InPrevInstance, PSTR InCmdLine, int InCmdShow);

    HINSTANCE Instance;
    HINSTANCE PrevInstance;
    PSTR CmdLine;
    int CmdShow;
};

#elif defined(__linux__)
class FLinuxCommandParameters {
public:
};

#elif defined(__APPLE__)
class FAppleCommandParameters {
public:
};
#endif
