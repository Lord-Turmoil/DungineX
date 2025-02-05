#pragma once

#include "DgeX/Application/Style/Property/BaseProperty.h"

DGEX_BEGIN

/**
 * @brief Size of an element.
 * @paragraph
 * The size property is a pair of width and height, each is a unit.
 */
class SizeProperty final : public BaseProperty
{
public:
    SizeProperty() = default;
    SizeProperty(const Unit& width, const Unit& height);

    ~SizeProperty() override = default;

    // clang-format off
    static const char* XmlName() { return "size"; }
    const char* Name() const override { return XmlName(); }

    // clang-format on

    void Load(const tinyxml2::XMLElement& element) override;
    bool Apply(const char* value) override;

    DECL_FIELD(Unit, Width, width)
    DECL_FIELD(Unit, Height, height)
};

/**
 * @brief Position of an element.
 * @paragraph
 * The position property is a pair of x and y, each is a unit.
 */
class PositionProperty final : public BaseProperty
{
public:
    PositionProperty() = default;
    PositionProperty(const Unit& x, const Unit& y);

    ~PositionProperty() override = default;

    // clang-format off
    static const char* XmlName() { return "position"; }
    const char* Name() const override { return XmlName(); }

    // clang-format on

    void Load(const tinyxml2::XMLElement& element) override;
    bool Apply(const char* value) override;

    DECL_FIELD(Unit, X, x)
    DECL_FIELD(Unit, Y, y)
};

/**
 * @brief Translate to the original position.
 */
class TranslateProperty final : public BaseProperty
{
public:
    TranslateProperty() = default;
    TranslateProperty(const Unit& x, const Unit& y);

    ~TranslateProperty() override = default;

    // clang-format off
    static const char* XmlName() { return "translate"; }
    const char* Name() const override { return XmlName(); }

    // clang-format on

    void Load(const tinyxml2::XMLElement& element) override;
    bool Apply(const char* value) override;

    DECL_FIELD(Unit, X, x)
    DECL_FIELD(Unit, Y, y)
};

/**
 * @brief Color property.
 * @paragraph
 * The value of color can be in the following formats:
 * - #RRGGBB           (Alpha value is 0xFF)
 * - #AARRGGBB
 * - rgb(r, g, b)      (Alpha value is 255)
 * - rgba(r, g, b, a)
 */
class ColorProperty final : public BaseProperty
{
public:
    ColorProperty() = default;
    ColorProperty(const Color& color);

    ~ColorProperty() override = default;

    // clang-format off
    static const char* XmlName() { return "color"; }
    const char* Name() const override { return XmlName(); }

    // clang-format on

    void Load(const tinyxml2::XMLElement& element) override;
    bool Apply(const char* value) override;

    DECL_FIELD(Color, Color, color)
};

/**
 * @brief Opacity in percentage.
 * @paragraph
 * The value of opacity is a percent, % can be omitted.
 * It is stored as a float value between 0.0 and 1.0.
 */
class OpacityProperty final : public BaseProperty
{
public:
    OpacityProperty();
    OpacityProperty(float opacity);

    ~OpacityProperty() override = default;

    // clang-format off
    static const char* XmlName() { return "opacity"; }
    const char* Name() const override { return XmlName(); }

    // clang-format on

    void Load(const tinyxml2::XMLElement& element) override;
    bool Apply(const char* value) override;

    DECL_PRIMITIVE_FIELD(float, Opacity, opacity)
};

DGEX_END
