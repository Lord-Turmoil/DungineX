#pragma once

DGEX_BEGIN

using MouseCode = uint16_t;

struct MousePosition
{
    double x;
    double y;
};

namespace Mouse
{

// clang-format off
enum : MouseCode // NOLINT(performance-enum-size), keep align with KeyCode
{
    // From glfw3.h
    Button0                = 0,
    Button1                = 1,
    Button2                = 2,
    Button3                = 3,
    Button4                = 4,
    Button5                = 5,
    Button6                = 6,
    Button7                = 7,

    ButtonLast             = Button7,
    ButtonLeft             = Button0,
    ButtonRight            = Button1,
    ButtonMiddle           = Button2
};

// clang-format on

} // namespace Mouse

DGEX_END
