#pragma once

#if defined(_WIN32)
#include "../../../simple_library/public/simple_library.h"

#define Engine_Log(format, ...) log_log(format, ##__VA_ARGS__)
#define Engine_Log_Error(format, ...) log_log(format, ##__VA_ARGS__)
#endif
