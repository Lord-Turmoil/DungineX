#pragma once

#include "DgeX/Application/Input/KeyCode.h"
#include "DgeX/Application/Input/MouseCode.h"

DGEX_BEGIN

class Input
{
public:
    static bool IsKeyPressed(KeyCode key);
    static bool IsMouseButtonPressed(MouseCode button);

    static MousePosition GetMousePosition();
    static double GetMouseX();
    static double GetMouseY();
};

DGEX_END
