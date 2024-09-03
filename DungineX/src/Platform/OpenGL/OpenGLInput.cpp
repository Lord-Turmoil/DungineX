#include "DgeX/Application/Input/Input.h"
#include "Dgex/Application/Application.h"

#ifdef DGEX_OPENGL

#include <GLFW/glfw3.h>

DGEX_BEGIN

namespace Input
{

bool IsKeyPressed(KeyCode key)
{
    auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS;
}

bool IsMouseButtonPressed(MouseCode button)
{
    auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

MousePosition GetMousePosition()
{
    auto window = static_cast<GLFWwindow*>(Application::GetInstance()->GetWindow()->GetNativeWindow());
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    return { x, y };
}

double GetMouseX()
{
    auto [x, y] = GetMousePosition();
    return x;
}

double GetMouseY()
{
    auto [x, y] = GetMousePosition();
    return y;
}

} // namespace Input

DGEX_END

#endif
