#pragma once

#include "../Core/Engine.h"

#if defined(_WIN32)
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif