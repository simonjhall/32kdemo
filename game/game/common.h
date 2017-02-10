#pragma once

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 256

#ifdef _WIN32
#include <assert.h>
#define ASSERT(x) assert(x)
#else
#define ASSERT(x)
#endif
