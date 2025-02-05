#pragma once

#include "DgeX/Application/Style/Property/BaseProperty.h"
#include "DgeX/Application/Style/Style.h"

#include <functional>
#include <string>
#include <unordered_map>

DGEX_BEGIN

/**
 * @brief A group of properties.
 */
class PropertyGroup
{
public:
    /**
     * @brief
     * If set, will be called before the new value of the property is assigned.
     * To accept the new value, simply assign the new value to the property.
     */
    using PropertyChangeCallback = std::function<void(Ref<BaseProperty>&, const Ref<BaseProperty>&)>;

    PropertyGroup() = default;
    ~PropertyGroup() = default;

    template <typename T> Ref<T> AddProperty();
    template <typename T> Ref<T> GetProperty() const;
    template <typename T> bool HasProperty() const;
    template <typename T> void RemoveProperty();

    /**
     * @brief Load properties directly from XML element.
     * @param element XML element of the widget.
     */
    void Load(const tinyxml2::XMLElement& element) const;

    /**
     * @brief Load properties in the given style.
     * @param style Style of the given element.
     */
    void Load(const Ref<Style>& style) const;

private:
    std::unordered_map<std::string, Ref<BaseProperty>> _properties;
};

template <typename T> Ref<T> PropertyGroup::AddProperty()
{
    auto property = CreateRef<T>();
    _properties[T::XmlName()] = property;
    return property;
}

template <typename T> Ref<T> PropertyGroup::GetProperty() const
{
    auto it = _properties.find(T::XmlName());
    if (it != _properties.end())
    {
        return std::static_pointer_cast<T>(it->second);
    }
    return nullptr;
}

template <typename T> bool PropertyGroup::HasProperty() const
{
    return _properties.find(T::XmlName()) != _properties.end();
}

template <typename T> void PropertyGroup::RemoveProperty()
{
    _properties.erase(T::XmlName());
}

DGEX_END
