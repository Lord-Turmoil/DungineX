#pragma once

#include "DgeX/Application/Style/Selector.h"
#include "DgeX/Core/Macros.h"

#include <tinyxml2.h>

#include <string>
#include <unordered_map>

DGEX_BEGIN

class DomElement;

/**
 * @brief Style for a single element.
 * @paragraph
 * The style element is as follows.
 *
 * <Style id="style-id">
 *     <Property name="color" value="#000000" />
 * </Style>
 *
 * Value in property is stored in plain text, which is parsed by corresponding
 * element.
 */
class Style
{
public:
    Style() = default;
    Style(const char* selector);
    Style(const tinyxml2::XMLElement& element);

    Style(const Style&) = default;
    Style& operator=(const Style&) = default;
    Style(Style&& other) noexcept;
    Style& operator=(Style&& other) noexcept;

    static const char* XmlTag()
    {
        return "Style";
    }

    /**
     * @brief Load the style from an XML element.
     * @param element The XML element to load from.
     * @note This will replace all existing properties.
     */
    void Load(const tinyxml2::XMLElement& element);

    /**
     * @brief Clear all properties.
     */
    void Clear();

public:
    bool Match(const DomElement& element) const;

    bool HasProperty(const std::string& name) const;

    /**
     * @brief Get the property by name with default value.
     * @param name The name of the property.
     * @param defaultValue The default value to return if property not found.
     * @return The value of the property if found, otherwise the default value.
     */
    const char* GetProperty(const std::string& name, const char* defaultValue = nullptr);

    /**
     * @brief Set property value.
     * @note This will replace existing property with the same name.
     * @note Property with empty name will be ignored.
     */
    void SetProperty(const std::string& name, const std::string& value);

private:
    Selector _selector;
    std::unordered_map<std::string, std::string> _properties;
};

DGEX_END
