#include "EngineFactory.h"

#if defined(_WIN32)
#include "Platform/WindowsEngine.h"
#endif

FEngineFactory::FEngineFactory()
{
}

FEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
    return new FWindowsEngine();
#elif defined(__linux__)

#elif defined(__APPLE__)

#endif
    return nullptr;
}
