// precompiled header file for DungineX library

#pragma once

#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "DgeX/Common/Base.h"

#include "DgeX/Common/Assert.h"
#include "DgeX/Common/DeltaTime.h"
#include "DgeX/Common/Log.h"
#include "DgeX/Common/Macros.h"

// prevent windows.h from defining min and max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
