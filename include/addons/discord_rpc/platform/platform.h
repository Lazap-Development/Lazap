#pragma once
#ifndef DISCORD_PLATFORM_H
#define DISCORD_PLATFORM_H

#include <cstdint>

#ifdef _WIN32
#include "windows.h"
#elif defined(__linux__)
#include "unix.h"
#elif defined(__APPLE__)
#include "unix.h"
#endif

#endif  // DISCORD_PLATFORM_H