/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : WidgetProperty.h                          *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 5, 2025                           *
 *                                                                            *
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Property required for widgets.                                             *
 ******************************************************************************/

#pragma once

#include "DgeX/Application/UI/Style/Property.h"
#include "DgeX/Renderer/Color.h"

DGEX_BEGIN

#define SET_PROPERTY_WITH_TRANSITION_IMPL(PROP, VALUE, TYPE)                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if (TransitionStyle->Value() != "none")                                                                        \
        {                                                                                                              \
            PROP = CreateRef<Property<TYPE>>(VALUE);                                                                   \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            PROP = CreateRef<DynamicProperty<TYPE>>(                                                                   \
                PROP->Value(), VALUE, TransitionTime->Value(),                                                         \
                Utils::Easing::StyleToEasingFn<timestamp_t>(TransitionStyle->Value()));                                \
        }                                                                                                              \
    } while (0)

#define SET_PROPERTY_WITHOUT_TRANSITION_IMPL(PROP, VALUE, TYPE)                                                        \
    do                                                                                                                 \
    {                                                                                                                  \
        PROP = CreateRef<Property<TYPE>>(VALUE);                                                                       \
    } while (0)

#define DEFINE_PROPERTY(PROP, TYPE)                                                                                    \
    Ref<Property<TYPE>> PROP;                                                                                          \
    void Set##PROP(PropertyTypeTrait<TYPE>::reference_type value)                                                      \
    {                                                                                                                  \
        SET_PROPERTY_WITH_TRANSITION_IMPL(PROP, value, TYPE);                                                          \
    }                                                                                                                  \
    void ForceSet##PROP(PropertyTypeTrait<TYPE>::reference_type value)                                                 \
    {                                                                                                                  \
        SET_PROPERTY_WITHOUT_TRANSITION_IMPL(PROP, value, TYPE);                                                       \
    }

#define DEFINE_NULLABLE_PROPERTY(PROP, TYPE)                                                                           \
    DEFINE_PROPERTY(PROP, TYPE)                                                                                        \
    void UnSet##PROP()                                                                                                 \
    {                                                                                                                  \
        PROP = nullptr;                                                                                                \
    }

#define DEFINE_NON_TRANSITIONABLE_PROPERTY(PROP, TYPE)                                                                 \
    Ref<Property<TYPE>> PROP;                                                                                          \
    void Set##PROP(PropertyTypeTrait<TYPE>::reference_type value)                                                      \
    {                                                                                                                  \
        SET_PROPERTY_WITHOUT_TRANSITION_IMPL(PROP, value, TYPE);                                                       \
    }                                                                                                                  \
    void ForceSet##PROP(PropertyTypeTrait<TYPE>::reference_type value)                                                 \
    {                                                                                                                  \
        SET_PROPERTY_WITHOUT_TRANSITION_IMPL(PROP, value, TYPE);                                                       \
    }

#define DEFINE_NULLABLE_NON_TRANSITIONABLE_PROPERTY(PROP, TYPE)                                                        \
    DEFINE_NON_TRANSITIONABLE_PROPERTY(PROP, TYPE)                                                                     \
    void UnSet##PROP()                                                                                                 \
    {                                                                                                                  \
        PROP = nullptr;                                                                                                \
    }

namespace UI
{

/**
 * @brief Properties of a widget for layout and rendering.
 *
 * It is recommended to use setters to modify the properties so that it can automatically
 * apply transition if needed. Also, can use ForceSet to set the property without transition.
 */
struct WidgetProperties
{
    DEFINE_NON_TRANSITIONABLE_PROPERTY(Position, std::string) // position: auto, relative

    DEFINE_PROPERTY(X, float)      // x: px, % (relative to parent)
    DEFINE_PROPERTY(Y, float)      // y: px, % (relative to parent)
    DEFINE_PROPERTY(Width, float)  // width: px, %
    DEFINE_PROPERTY(Height, float) // height: px, %

    DEFINE_NULLABLE_PROPERTY(ForegroundColor, Color) // color: hex, rgb, rgba
    DEFINE_PROPERTY(BackgroundColor, Color)          // background-color: hex, rgb, rgba

    DEFINE_PROPERTY(Opacity, float)  // opacity: 0.0 - 1.0, 0% - 100%
    DEFINE_PROPERTY(Rotation, float) // rotation: degrees
    DEFINE_PROPERTY(Scale, float)    // scale: 1.0 = 100%

    DEFINE_NULLABLE_PROPERTY(FontSize, float)                           // font-size: px (inherited)
    DEFINE_NULLABLE_NON_TRANSITIONABLE_PROPERTY(Font, std::string)      // font-family (inherited)
    DEFINE_NULLABLE_NON_TRANSITIONABLE_PROPERTY(FontStyle, std::string) // font-style (inherited)

    DEFINE_NON_TRANSITIONABLE_PROPERTY(TextAlign, std::string)     // text-align: left, center, right
    DEFINE_NON_TRANSITIONABLE_PROPERTY(VerticalAlign, std::string) // vertical-align: top, middle, bottom

    DEFINE_NON_TRANSITIONABLE_PROPERTY(TransitionTime, float)        // transition-time: e.g. 1s, 1000ms
    DEFINE_NON_TRANSITIONABLE_PROPERTY(TransitionStyle, std::string) // transition-style: see `StyleToEasingFn`

    void Update(DeltaTime delta) const;
};

enum class MetricUnit
{
    Unspecified,
    Pixel,
    Percent,
    Second,
    Millisecond
};

inline std::string ToString(MetricUnit unit)
{
    switch (unit)
    {
    case MetricUnit::Unspecified:
        return "";
    case MetricUnit::Pixel:
        return "px";
    case MetricUnit::Percent:
        return "%";
    case MetricUnit::Second:
        return "s";
    case MetricUnit::Millisecond:
        return "ms";
    }

    return "";
}

inline MetricUnit MetricUnitFromString(const std::string& value)
{
    if (value == "px")
    {
        return MetricUnit::Pixel;
    }
    if (value == "%")
    {
        return MetricUnit::Percent;
    }
    if (value == "s")
    {
        return MetricUnit::Second;
    }
    if (value == "ms")
    {
        return MetricUnit::Millisecond;
    }
    return MetricUnit::Unspecified;
}

/**
 * @brief A self-determined number property.
 *
 * Can be .3, 1.2, 30%, etc.
 *
 * Used for Opacity, etc.
 */
struct NumberProperty
{
    NumberProperty();
    explicit NumberProperty(float value);
    explicit NumberProperty(const char* value);

    float Value;
};

/**
 * @brief A metric property with unit.
 *
 * If with percent, it is relative to the parent widget's corresponding property.
 * Can be 30px, 50%, etc.
 *
 * Used for X, Y, Width, Height, etc.
 */
struct MetricProperty
{
    MetricProperty();
    explicit MetricProperty(float value, MetricUnit unit = MetricUnit::Unspecified);
    explicit MetricProperty(const char* value);

    float Value;
    MetricUnit Unit;
};

struct StringProperty
{
    StringProperty() = default;
    explicit StringProperty(std::string value);
    explicit StringProperty(const char* value);

    std::string Value;
};

/**
 * @brief Wrapper for color property.
 */
struct ColorProperty
{
    ColorProperty();
    explicit ColorProperty(Color color);
    explicit ColorProperty(const char* value);

    Color Value;
};

} // namespace UI

DGEX_END
