#pragma once

void install_debugger(void);

#ifdef _WIN32
inline void install_debugger(void)
{
}
#endif
