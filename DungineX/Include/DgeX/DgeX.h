#pragma once

#include "DgeX/Common/Base.h"
#include "DgeX/Utils/String.h"

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/EventEmitter.h"
#include "DgeX/Application/Event/KeyEvent.h"
#include "DgeX/Application/Event/MouseEvent.h"
#include "Dgex/Application/Application.h"

#include "DgeX/Renderer/RenderApi.h"

#include "DgeX/Utils/Easing.h"
#include "DgeX/Utils/String.h"

#include "Dgex/EntryPoint.h"

#undef __DGEX__CORE__
#define __DGEX__CLIENT__

#if DGEX_OPENGL
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glfw3.lib")
#endif
