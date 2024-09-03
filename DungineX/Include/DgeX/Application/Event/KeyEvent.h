#pragma once

#include "DgeX/Common/Base.h"
#include "DgeX/Application/Event/Event.h"
#include "DgeX/Application/Input/KeyCode.h"

DGEX_BEGIN

class KeyEvent : public Event
{
public:
    KeyCode GetKeyCode() const { return _keyCode; }

    DECL_EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

protected:
    explicit KeyEvent(const KeyCode code) : _keyCode(code)
    {
    }


    KeyCode _keyCode;
};


class KeyPressedEvent final : public KeyEvent
{
public:
    explicit KeyPressedEvent(const KeyCode code, const bool isRepeat = false)
        : KeyEvent(code), _isRepeat(isRepeat)
    {
    }


    bool IsRepeat() const { return _isRepeat; }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << _keyCode << L" (repeat = " << _isRepeat << L")";
        return ss.str();
    }


    DECL_EVENT_CLASS_TYPE(KeyPressed)

private:
    /**
     * @brief Whether the key is being held down or pressed for the first time.
     */
    bool _isRepeat;
};


class KeyReleasedEvent final : public KeyEvent
{
public:
    explicit KeyReleasedEvent(const KeyCode code) : KeyEvent(code)
    {
    }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << _keyCode;
        return ss.str();
    }


    DECL_EVENT_CLASS_TYPE(KeyReleased)
};


class KeyTypedEvent final : public KeyEvent
{
public:
    explicit KeyTypedEvent(const KeyCode code) : KeyEvent(code)
    {
    }


    std::wstring ToString() const override
    {
        std::wstringstream ss;
        ss << GetName() << L" Event: " << _keyCode;
        return ss.str();
    }


    DECL_EVENT_CLASS_TYPE(KeyTyped)
};


DGEX_END
