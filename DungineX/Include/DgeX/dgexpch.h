#pragma once

#include "DgeX/Common/Macros.h"

#include "DgeX/Common/Log.h"

#include "DgeX/Common/Assert.h"
#include "DgeX/Common/Base.h"

#ifdef INFINITE
#undef INFINITE
#endif

#ifdef ERROR
#undef ERROR
#endif

#include <msdf-atlas-gen/msdf-atlas-gen.h>
#include <msdfgen/msdfgen.h>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <filesystem>
#include <string>
#include <unordered_set>
#include <vector>

// prevent windows.h from defining min and max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
