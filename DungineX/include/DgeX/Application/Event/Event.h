/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Event.h                                   *
 *                                                                            *
 *                     Programmer : Tony S.                                   *
 *                                                                            *
 *                     Start Date : August 3, 2025                            *
 *                                                                            *
 *                    Last Update : August 15, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base event type.                                                           *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/Event/EventTypes.h"
#include "DgeX/Defines.h"
#include "DgeX/Utils/Types.h"

#include <string>

DGEX_BEGIN

/**
 * @brief Base class for all events in the application.
 */
class Event
{
public:
    Event();
    virtual ~Event() = default;

    /**
     * @brief Get the name of the event.
     *
     * @return Name of the event.
     */
    DGEX_API virtual const char* GetName() const = 0;

    /**
     * @brief Get the type of the event.
     *
     * @return The type of the event.
     */
    DGEX_API virtual EventType GetType() const = 0;

    /**
     * @brief Get the string representation of the event.
     *
     * @return The event as a string.
     */
    DGEX_API virtual std::string ToString() const;

    /**
     * @brief Check if the event is handled.
     *
     * @return Whether the event is handled or not.
     */
    DGEX_API bool IsHandled() const;

    /**
     * @brief Set the handled state of the event.
     *
     * @param handled Whether the event is handled.
     */
    DGEX_API void SetHandled(bool handled);

private:
    bool _handled;
};

/**
 * @brief Declare member functions for event type and name.
 *
 * @param type type of the event.
 */
#define DECL_EVENT_CLASS_TYPE(type)                                                                                    \
    static EventType GetStaticType()                                                                                   \
    {                                                                                                                  \
        return EventType::type;                                                                                        \
    }                                                                                                                  \
    virtual const char* GetName() const override                                                                       \
    {                                                                                                                  \
        return #type;                                                                                                  \
    }                                                                                                                  \
    virtual EventType GetType() const override                                                                         \
    {                                                                                                                  \
        return GetStaticType();                                                                                        \
    }

/**
 * @brief Dispatch an event to a handling function.
 *
 * This will check if the event type matches the type of the handling function.
 * If it matches, the handling function will be called with the event.
 *
 * @tparam T Type of the event.
 * @tparam F Type of the handling function, which should be bool(const T&).
 * @param event Event to dispatch.
 * @param func Handling function.
 * @return Whether the event is dispatched or not.
 */
template <typename T, typename F> bool DispatchEvent(const Ref<Event>& event, const F& func)
{
    if (event->GetType() == T::GetStaticType())
    {
        if (func(static_cast<T&>(*event)))
        {
            event->SetHandled(true);
        }
        return true;
    }
    return false;
}

DGEX_END
