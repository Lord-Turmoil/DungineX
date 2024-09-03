#pragma once

#include "DgeX/Core/Base.h"

#include "DgeX/Application/Event/ApplicationEvent.h"
#include "DgeX/Application/Event/EventEmitter.h"
#include "DgeX/Application/Event/KeyEvent.h"
#include "DgeX/Application/Event/MouseEvent.h"
#include "DgeX/Application/Input/Input.h"
#include "Dgex/Application/Application.h"
#include "Dgex/Application/Interface/Interface.h"
#include "Dgex/Application/Interface/Layer.h"

#include "DgeX/Renderer/Camera/SceneCamera.h"
#include "DgeX/Renderer/Color.h"
#include "DgeX/Renderer/RenderApi.h"
#include "DgeX/Renderer/RenderCommand.h"

#include "DgeX/Utils/Easing.h"
#include "DgeX/Utils/String.h"

#include "DgeX/Library/IntrusiveList.h"
#include "DgeX/Library/ObjectPool.h"
#include "DgeX/Library/ObjectQueue.h"

#include "DgeX/Physics/Physics.h"

#undef __DGEX__CORE__
#define __DGEX__CLIENT__
