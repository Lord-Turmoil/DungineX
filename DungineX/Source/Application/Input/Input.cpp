#include "DgeX/Application/Input/Input.h"
#include "Dgex/Application/Application.h"

DGEX_BEGIN

namespace Input
{

MousePosition GetInverseMousePosition()
{
    auto window = Application::GetInstance()->GetWindow()->GetHeight();
    auto [x, y] = GetMousePosition();
    return { x, window - y };
}

double GetMouseInverseY()
{
    return GetInverseMousePosition().y;
}

} // namespace Input

DGEX_END
