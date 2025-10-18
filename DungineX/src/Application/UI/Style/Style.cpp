/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : Style.cpp                                 *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : August 24, 2025                           *
 *                                                                            *
 *                    Last Update : October 18, 2025                          *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * Style for UI elements.                                                     *
 ******************************************************************************/

#include "DgeX/Application/UI/Style/Style.h"

#include "DgeX/Utils/Assert.h"
#include "DgeX/Utils/Macros.h"
#include "DgeX/Utils/UUID.h"

DGEX_BEGIN

namespace UI
{

BaseStyle::BaseStyle(std::string name) : _name(std::move(name))
{
}

BaseStyle::BaseStyle(Ext::XmlElement element)
{
    if (!element.IsValid())
    {
        DGEX_CORE_ERROR("Invalid XML element for BaseStyle");
        return;
    }

    if (const char* name = element.Attribute("name"))
    {
        _name = name;
    }
    else
    {
        DGEX_CORE_WARN("BaseStyle element missing 'name' attribute");
        _name = UUID().ToString();
    }

    Ext::XmlElement property = element.FirstChild("Property");
    while (property)
    {
        if (const char* name = property.Attribute("name"))
        {
            _properties[name] = property.Attribute("value", "");
        }
        else
        {
            DGEX_CORE_ERROR("Property element missing 'name' attribute in '{0}'", _name);
        }
        property = property.NextSibling("Property");
    }
}

BaseStyle::BaseStyle(const BaseStyle& other)
{
    _name = other._name;
    _properties = other._properties;
}

BaseStyle::BaseStyle(BaseStyle&& other) noexcept
{
    _name = std::move(other._name);
    _properties = std::move(other._properties);
}

BaseStyle& BaseStyle::operator=(const BaseStyle& other)
{
    if (this != &other)
    {
        _name = other._name;
        _properties = other._properties;
    }
    return *this;
}

BaseStyle& BaseStyle::operator=(BaseStyle&& other) noexcept
{
    if (this != &other)
    {
        _name = std::move(other._name);
        _properties = std::move(other._properties);
    }
    return *this;
}

void BaseStyle::_Merge(const Ref<BaseStyle>& style)
{
    if (!style)
    {
        return;
    }

    for (const auto& [key, value] : style->_properties)
    {
        _properties[key] = value;
    }
}

void BaseStyle::_Merge(Ext::XmlElement element)
{
    Ext::XmlAttribute attribute = element.FirstAttribute();
    while (attribute)
    {
        const char* name = attribute.Name(); // It can't be null, I think?
        const char* value = attribute.Value();
        if (!value)
        {
            DGEX_CORE_WARN("Attribute '{0}' in element '{1}' has no value, using empty string", name, element.Name());
            value = "";
        }
        _properties[name] = value;
        attribute = attribute.Next();
    }

    Ext::XmlElement property = element.FirstChild("Property");
    while (property)
    {
        if (const char* name = property.Attribute("name"))
        {
            _properties[name] = property.Attribute("value", "");
        }
        else
        {
            DGEX_CORE_ERROR("Property element missing 'name' attribute in '{0}'", element.Name());
        }
        property = property.NextSibling("Property");
    }
}

const std::string& BaseStyle::GetName() const
{
    return _name;
}

std::string BaseStyle::GetProperty(const std::string& name, const std::string& defaultValue)
{
    if (const auto it = _properties.find(name); it != _properties.end())
    {
        return it->second;
    }
    return defaultValue;
}

void BaseStyle::SetProperty(const std::string& name, const std::string& value)
{
    _properties[name] = value;
}

bool BaseStyle::HasProperty(const std::string& name)
{
    return _properties.find(name) != _properties.end();
}

void BaseStyle::_DumpProperties(tinyxml2::XMLPrinter& printer) const
{
    for (const auto& [propName, propValue] : _properties)
    {
        printer.OpenElement("Property");
        printer.PushAttribute("name", propName.c_str());
        printer.PushAttribute("value", propValue.c_str());
        printer.CloseElement();
    }
}

Style::Style(const std::string& name) : BaseStyle(name)
{
}

Style::Style(Ext::XmlElement element) : BaseStyle(element)
{
    if (!element.IsValid())
    {
        DGEX_CORE_ERROR("Invalid XML element for Style");
        return;
    }

    Ext::XmlElement stateElement = element.FirstChild("State");
    while (stateElement)
    {
        Ref<BaseStyle> style = CreateRef<BaseStyle>(stateElement);
        StyleState state = StyleStateFromString(style->GetName());
        if (state == StyleState::Unknown)
        {
            DGEX_CORE_WARN("State '{0}' is not a recognized state in '{1}', ignored", style->GetName(), element.Name());
        }
        else
        {
            _states[L(state)] = style;
        }
        stateElement = stateElement.NextSibling("State");
    }
}

Style::Style(const Style& other) : BaseStyle(other)
{
    _name = other._name;
    for (int i = 0; i < L(StyleState::NumStates); i++)
    {
        if (other._states[i])
        {
            _states[i] = CreateRef<BaseStyle>(*other._states[i]);
        }
    }
}

Style::Style(Style&& other) noexcept : BaseStyle(std::move(other))
{
    for (int i = 0; i < L(StyleState::NumStates); i++)
    {
        // OK to ignore use after move here.
        _states[i] = std::move(other._states[i]);
    }
}

Style& Style::operator=(const Style& other)
{
    if (this != &other)
    {
        BaseStyle::operator=(other);
        for (int i = 0; i < L(StyleState::NumStates); i++)
        {
            if (other._states[i])
            {
                _states[i] = CreateRef<BaseStyle>(*other._states[i]);
            }
            else
            {
                _states[i] = nullptr;
            }
        }
    }
    return *this;
}

Style& Style::operator=(Style&& other) noexcept
{
    if (this != &other)
    {
        BaseStyle::operator=(std::move(other));
        for (int i = 0; i < L(StyleState::NumStates); i++)
        {
            // OK to ignore use after move here.
            _states[i] = std::move(other._states[i]);
        }
    }
    return *this;
}

void Style::Merge(const Ref<Style>& style)
{
    if (!style)
    {
        return;
    }

    _Merge(style);
    for (int i = 0; i < L(StyleState::NumStates); i++)
    {
        if (style->_states[i])
        {
            if (_states[i])
            {
                _states[i]->_Merge(style->_states[i]);
            }
            else
            {
                _states[i] = CreateRef<BaseStyle>(*style->_states[i]);
            }
        }
    }
}

void Style::Merge(Ext::XmlElement element)
{
    _Merge(element);

    Ext::XmlElement stateElement = element.FirstChild("State");
    while (stateElement)
    {
        Ref<BaseStyle> style = CreateRef<BaseStyle>(stateElement);
        if (StyleState state = StyleStateFromString(style->GetName()); state != StyleState::Unknown)
        {
            if (_states[L(state)])
            {
                _states[L(state)]->_Merge(style);
            }
            else
            {
                _states[L(state)] = CreateRef<BaseStyle>(*style);
            }
        }
        else
        {
            DGEX_CORE_WARN("State '{0}' is not a recognized state in '{1}', ignored", style->GetName(), element.Name());
        }

        stateElement = stateElement.NextSibling("State");
    }
}

std::string Style::GetStateProperty(StyleState state, const std::string& name, const std::string& defaultValue)
{
    if (_states[L(state)])
    {
        return _states[L(state)]->GetProperty(name, defaultValue);
    }
    return GetProperty(name, defaultValue);
}

void Style::SetStateProperty(StyleState state, const std::string& name, const std::string& value)
{
    if (!_states[L(state)])
    {
        _states[L(state)] = CreateRef<BaseStyle>(ToString(state));
    }
    _states[L(state)]->SetProperty(name, value);
}

bool Style::HasState(StyleState state) const
{
    return _states[L(state)] != nullptr;
}

bool Style::HasStateProperty(StyleState state, const std::string& name)
{
    if (_states[L(state)])
    {
        return _states[L(state)]->HasProperty(name);
    }
    return false;
}

void Style::Dump(const Ref<Files::OutputHandle>& handle) const
{
    DGEX_ASSERT(handle && handle->IsOpen(), "Output handle must be valid and open");

    tinyxml2::XMLPrinter printer;
    _Dump(printer);
    handle->WriteLine(printer.CStr());
}

void Style::_Dump(tinyxml2::XMLPrinter& printer) const
{
    printer.OpenElement("Style");
    printer.PushAttribute("name", GetName().c_str());
    _DumpProperties(printer);
    for (const auto& _state : _states)
    {
        if (_state)
        {
            printer.OpenElement("State");
            printer.PushAttribute("name", _state->GetName().c_str());
            _state->_DumpProperties(printer);
            printer.CloseElement();
        }
    }
    printer.CloseElement();
}

} // namespace UI

DGEX_END
