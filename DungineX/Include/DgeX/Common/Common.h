// precompiled header file for DungineX library

#pragma once

#include "DgeX/Common/Assert.h"
#include "DgeX/Common/Base.h"

// prevent windows.h from defining min and max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#ifdef INFINITE
#undef INFINITE
#endif
