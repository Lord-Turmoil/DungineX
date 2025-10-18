/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Property.h                                *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : October 4, 2025                           *
 *                                                                            *
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base definition for properties.                                            *
 ******************************************************************************/

#include "DgeX/Application/UI/Style/WidgetProperty.h"

#include "DgeX/Utils/Macros.h"

#include <regex>

DGEX_BEGIN

namespace UI
{

void WidgetProperties::Update(DeltaTime delta) const
{
    X->Update(delta);
    Y->Update(delta);
    Width->Update(delta);
    Height->Update(delta);
    ForegroundColor->Update(delta);
    BackgroundColor->Update(delta);
    Opacity->Update(delta);
    Rotation->Update(delta);
    Scale->Update(delta);
    if (FontSize)
    {
        FontSize->Update(delta);
    }
}

NumberProperty::NumberProperty() : Value(0.0f)
{
}

NumberProperty::NumberProperty(float value) : Value(value)
{
}

NumberProperty::NumberProperty(const char* value) : Value(0.0f)
{
    static const std::regex NUMBER_PATTERN(R"(^\s*([+-]?\d*\.?\d*)\s*([%]?)\s*$)");

    DGEX_ASSERT(value, "Null value for NumberProperty");

    std::string source(value);
    std::smatch matches;

    if (std::regex_match(source, matches, NUMBER_PATTERN))
    {
        float number;
        if (DGEX_SSCANF(matches[1].str().c_str(), "%f", &number) != 1)
        {
            DGEX_CORE_WARN("Invalid value '{}' for NumberProperty, defaulting to 0", value);
            return;
        }
        if (matches[2].str() == "%")
        {
            number /= 100.0f;
        }
        Value = number;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for NumberProperty, defaulting to 0", value);
    }
}

MetricProperty::MetricProperty() : Value(0.0f), Unit(MetricUnit::Pixel)
{
}

MetricProperty::MetricProperty(float value, MetricUnit unit) : Value(value), Unit(unit)
{
}

// Possible units: px, %, s, ms
MetricProperty::MetricProperty(const char* value) : Value(0.0f), Unit(MetricUnit::Unspecified)
{
    static const std::regex METRIC_PATTERN(R"(^\s*([+-]?\d*\.?\d*)\s*([a-zA-Z%]*)\s*$)");

    DGEX_ASSERT(value, "Null value for MetricProperty");

    std::string source(value);
    std::smatch matches;

    if (std::regex_match(source, matches, METRIC_PATTERN))
    {
        float number;
        if (DGEX_SSCANF(matches[1].str().c_str(), "%f", &number) != 1)
        {
            DGEX_CORE_WARN("Invalid value '{}' for MetricProperty, defaulting to 0", value);
            return;
        }
        Value = number;
        Unit = MetricUnitFromString(matches[2].str());
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for MetricProperty, defaulting to 0", value);
    }
}

StringProperty::StringProperty(const char* value)
{
    DGEX_ASSERT(value, "Null value for StringProperty");

    Value = std::string(value);
}

ColorProperty::ColorProperty() : Value(Color::Black)
{
}

ColorProperty::ColorProperty(Color color) : Value(color)
{
}

ColorProperty::ColorProperty(const char* value) : Value(Color::Black)
{
    static const std::regex HEX_PATTERN(R"(^\s*#([0-9a-fA-F]{3}|[0-9a-fA-F]{4}|[0-9a-fA-F]{6}|[0-9a-fA-F]{8})\s*$)");
    static const std::regex RGB_PATTERN(R"(^\s*rgb\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\)\s*$)");
    static const std::regex RGBA_PATTERN(
        R"(^\s*rgba\(\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*,\s*(\d{1,3})\s*\)\s*$)");

    DGEX_ASSERT(value, "Null value for ColorProperty");

    std::string source(value);
    std::smatch matches;

    uint8_t r, g, b, a;
    if (std::regex_match(source, matches, HEX_PATTERN))
    {
        if (matches[1].str().length() == 3)
        {
            DGEX_SSCANF(matches[1].str().c_str(), "%1hhx%1hhx%1hhx", &r, &g, &b);
            Value = Color(static_cast<uint8_t>(r | (r << 4)), static_cast<uint8_t>(g | (g << 4)),
                          static_cast<uint8_t>(b | (b << 4)));
        }
        else if (matches[1].str().length() == 4)
        {
            DGEX_SSCANF(matches[1].str().c_str(), "%1hhx%1hhx%1hhx%1hhx", &a, &r, &g, &b);
            Value = Color(static_cast<uint8_t>(r | (r << 4)), static_cast<uint8_t>(g | (g << 4)),
                          static_cast<uint8_t>(b | (b << 4)), static_cast<uint8_t>(a | (a << 4)));
        }
        else if (matches[1].str().length() == 6)
        {
            DGEX_SSCANF(matches[1].str().c_str(), "%02hhx%02hhx%02hhx", &r, &g, &b);
            Value = Color(r, g, b);
        }
        else if (matches[1].str().length() == 8)
        {
            DGEX_SSCANF(matches[1].str().c_str(), "%02hhx%02hhx%02hhx%02hhx", &a, &r, &g, &b);
            Value = Color(r, g, b, a);
        }
    }
    else if (std::regex_match(source, matches, RGB_PATTERN))
    {
        DGEX_SSCANF(matches[1].str().c_str(), "%hhu", &r);
        DGEX_SSCANF(matches[2].str().c_str(), "%hhu", &g);
        DGEX_SSCANF(matches[3].str().c_str(), "%hhu", &b);
        Value = Color(r, g, b);
    }
    else if (std::regex_match(source, matches, RGBA_PATTERN))
    {
        DGEX_SSCANF(matches[1].str().c_str(), "%hhu", &r);
        DGEX_SSCANF(matches[2].str().c_str(), "%hhu", &g);
        DGEX_SSCANF(matches[3].str().c_str(), "%hhu", &b);
        DGEX_SSCANF(matches[4].str().c_str(), "%hhu", &a);
        Value = Color(r, g, b, a);
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for ColorProperty, defaulting to black", value);
    }
}

void InitWidgetProperties(WidgetProperties& props)
{
    props.ForceSetPosition("auto");

    props.ForceSetX(0.0f);
    props.ForceSetY(0.0f);
    props.ForceSetWidth(0.0f);
    props.ForceSetHeight(0.0f);

    props.ForceSetForegroundColor(Color::Black);
    props.ForceSetBackgroundColor(Color::White);

    props.ForceSetOpacity(0.0f);

    props.ForceSetTransitionTime(0.0f);
    props.ForceSetTransitionStyle("none");
}

} // namespace UI

DGEX_END
