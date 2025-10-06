/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Style.h                                   *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : August 24, 2025                           *
 *                                                                            *
 *                    Last Update : October 4, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Style for UI elements.                                                     *
 ******************************************************************************/

#pragma once

#include "DgeX/Defines.h"
#include "DgeX/Extension/XmlDocument.h"
#include "DgeX/Utils/Files.h"
#include "DgeX/Utils/Types.h"

#include <map>
#include <string>

DGEX_BEGIN

namespace UI
{

/**
 * @brief Base style is simply a collection of properties.
 */
class BaseStyle
{
    friend class Style;

public:
    BaseStyle() = default;
    BaseStyle(const BaseStyle& other);
    BaseStyle(BaseStyle&& other) noexcept;
    BaseStyle& operator=(const BaseStyle& other);
    BaseStyle& operator=(BaseStyle&& other) noexcept;

    /**
     * @brief Create an empty style with a name.
     *
     * @param name Name of the style.
     */
    explicit BaseStyle(std::string name);

    /**
     * @brief Create a style from XML element.
     *
     * This requires the element to have a "name" attribute, and may contain <Property>
     * child elements. It will not include other attributes on the element into the
     * properties.
     *
     * @param element XML element.
     */
    explicit BaseStyle(Ext::XmlElement element);

    virtual ~BaseStyle() = default;

    /**
     * @brief Get the name of the style.
     *
     * @return The name of the style.
     */
    DGEX_API const std::string& GetName() const;

    /**
     * @brief Check if the style is valid.
     *
     * A valid style must have a non-empty name.
     *
     * @return Whether the style is valid.
     */
    DGEX_API bool IsValid() const;

    /**
     * @brief Get the property by name with default value.
     *
     * @param name The name of the property.
     * @param defaultValue The default value to return if property not found.
     * @return The value of the property if found, otherwise the default value.
     */
    DGEX_API std::string GetProperty(const std::string& name, const std::string& defaultValue = "");

    /**
     * @brief Get the property of the given type.
     *
     * It requires T to have a default constructor and a constructor that accepts const char*.
     */
    template <typename T>
    DGEX_API T GetPropertyAs(const std::string& name, const T& defaultValue = T()) const;

    /**
     * @brief Set property value.
     *
     * @param name The name of the property.
     * @param value The value of the property.
     */
    DGEX_API void SetProperty(const std::string& name, const std::string& value);

    /**
     * @brief Check if the property exists.
     *
     * @param name The name of the property.
     * @return Whether the property exists.
     */
    DGEX_API bool HasProperty(const std::string& name);

protected:
    /**
     * @brief Merge another style into this one.
     *
     * Properties in the other style will overwrite those in this style if they
     * share the same name.
     *
     * @param style The other style to merge from.
     */
    void _Merge(const Ref<BaseStyle>& style);

    /**
     * @brief Merge styles from an XML element.
     *
     * This is used to override existing properties with the attributes from a
     * widget XML element.
     *
     * Unlike the constructor, it does not require a "name" attribute, and will
     * take all attributes as properties.
     *
     * @param element XML element containing styles.
     */
    void _Merge(Ext::XmlElement element);

    /**
     * @brief Dump the properties to an XML printer.
     *
     * @param printer XML printer.
     */
    void _DumpProperties(tinyxml2::XMLPrinter& printer) const;

private:
    std::string _name;
    std::map<std::string, std::string> _properties;
};

template <typename T>
T BaseStyle::GetPropertyAs(const std::string& name, const T& defaultValue) const
{
    if (const auto it = _properties.find(name); it != _properties.end())
    {
        return T(it->second.c_str());
    }
    return defaultValue;
}

/**
 * @brief Primary style class that supports multiple states.
 *
 *
 */
class Style : public BaseStyle
{
public:
    Style(const Style& other);
    Style(Style&& other) noexcept;
    Style& operator=(const Style& other);
    Style& operator=(Style&& other) noexcept;

    /**
     * @brief Create an empty style with a name.
     *
     * @param name Name of the style
     */
    Style(const std::string& name);

    /**
     * @brief Create a style from an XML element.
     *
     * @param element An <Style></Style> element.
     */
    explicit Style(Ext::XmlElement element);

    /**
     * @brief Merge another style into this one.
     *
     * Properties and states in the other style will overwrite those in this
     * style if they share the same name.
     *
     * @param style The other style to merge from.
     */
    DGEX_API void Merge(const Ref<Style>& style);

    /**
     * @brief Merge styles from an XML element.
     *
     * @param element The XML element to merge.
     */
    DGEX_API void Merge(Ext::XmlElement element);

    /**
     * @brief Get the property of a specific state by name with default value.
     *
     * If the property is not found in the specified state, it will fall back to
     * the base properties.
     *
     * @param state The name of the state.
     * @param name The name of the property.
     * @param defaultValue The default value to return if property not found.
     * @return The value of the property.
     */
    DGEX_API std::string GetStateProperty(const std::string& state, const std::string& name,
                                          const std::string& defaultValue = "");

    /**
     * @brief Get the property of a specific state with type conversion.
     *
     * If the property is not found in the specified state, it will fall back to
     * the base properties.
     */
    template <typename T>
    DGEX_API T GetStatePropertyAs(const std::string& state, const std::string& name, const T& defaultValue = T()) const;

    /**
     * @brief Set value for a property in a specific state.
     *
     * @note If the state does not exist, it will be created.
     *
     * @param state The name of the state.
     * @param name The name of the property.
     * @param value The value to set.
     */
    DGEX_API void SetStateProperty(const std::string& state, const std::string& name, const std::string& value);

    /**
     * @brief Check if a state exists.
     *
     * @param state The name of the state.
     * @return Whether the state exists.
     */
    DGEX_API bool HasState(const std::string& state);

public:
    /**
     * @brief Dump the complete formatted style to an output handle.
     *
     * @param handle Output handle.
     */
    DGEX_API void Dump(const Ref<Files::OutputHandle>& handle) const;

private:
    /**
     * @brief Dump the complete formatted style to an XML printer.
     *
     * @param printer XML printer.
     */
    void _Dump(tinyxml2::XMLPrinter& printer) const;

private:
    std::map<std::string, Ref<BaseStyle>> _states;
};

template <typename T>
T Style::GetStatePropertyAs(const std::string& state, const std::string& name, const T& defaultValue) const
{
    if (auto it = _states.find(state); it != _states.end())
    {
        if (it->second->HasProperty(name))
        {
            return T(it->second->GetProperty(name, "").c_str());
        }
    }
    return GetPropertyAs<T>(name, defaultValue);
}

} // namespace UI

DGEX_END
