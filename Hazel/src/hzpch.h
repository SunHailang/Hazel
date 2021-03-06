#pragma once

#include "Hazel/Core/PlatformDetection.h"

#ifdef HZ_PLATFORM_WINDOWS
	#ifndef NOMINMAX
	// See github.com/skypjack/entt/wiki/Frequently-Asked-Questions#warning-c4003-the-min-the-max-and-the-macro
		#define NOMINMAX
	#endif
#endif

#include <iostream>
#include <memory>
#include <utility>
#include <sstream>
#include <fstream>
#include <array>
#include <algorithm>
#include <functional>
#include <string>

#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Hazel/Core/Log.h"
#include "Hazel/Debug/Instrumentor.h"

#ifdef  HZ_PLATFORM_WINDOWS
	#include <Windows.h>
#endif //  HZ_PLATFORM_WINDOWS

