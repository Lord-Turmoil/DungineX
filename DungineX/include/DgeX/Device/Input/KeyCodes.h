/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : KeyCodes.h                                 *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : July 31, 2025                             *
 *                                                                            *
 *                    Last Update : July 31, 2025                             *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Key and mouse code definition.                                             *
 * -------------------------------------------------------------------------- *
 * REFERENCES:                                                                *
 *                                                                            *
 * From glfw3.h                                                               *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"

#include <SDL3/SDL.h>

#include <cstdint>

DGEX_BEGIN

// clang-format off

using KeyCode = uint32_t;

/**
 * @brief Key codes enumeration.
 *
 * Key code values are based on Windows virtual key codes, and mapped to SDL scancodes.
 */
enum class KeyCodes : KeyCode
{
    Backspace           = SDL_SCANCODE_BACKSPACE, /* Backspace */
    Tab                 = SDL_SCANCODE_TAB, /* Tab */

    Clear               = SDL_SCANCODE_CLEAR, /* Clear */
    Return              = SDL_SCANCODE_RETURN, /* Enter */

    Pause               = SDL_SCANCODE_PAUSE, /* Pause */
    CapsLock            = SDL_SCANCODE_CAPSLOCK, /* Caps Lock */

    Escape              = SDL_SCANCODE_ESCAPE, /* Escape */

    Space               = SDL_SCANCODE_SPACE, /* Space */
    PageUp              = SDL_SCANCODE_PAGEUP, /* Page Up */
    PageDown            = SDL_SCANCODE_PAGEDOWN, /* Page Down */
    End                 = SDL_SCANCODE_END, /* End */
    Home                = SDL_SCANCODE_HOME, /* Home */
    Left                = SDL_SCANCODE_LEFT, /* Left Arrow */
    Up                  = SDL_SCANCODE_UP, /* Up Arrow */
    Right               = SDL_SCANCODE_RIGHT, /* Right Arrow */
    Down                = SDL_SCANCODE_DOWN, /* Down Arrow */
    Select              = SDL_SCANCODE_SELECT, /* Select */
    Print               = SDL_SCANCODE_PRINTSCREEN, /* Print */
    Execute             = SDL_SCANCODE_EXECUTE, /* Execute */
    Insert              = SDL_SCANCODE_INSERT, /* Insert */
    Delete              = SDL_SCANCODE_DELETE, /* Delete */
    Help                = SDL_SCANCODE_HELP, /* Help */

    D0                  = SDL_SCANCODE_0, /* 0 */
    D1                  = SDL_SCANCODE_1, /* 1 */
    D2                  = SDL_SCANCODE_2, /* 2 */
    D3                  = SDL_SCANCODE_3, /* 3 */
    D4                  = SDL_SCANCODE_4, /* 4 */
    D5                  = SDL_SCANCODE_5, /* 5 */
    D6                  = SDL_SCANCODE_6, /* 6 */
    D7                  = SDL_SCANCODE_7, /* 7 */
    D8                  = SDL_SCANCODE_8, /* 8 */
    D9                  = SDL_SCANCODE_9, /* 9 */

    Semicolon           = SDL_SCANCODE_SEMICOLON, /* ; */
    Equal               = SDL_SCANCODE_EQUALS, /* = */

    A                   = SDL_SCANCODE_A, /* A */
    B                   = SDL_SCANCODE_B, /* B */
    C                   = SDL_SCANCODE_C, /* C */
    D                   = SDL_SCANCODE_D, /* D */
    E                   = SDL_SCANCODE_E, /* E */
    F                   = SDL_SCANCODE_F, /* F */
    G                   = SDL_SCANCODE_G, /* G */
    H                   = SDL_SCANCODE_H, /* H */
    I                   = SDL_SCANCODE_I, /* I */
    J                   = SDL_SCANCODE_J, /* J */
    K                   = SDL_SCANCODE_K, /* K */
    L                   = SDL_SCANCODE_L, /* L */
    M                   = SDL_SCANCODE_M, /* M */
    N                   = SDL_SCANCODE_N, /* N */
    O                   = SDL_SCANCODE_O, /* O */
    P                   = SDL_SCANCODE_P, /* P */
    Q                   = SDL_SCANCODE_Q, /* Q */
    R                   = SDL_SCANCODE_R, /* R */
    S                   = SDL_SCANCODE_S, /* S */
    T                   = SDL_SCANCODE_T, /* T */
    U                   = SDL_SCANCODE_U, /* U */
    V                   = SDL_SCANCODE_V, /* V */
    W                   = SDL_SCANCODE_W, /* W */
    X                   = SDL_SCANCODE_X, /* X */
    Y                   = SDL_SCANCODE_Y, /* Y */
    Z                   = SDL_SCANCODE_Z, /* Z */

    Numpad0            = SDL_SCANCODE_KP_0, /* NumPad 0 */
    Numpad1            = SDL_SCANCODE_KP_1, /* NumPad 1 */
    Numpad2            = SDL_SCANCODE_KP_2, /* NumPad 2 */
    Numpad3            = SDL_SCANCODE_KP_3, /* NumPad 3 */
    Numpad4            = SDL_SCANCODE_KP_4, /* NumPad 4 */
    Numpad5            = SDL_SCANCODE_KP_5, /* NumPad 5 */
    Numpad6            = SDL_SCANCODE_KP_6, /* NumPad 6 */
    Numpad7            = SDL_SCANCODE_KP_7, /* NumPad 7 */
    Numpad8            = SDL_SCANCODE_KP_8, /* NumPad 8 */
    Numpad9            = SDL_SCANCODE_KP_9, /* NumPad 9 */

    Multiply           = SDL_SCANCODE_KP_MULTIPLY, /* NumPad * */
    Add                = SDL_SCANCODE_KP_PLUS,     /* NumPad + */
    Separator          = SDL_SCANCODE_KP_COMMA,    /* NumPad , */
    Subtract           = SDL_SCANCODE_KP_MINUS,    /* NumPad - */
    Decimal            = SDL_SCANCODE_KP_PERIOD,   /* NumPad . */
    Divide             = SDL_SCANCODE_KP_DIVIDE,   /* NumPad / */

    F1                 = SDL_SCANCODE_F1, /* F1 */
    F2                 = SDL_SCANCODE_F2, /* F2 */
    F3                 = SDL_SCANCODE_F3,  /* F3 */
    F4                 = SDL_SCANCODE_F4,  /* F4 */
    F5                 = SDL_SCANCODE_F5,  /* F5 */
    F6                 = SDL_SCANCODE_F6,  /* F6 */
    F7                 = SDL_SCANCODE_F7,  /* F7 */
    F8                 = SDL_SCANCODE_F8,  /* F8 */
    F9                 = SDL_SCANCODE_F9,  /* F9 */
    F10                = SDL_SCANCODE_F10, /* F10 */
    F11                = SDL_SCANCODE_F11, /* F11 */
    F12                = SDL_SCANCODE_F12, /* F12 */
    F13                = SDL_SCANCODE_F13, /* F13 */
    F14                = SDL_SCANCODE_F14, /* F14 */
    F15                = SDL_SCANCODE_F15, /* F15 */
    F16                = SDL_SCANCODE_F16, /* F16 */
    F17                = SDL_SCANCODE_F17, /* F17 */
    F18                = SDL_SCANCODE_F18, /* F18 */
    F19                = SDL_SCANCODE_F19, /* F19 */
    F20                = SDL_SCANCODE_F20, /* F20 */
    F21                = SDL_SCANCODE_F21, /* F21 */
    F22                = SDL_SCANCODE_F22, /* F22 */
    F23                = SDL_SCANCODE_F23, /* F23 */
    F24                = SDL_SCANCODE_F24, /* F24 */

    NumLock            = SDL_SCANCODE_NUMLOCKCLEAR, /* Num Lock */
    ScrollLock         = SDL_SCANCODE_SCROLLLOCK,   /* Scroll Lock */

    LShift             = SDL_SCANCODE_LSHIFT, /* Left Shift */
    RShift             = SDL_SCANCODE_RSHIFT, /* Right Shift */
    LControl           = SDL_SCANCODE_LCTRL,  /* Left Control */
    RControl           = SDL_SCANCODE_RCTRL,  /* Right Control */
    LAlt               = SDL_SCANCODE_LALT,   /* Left Alt */
    RAlt               = SDL_SCANCODE_RALT,   /* Right Alt */

    LeftBracket        = SDL_SCANCODE_LEFTBRACKET,  /* [ */
    Backslash          = SDL_SCANCODE_RIGHTBRACKET, /* \ */
    RightBracket       = SDL_SCANCODE_RIGHTBRACKET, /* ] */
};


using MouseCode = uint32_t;

enum class MouseCodes : MouseCode
{
    Left          = SDL_BUTTON_LEFT,
    Right         = SDL_BUTTON_RIGHT,
    Middle        = SDL_BUTTON_MIDDLE,
};

// clang-format on

DGEX_END
