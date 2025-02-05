#include "DgeX/Utils/PlatformUtils.h"

#ifdef DGEX_OPENGL

#include <GLFW/glfw3.h>

DGEX_BEGIN

namespace Time
{

timestamp_t GetTimestamp()
{
    return static_cast<timestamp_t>(glfwGetTime());
}

} // namespace Time

DGEX_END

#endif
