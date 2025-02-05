#include "DgeX/Application/Style/Style.h"

#include "DgeX/Core/Assert.h"
#include "DgeX/Core/UUID.h"
#include "DgeX/Utils/String.h"
#include "DgeX/Utils/Xml.h"

DGEX_BEGIN

Style::Style(const char* selector) : _selector(selector)
{
}

Style::Style(const tinyxml2::XMLElement& element)
{
    Load(element);
}

Style::Style(Style&& other) noexcept
{
    if (this != &other)
    {
        _properties = std::move(other._properties);
    }
}

Style& Style::operator=(Style&& other) noexcept
{
    if (this != &other)
    {
        _properties = std::move(other._properties);
    }
    return *this;
}

void Style::Load(const tinyxml2::XMLElement& element)
{
    DGEX_ASSERT(Utils::String::Equals(element.Name(), XmlTag()));

    Utils::Xml::XmlElementAdapter adapter(&element);

    _selector.Assign(adapter.Attribute("selector", ""));

    auto property = element.FirstChildElement("Property");
    while (property)
    {
        adapter.Attach(property);
        SetProperty(adapter.Attribute("name", ""), adapter.Attribute("value", ""));
        property = property->NextSiblingElement("Property");
    }
}

void Style::Clear()
{
    _properties.clear();
}

bool Style::Match(const DomElement& element) const
{
    return _selector.Match(element);
}

bool Style::HasProperty(const std::string& name) const
{
    return _properties.find(name) != _properties.end();
}

const char* Style::GetProperty(const std::string& name, const char* defaultValue)
{
    auto it = _properties.find(name);
    if (it == _properties.end())
    {
        return defaultValue;
    }
    return it->second.c_str();
}

void Style::SetProperty(const std::string& name, const std::string& value)
{
    if (!name.empty())
    {
        _properties[name] = value;
    }
}

DGEX_END
