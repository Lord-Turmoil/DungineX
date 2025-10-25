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
 *                    Last Update : October 25, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Base definition for properties.                                            *
 ******************************************************************************/

#include "DgeX/Application/UI/Style/WidgetProperty.h"

#include "DgeX/Utils/Macros.h"
#include "DgeX/Utils/Strings.h"

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

float WidgetProperties::GlobalX() const
{
    return OffsetX + X->Value();
}

float WidgetProperties::GlobalY() const
{
    return OffsetY + Y->Value();
}

IntegerProperty::IntegerProperty() : Value(0)
{
}

IntegerProperty::IntegerProperty(int value) : Value(value)
{
}

IntegerProperty::IntegerProperty(const char* value) : Value(0)
{
    static const std::regex INTEGER_PATTERN(R"(^\s*([+-]?\d+)\s*$)");

    DGEX_ASSERT(value, "Null value for IntegerProperty");

    std::string source(value);
    std::smatch matches;

    if (std::regex_match(source, matches, INTEGER_PATTERN))
    {
        int number;
        if (DGEX_SSCANF(matches[1].str().c_str(), "%d", &number) != 1)
        {
            DGEX_CORE_WARN("Invalid value '{}' for IntegerProperty, defaulting to 0", value);
            return;
        }
        Value = number;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for IntegerProperty, defaulting to 0", value);
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

StringProperty::StringProperty(std::string value) : Value(std::move(value))
{
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

DisplayProperty::DisplayProperty() : Value(DisplayValues::Block)
{
}

DisplayProperty::DisplayProperty(DisplayValues value) : Value(value)
{
}

DisplayProperty::DisplayProperty(const char* value) : Value(DisplayValues::Block)
{
    DGEX_ASSERT(value, "Null value for DisplayProperty");

    if (Strings::Equals(value, "none"))
    {
        Value = DisplayValues::None;
    }
    else if (Strings::Equals(value, "block"))
    {
        Value = DisplayValues::Block;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for DisplayProperty, defaulting to 'block'", value);
        Value = DisplayValues::Block;
    }
}

PositionProperty::PositionProperty() : Value(PositionValues::Auto)
{
}

PositionProperty::PositionProperty(PositionValues value) : Value(value)
{
}

PositionProperty::PositionProperty(const char* value)
{
    DGEX_ASSERT(value, "Null value for PositionProperty");

    if (Strings::Equals(value, "auto"))
    {
        Value = PositionValues::Auto;
    }
    else if (Strings::Equals(value, "relative"))
    {
        Value = PositionValues::Relative;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for PositionProperty, defaulting to 'auto'", value);
        Value = PositionValues::Auto;
    }
}

TextAlignProperty::TextAlignProperty() : Value(TextAlignValues::Left)
{
}

TextAlignProperty::TextAlignProperty(TextAlignValues value) : Value(value)
{
}

TextAlignProperty::TextAlignProperty(const char* value)
{
    DGEX_ASSERT(value, "Null value for TextAlignProperty");

    if (Strings::Equals(value, "left"))
    {
        Value = TextAlignValues::Left;
    }
    else if (Strings::Equals(value, "center"))
    {
        Value = TextAlignValues::Center;
    }
    else if (Strings::Equals(value, "right"))
    {
        Value = TextAlignValues::Right;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for TextAlignProperty, defaulting to 'left'", value);
        Value = TextAlignValues::Left;
    }
}

VerticalAlignProperty::VerticalAlignProperty() : Value(VerticalAlignValues::Top)
{
}

VerticalAlignProperty::VerticalAlignProperty(VerticalAlignValues value) : Value(value)
{
}

VerticalAlignProperty::VerticalAlignProperty(const char* value)
{
    DGEX_ASSERT(value, "Null value for VerticalAlignProperty");

    if (Strings::Equals(value, "top"))
    {
        Value = VerticalAlignValues::Top;
    }
    else if (Strings::Equals(value, "middle"))
    {
        Value = VerticalAlignValues::Middle;
    }
    else if (Strings::Equals(value, "bottom"))
    {
        Value = VerticalAlignValues::Bottom;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for VerticalAlignProperty, defaulting to 'top'", value);
        Value = VerticalAlignValues::Top;
    }
}

FontStyleProperty::FontStyleProperty() : Value(FontStyles::Regular)
{
}

FontStyleProperty::FontStyleProperty(FontStyles value) : Value(value)
{
}

FontStyleProperty::FontStyleProperty(const char* value)
{
    DGEX_ASSERT(value, "Null value for FontStyleProperty");

    if (Strings::Equals(value, "Regular"))
    {
        Value = FontStyles::Regular;
    }
    else if (Strings::Equals(value, "Bold"))
    {
        Value = FontStyles::Bold;
    }
    else if (Strings::Equals(value, "Italic"))
    {
        Value = FontStyles::Italic;
    }
    else if (Strings::Equals(value, "Bold Italic") || Strings::Equals(value, "BoldItalic"))
    {
        Value = FontStyles::BoldItalic;
    }
    else
    {
        DGEX_CORE_WARN("Invalid value '{}' for FontStyleProperty, defaulting to 'Regular'", value);
        Value = FontStyles::Regular;
    }
}

} // namespace UI

DGEX_END