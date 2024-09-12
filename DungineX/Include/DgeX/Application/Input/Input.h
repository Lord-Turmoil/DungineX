#pragma once

#include "DgeX/Application/Input/KeyCode.h"
#include "DgeX/Application/Input/MouseCode.h"

DGEX_BEGIN

namespace Input
{

bool IsKeyPressed(KeyCode key);
bool IsMouseButtonPressed(MouseCode button);

MousePosition GetMousePosition();
MousePosition GetInverseMousePosition();
double GetMouseX();
double GetMouseY();
double GetMouseInverseY();

}; // namespace Input

DGEX_END
