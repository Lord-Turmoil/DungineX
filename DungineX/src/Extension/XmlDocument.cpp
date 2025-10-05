/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : XmlDocument.cpp                           *
 *                                                                            *
 *                     Programmer : Tony Lewis                                *
 *                                                                            *
 *                     Start Date : August 24, 2025                           *
 *                                                                            *
 *                    Last Update : August 24, 2025                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * OVERVIEW:                                                                  *
 *                                                                            *
 * A wrapper around tinyxml2.                                                 *
 ******************************************************************************/

#include "DgeX/Extension/XmlDocument.h"

DGEX_BEGIN

namespace Ext
{

XmlAttribute::XmlAttribute(const tinyxml2::XMLAttribute* attribute) : _impl(attribute)
{
}

const char* XmlAttribute::Name() const
{
    return _impl ? _impl->Name() : nullptr;
}

const char* XmlAttribute::Value(const char* defaultValue) const
{
    return _impl ? _impl->Value() : defaultValue;
}

int XmlAttribute::IntValue(int defaultValue) const
{
    return _impl ? _impl->IntValue() : defaultValue;
}

float XmlAttribute::FloatValue(int defaultValue) const
{
    return _impl ? _impl->FloatValue() : defaultValue;
}

bool XmlAttribute::BoolValue(int defaultValue) const
{
    return _impl ? _impl->BoolValue() : defaultValue;
}

XmlAttribute XmlAttribute::Next() const
{
    return _impl ? XmlAttribute(_impl->Next()) : XmlAttribute(nullptr);
}

bool XmlAttribute::IsValid() const
{
    return _impl != nullptr;
}

bool XmlAttribute::operator==(const XmlAttribute& other) const
{
    return _impl == other._impl;
}

bool XmlAttribute::operator!=(const XmlAttribute& other) const
{
    return !(*this == other);
}

XmlAttribute::operator bool() const
{
    return IsValid();
}

XmlElement::XmlElement(tinyxml2::XMLElement* element) : _impl(element)
{
}

const char* XmlElement::Name() const
{
    return _impl ? _impl->Name() : nullptr;
}

void XmlElement::SetName(const char* name) const
{
    if (_impl)
    {
        _impl->SetName(name);
    }
}

const char* XmlElement::Attribute(const char* name, const char* defaultValue) const
{
    return _impl ? _impl->Attribute(name) : defaultValue;
}

bool XmlElement::QueryAttribute(const char* name, const char** value) const
{
    if (_impl)
    {
        return _impl->QueryAttribute(name, value) == tinyxml2::XML_SUCCESS;
    }
    return false;
}

void XmlElement::SetAttribute(const char* name, const char* value)
{
    if (_impl)
    {
        _impl->SetAttribute(name, value);
    }
}

int XmlElement::IntAttribute(const char* name, int defaultValue) const
{
    return _impl ? _impl->IntAttribute(name, defaultValue) : defaultValue;
}

bool XmlElement::QueryIntAttribute(const char* name, int* value) const
{
    if (_impl)
    {
        return _impl->QueryIntAttribute(name, value) == tinyxml2::XML_SUCCESS;
    }
    return false;
}

void XmlElement::SetAttribute(const char* name, int value)
{
    if (_impl)
    {
        _impl->SetAttribute(name, value);
    }
}

float XmlElement::FloatAttribute(const char* name, float defaultValue) const
{
    return _impl ? _impl->FloatAttribute(name, defaultValue) : defaultValue;
}

bool XmlElement::QueryFloatAttribute(const char* name, float* value) const
{
    if (_impl)
    {
        return _impl->QueryFloatAttribute(name, value) == tinyxml2::XML_SUCCESS;
    }
    return false;
}

void XmlElement::SetAttribute(const char* name, float value)
{
    if (_impl)
    {
        _impl->SetAttribute(name, value);
    }
}

bool XmlElement::BoolAttribute(const char* name, bool defaultValue) const
{
    return _impl ? _impl->BoolAttribute(name, defaultValue) : defaultValue;
}

bool XmlElement::QueryBoolAttribute(const char* name, bool* value) const
{
    if (_impl)
    {
        return _impl->QueryBoolAttribute(name, value) == tinyxml2::XML_SUCCESS;
    }
    return false;
}

void XmlElement::SetAttribute(const char* name, bool value)
{
    if (_impl)
    {
        _impl->SetAttribute(name, value);
    }
}

XmlElement XmlElement::FirstChild(const char* name) const
{
    if (_impl)
    {
        tinyxml2::XMLElement* child = _impl->FirstChildElement(name);
        return XmlElement(child);
    }
    return XmlElement(nullptr);
}

XmlElement XmlElement::LastChild(const char* name) const
{
    return _impl ? XmlElement(_impl->LastChildElement(name)) : XmlElement(nullptr);
}

XmlElement XmlElement::NextSibling(const char* name) const
{
    return _impl ? XmlElement(_impl->NextSiblingElement(name)) : XmlElement(nullptr);
}

XmlElement XmlElement::PreviousSibling(const char* name) const
{
    return _impl ? XmlElement(_impl->PreviousSiblingElement(name)) : XmlElement(nullptr);
}

XmlAttribute XmlElement::FirstAttribute() const
{
    return _impl ? XmlAttribute(_impl->FirstAttribute()) : XmlAttribute(nullptr);
}

XmlElement XmlElement::Parent() const
{
    return _impl ? XmlElement(_impl->Parent()->ToElement()) : XmlElement(nullptr);
}

XmlElement XmlElement::InsertEndChild(XmlElement element) const
{
    if (_impl && element._impl)
    {
        return XmlElement(_impl->InsertEndChild(element._impl)->ToElement());
    }
    return XmlElement(nullptr);
}

XmlElement XmlElement::InsertEndChild(const char* name) const
{
    return _impl ? XmlElement(_impl->InsertNewChildElement(name)) : XmlElement(nullptr);
}

bool XmlElement::IsValid() const
{
    return _impl != nullptr;
}

bool XmlElement::operator==(const XmlElement& other) const
{
    return _impl == other._impl;
}

bool XmlElement::operator!=(const XmlElement& other) const
{
    return !(*this == other);
}

XmlElement::operator bool() const
{
    return IsValid();
}

XmlDocument::XmlDocument(const std::string& filename)
{
    LoadFromFile(filename);
}

bool XmlDocument::LoadFromFile(const std::string& filename)
{
    return _impl.LoadFile(filename.c_str()) == tinyxml2::XML_SUCCESS;
}

bool XmlDocument::LoadFromBuffer(const char* buffer, size_t size)
{
    return _impl.Parse(buffer, size) == tinyxml2::XML_SUCCESS;
}

XmlElement XmlDocument::GetRoot()
{
    return XmlElement(_impl.RootElement());
}

XmlElement XmlDocument::NewElement(const char* name)
{
    return XmlElement(_impl.NewElement(name));
}

} // namespace Ext

DGEX_END
