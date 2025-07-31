/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : EventAdapter.h                            *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : August 15, 2025                           *
 *                                                                            *
 *                    Last Update : August 15, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Convert SDL event to DungineX event.                                       *
 ******************************************************************************/

#include "Application/Event/EventAdapter.h"

#include "DgeX/Application/Event/KeyEvents.h"
#include "DgeX/Application/Event/MouseEvents.h"
#include "DgeX/Application/Event/WindowEvents.h"
#include "DgeX/Utils/Assert.h"

DGEX_BEGIN

static Ref<KeyEvent> CreateKeyEvent(const SDL_KeyboardEvent* event);

static Ref<MouseMovedEvent> CreateMouseMovedEvent(const SDL_MouseMotionEvent* event);
static Ref<MouseScrolledEvent> CreateMouseScrolledEvent(const SDL_MouseWheelEvent* event);
static Ref<MouseButtonEvent> CreateMouseButtonEvent(const SDL_MouseButtonEvent* event);

// https://wiki.libsdl.org/SDL3/SDL_Event
Ref<Event> ConvertSdlEvent(const SDL_Event* event)
{
    switch (event->type)
    {
    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
        return CreateKeyEvent(&event->key);
    case SDL_EVENT_MOUSE_MOTION:
        return CreateMouseMovedEvent(&event->motion);
    case SDL_EVENT_MOUSE_WHEEL:
        return CreateMouseScrolledEvent(&event->wheel);
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        return CreateMouseButtonEvent(&event->button);
    case SDL_EVENT_QUIT:
        return CreateRef<WindowCloseEvent>();
    }

    // currently ignore other events
    DGEX_CORE_WARN("Unhandled SDL event type: {}", event->type);
    return nullptr;
}

// https://wiki.libsdl.org/SDL3/SDL_KeyboardEvent
Ref<KeyEvent> CreateKeyEvent(const SDL_KeyboardEvent* event)
{
    if (event->type == SDL_EVENT_KEY_DOWN)
    {
        return CreateRef<KeyPressedEvent>(event->key & (SDLK_SCANCODE_MASK - 1), event->repeat);
    }
    // else (event->type == SDL_EVENT_KEY_UP)
    return CreateRef<KeyReleasedEvent>(event->key & (SDLK_SCANCODE_MASK - 1));
}

// https://wiki.libsdl.org/SDL3/SDL_MouseMotionEvent
Ref<MouseMovedEvent> CreateMouseMovedEvent(const SDL_MouseMotionEvent* event)
{
    return CreateRef<MouseMovedEvent>(FPoint(event->x, event->y));
}

// https://wiki.libsdl.org/SDL3/SDL_MouseWheelEvent
Ref<MouseScrolledEvent> CreateMouseScrolledEvent(const SDL_MouseWheelEvent* event)
{
    return CreateRef<MouseScrolledEvent>(event->y);
}

// https://wiki.libsdl.org/SDL3/SDL_MouseButtonEvent
// https://wiki.libsdl.org/SDL3/SDL_MouseButtonFlags
Ref<MouseButtonEvent> CreateMouseButtonEvent(const SDL_MouseButtonEvent* event)
{
    static int SDL_BUTTON_ID_TO_MOUSE_CODE[6] = {
        0,
        L(MouseCodes::Left),   // SDL_BUTTON_LEFT
        L(MouseCodes::Middle), // SDL_BUTTON_MIDDLE
        L(MouseCodes::Right),  // SDL_BUTTON_RIGHT
        0,                     // SDL_BUTTON_X1
        0                      // SDL_BUTTON_X2
    };

    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN)
    {
        return CreateRef<MouseButtonPressedEvent>(SDL_BUTTON_ID_TO_MOUSE_CODE[event->button], event->down);
    }
    return CreateRef<MouseButtonReleasedEvent>(SDL_BUTTON_ID_TO_MOUSE_CODE[event->button]);
}

DGEX_END
