#include "DgeX/Application/Style/Property/Property.h"

#include "DgeX/Core/Assert.h"
#include "DgeX/Utils/String.h"

DGEX_BEGIN

#pragma region SizeProperty

SizeProperty::SizeProperty(const Unit& width, const Unit& height) : _width(width), _height(height)
{
}

void SizeProperty::Load(const tinyxml2::XMLElement& element)
{
    if (const char* value = element.Attribute(XmlName()))
    {
        Apply(value);
    }
    else
    {
        const char* width = element.Attribute("width");
        const char* height = element.Attribute("height");
        if (width)
        {
            _width = Unit::FromString(width);
        }
        if (height)
        {
            _height = Unit::FromString(height);
        }
    }
}

bool SizeProperty::Apply(const char* value)
{
    DGEX_ASSERT(value);

    // Warning: Buffer overflow?
    char widthBuffer[16];
    char heightBuffer[16];

    if (sscanf(value, "%s %s", widthBuffer, heightBuffer) == 2)
    {
        SetWidth(Unit::FromString(widthBuffer));
        SetHeight(Unit::FromString(heightBuffer));
        return true;
    }

    DGEX_LOG_WARN("Failed to parse size property value: {0}", value);

    return false;
}

#pragma endregion

#pragma region PositionProperty

PositionProperty::PositionProperty(const Unit& x, const Unit& y) : _x(x), _y(y)
{
}

void PositionProperty::Load(const tinyxml2::XMLElement& element)
{
    if (const char* value = element.Attribute(XmlName()))
    {
        Apply(value);
    }
}

bool PositionProperty::Apply(const char* value)
{
    DGEX_ASSERT(value);

    // Warning: Buffer overflow?
    char xBuffer[16];
    char yBuffer[16];

    if (sscanf(value, "%s %s", xBuffer, yBuffer) == 2)
    {
        SetX(Unit::FromString(xBuffer));
        SetY(Unit::FromString(yBuffer));
        return true;
    }

    DGEX_LOG_WARN("Failed to parse position property value: {0}", value);

    return false;
}

#pragma endregion

#pragma region TranslateProperty

TranslateProperty::TranslateProperty(const Unit& x, const Unit& y) : _x(x), _y(y)
{
}

void TranslateProperty::Load(const tinyxml2::XMLElement& element)
{
    if (const char* value = element.Attribute(XmlName()))
    {
        Apply(value);
    }
}

bool TranslateProperty::Apply(const char* value)
{
    DGEX_ASSERT(value);

    // Warning: Buffer overflow?
    char xBuffer[16];
    char yBuffer[16];

    if (sscanf(value, "%s %s", xBuffer, yBuffer) == 2)
    {
        SetX(Unit::FromString(xBuffer));
        SetY(Unit::FromString(yBuffer));
        return true;
    }

    DGEX_LOG_WARN("Failed to parse translate property value: {0}", value);

    return false;
}

#pragma endregion

#pragma region ColorProperty

ColorProperty::ColorProperty(const Color& color) : _color(color)
{
}

void ColorProperty::Load(const tinyxml2::XMLElement& element)
{
    if (const char* value = element.Attribute(XmlName()))
    {
        Apply(value);
    }
}

bool ColorProperty::Apply(const char* value)
{
    DGEX_ASSERT(value);

    uint32_t r, g, b, a = 255;

    // clang-format off
    if ((sscanf(value, "#%02x%02x%02x%02x",    &a, &r, &g, &b) == 4) ||
        (sscanf(value, "#%02x%02x%02x",            &r, &g, &b) == 3) ||
        (sscanf(value, "rgba(%u, %u, %u, %u)", &r, &g, &b, &a) == 4) ||
        (sscanf(value, "rgb(%u, %u, %u)",      &r, &g, &b    ) == 3))
    {
        SetColor(Color::FromRGBA(
            static_cast<float>(r),
            static_cast<float>(g),
            static_cast<float>(b),
            static_cast<float>(a)));
        return true;
    }
    // clang-format on

    DGEX_LOG_WARN("Failed to parse color property value: {0}", value);

    return false;
}

#pragma endregion

#pragma region OpacityProperty

OpacityProperty::OpacityProperty() : _opacity(1.0f)
{
}

OpacityProperty::OpacityProperty(float opacity) : _opacity(opacity)
{
}

void OpacityProperty::Load(const tinyxml2::XMLElement& element)
{
    if (const char* value = element.Attribute(XmlName()))
    {
        Apply(value);
    }
}

bool OpacityProperty::Apply(const char* value)
{
    float opacity;

    if (sscanf(value, "%f", &opacity) == 1)
    {
        SetOpacity(opacity);
        return true;
    }

    DGEX_LOG_WARN("Failed to parse opacity property value: {0}", value);

    return false;
}

#pragma endregion

DGEX_END
