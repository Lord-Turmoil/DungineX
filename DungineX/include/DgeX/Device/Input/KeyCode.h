/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : KeyCode.h                                 *
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

#include <cstdint>

DGEX_BEGIN

// clang-format off

using KeyCodeFlags = uint32_t;

enum class KeyCode : KeyCodeFlags
{
    Backspace           = 0x08, /* Backspace */
    Tab                 = 0x09, /* Tab */

    Clear               = 0x0C, /* Clear */
    Return              = 0x0D, /* Enter */

    Shift               = 0x10, /* Shift */
    Control             = 0x11, /* Control */
    Alt                 = 0x12, /* Alt */
    Pause               = 0x13, /* Pause */
    CapsLock            = 0x14, /* Caps Lock */

    Escape              = 0x1B, /* Escape */

    Space               = 0x20, /* Space */
    PageUp              = 0x21, /* Page Up */
    PageDown            = 0x22, /* Page Down */
    End                 = 0x23, /* End */
    Home                = 0x24, /* Home */
    Left                = 0x25, /* Left Arrow */
    Up                  = 0x26, /* Up Arrow */
    Right               = 0x27, /* Right Arrow */
    Down                = 0x28, /* Down Arrow */
    Select              = 0x29, /* Select */
    Print               = 0x2A, /* Print */
    Execute             = 0x2B, /* Execute */
    Insert              = 0x2D, /* Insert */
    Delete              = 0x2E, /* Delete */
    Help                = 0x2F, /* Help */

    D0                  = 0x30, /* 0 */
    D1                  = 0x31, /* 1 */
    D2                  = 0x32, /* 2 */
    D3                  = 0x33, /* 3 */
    D4                  = 0x34, /* 4 */
    D5                  = 0x35, /* 5 */
    D6                  = 0x36, /* 6 */
    D7                  = 0x37, /* 7 */
    D8                  = 0x38, /* 8 */
    D9                  = 0x39, /* 9 */

    Semicolon           = 0x3B, /* ; */
    Equal               = 0x3D, /* = */

    A                   = 0x41, /* A */
    B                   = 0x42, /* B */
    C                   = 0x43, /* C */
    D                   = 0x44, /* D */
    E                   = 0x45, /* E */
    F                   = 0x46, /* F */
    G                   = 0x47, /* G */
    H                   = 0x48, /* H */
    I                   = 0x49, /* I */
    J                   = 0x4A, /* J */
    K                   = 0x4B, /* K */
    L                   = 0x4C, /* L */
    M                   = 0x4D, /* M */
    N                   = 0x4E, /* N */
    O                   = 0x4F, /* O */
    P                   = 0x50, /* P */
    Q                   = 0x51, /* Q */
    R                   = 0x52, /* R */
    S                   = 0x53, /* S */
    T                   = 0x54, /* T */
    U                   = 0x55, /* U */
    V                   = 0x56, /* V */
    W                   = 0x57, /* W */
    X                   = 0x58, /* X */
    Y                   = 0x59, /* Y */
    Z                   = 0x5A, /* Z */

    Numpad0            = 0x60, /* NumPad 0 */
    Numpad1            = 0x61, /* NumPad 1 */
    Numpad2            = 0x62, /* NumPad 2 */
    Numpad3            = 0x63, /* NumPad 3 */
    Numpad4            = 0x64, /* NumPad 4 */
    Numpad5            = 0x65, /* NumPad 5 */
    Numpad6            = 0x66, /* NumPad 6 */
    Numpad7            = 0x67, /* NumPad 7 */
    Numpad8            = 0x68, /* NumPad 8 */
    Numpad9            = 0x69, /* NumPad 9 */

    Multiply           = 0x6A, /* NumPad * */
    Add                = 0x6B, /* NumPad + */
    Separator          = 0x6C, /* NumPad , */
    Subtract           = 0x6D, /* NumPad - */
    Decimal            = 0x6E, /* NumPad . */
    Divide             = 0x6F, /* NumPad / */

    F1                 = 0x70, /* F1 */
    F2                 = 0x71, /* F2 */
    F3                 = 0x72, /* F3 */
    F4                 = 0x73, /* F4 */
    F5                 = 0x74, /* F5 */
    F6                 = 0x75, /* F6 */
    F7                 = 0x76, /* F7 */
    F8                 = 0x77, /* F8 */
    F9                 = 0x78, /* F9 */
    F10                = 0x79, /* F10 */
    F11                = 0x7A, /* F11 */
    F12                = 0x7B, /* F12 */
    F13                = 0x7C, /* F13 */
    F14                = 0x7D, /* F14 */
    F15                = 0x7E, /* F15 */
    F16                = 0x7F, /* F16 */
    F17                = 0x80, /* F17 */
    F18                = 0x81, /* F18 */
    F19                = 0x82, /* F19 */
    F20                = 0x83, /* F20 */
    F21                = 0x84, /* F21 */
    F22                = 0x85, /* F22 */
    F23                = 0x86, /* F23 */
    F24                = 0x87, /* F24 */

    NumLock            = 0x90, /* Num Lock */
    ScrollLock         = 0x91, /* Scroll Lock */

    LShift             = 0xA0, /* Left Shift */
    RShift             = 0xA1, /* Right Shift */
    LControl           = 0xA2, /* Left Control */
    RControl           = 0xA3, /* Right Control */
    LAlt               = 0xA4, /* Left Alt */
    RAlt               = 0xA5, /* Right Alt */

    LeftBracket        = 0xDB, /* [ */
    Backslash          = 0xDC, /* \ */
    RightBracket       = 0xDD, /* ] */
};

using MouseCodeFlags = uint32_t;

enum class MouseCode : MouseCodeFlags
{
    Button0             = 0,
    Button1             = 1,
    Button2             = 2,
    Button3             = 3,
    Button4             = 4,
    Button5             = 5,
    Button6             = 6,
    Button7             = 7,

    Left          = Button1,
    Right         = Button2,
    Middle        = Button4
};

// clang-format on

DGEX_END
