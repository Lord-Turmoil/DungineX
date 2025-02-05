#pragma once

#include "DgeX/Application/Style/Color.h"
#include "DgeX/Application/Style/Style.h"
#include "DgeX/Application/Style/Unit.h"
#include "DgeX/Core/Base.h"

#include <functional>
#include <tinyxml2.h>

DGEX_BEGIN

template <typename T> using PropertyChangeCallback = std::function<void(T&, const T&)>;

template <typename T> struct PropertyField
{
    T Value;
    PropertyChangeCallback<T> OnChange;

    PropertyField() = default;

    template <typename... Args> PropertyField(Args&&... args) : Value(std::forward<Args>(args)...)
    {
    }
};

#pragma region

// clang-format off

/**
 * These two macro provides the following field and functions:
 * - Property field  : private field with the given type.
 * - Getter          : getter function.
 * - Setter          : setter function, will invoke OnChange callback if present
 * - Silent setter   : setter function, does not invoke callback
 * - Callback setter : on change callback, nullptr by default
 */

#define DECL_FIELD(TYPE, NAME, FIELD) _DECL_FIELD_IMPL(TYPE, const TYPE&, _## FIELD, Get## NAME, Set## NAME, FIELD, On## NAME## Change)
#define DECL_PRIMITIVE_FIELD(TYPE, NAME, FIELD) _DECL_FIELD_IMPL(TYPE, TYPE, _## FIELD, Get## NAME, Set## NAME, FIELD, On## NAME## Change)
#define _DECL_FIELD_IMPL(TYPE, REF_TYPE, FIELD, GETTER, SETTER, PARAM, CALLBACK)                                        \
private:                                                                                                               \
    PropertyField<TYPE> FIELD;                                                                                         \
public:                                                                                                                \
    REF_TYPE GETTER() const { return (FIELD).Value; }                                                                  \
    void SETTER(REF_TYPE PARAM)                                                                                        \
    {                                                                                                                  \
        if ((FIELD).OnChange) { (FIELD).OnChange((FIELD).Value, (PARAM)); }                                            \
        else { (FIELD).Value = (PARAM); }                                                                              \
    }                                                                                                                  \
    void SETTER## Silent(REF_TYPE PARAM) { (FIELD).Value = (PARAM); }                                                   \
    void CALLBACK(PropertyChangeCallback<TYPE> callback) { (FIELD).OnChange = std::move(callback); }
// clang-format on

#pragma endregion

/**
 * @brief Base property class.
 */
class BaseProperty
{
public:
    virtual ~BaseProperty() = default;

    virtual const char* Name() const = 0;

    /**
     * @brief Load property directly from an XML element.
     * @param element XML element.
     */
    virtual void Load(const tinyxml2::XMLElement& element) = 0;

    /**
     * @brief Load property via style.
     * @param style The style of the widget.
     */
    virtual void Load(const Ref<Style>& style);

    /**
     * @brief Apply the property value.
     * @note The value is plain-text from the XML file.
     * @return True if the property is applied successfully, false otherwise.
     */
    virtual bool Apply(const char* value) = 0;
};

DGEX_END
