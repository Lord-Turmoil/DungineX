/******************************************************************************
 ***                   N E W  D E S I R E  S T U D I O S                    ***
 ******************************************************************************
 *                   Project Name : DungineX                                  *
 *                                                                            *
 *                      File Name : XmlDocument.h                             *
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

#pragma once

#include "DgeX/Defines.h"

#include <tinyxml2.h>

#include <string>

DGEX_BEGIN

namespace Ext
{

class XmlAttribute
{
public:
    explicit XmlAttribute(const tinyxml2::XMLAttribute* attribute);

    /**
     * @brief Get the name of the attribute.
     *
     * @return The name of the attribute.
     */
    DGEX_API const char* Name() const;

    /**
     * @brief Get the value of the attribute.
     *
     * @return The value of the attribute.
     */
    DGEX_API const char* Value(const char* defaultValue = nullptr) const;

    DGEX_API int IntValue(int defaultValue = 0) const;
    DGEX_API float FloatValue(float defaultValue = 0.0f) const;
    DGEX_API bool BoolValue(bool defaultValue = false) const;

    template <typename T>
    DGEX_API T ValueAs(const T& defaultValue = T()) const;

    /**
     * @brief Get the next attribute.
     *
     * @return The next attribute. Return invalid attribute if is the end.
     */
    DGEX_API XmlAttribute Next() const;

    /**
     * @brief Check if the attribute is valid.
     *
     * @return Whether the attribute points to a valid tinyxml2::XMLAttribute.
     */
    DGEX_API bool IsValid() const;

    bool operator==(const XmlAttribute& other) const;
    bool operator!=(const XmlAttribute& other) const;
    operator bool() const;

private:
    const tinyxml2::XMLAttribute* _impl;
};

/**
 * @brief Wrapper class for tinyxml2::XMLElement.
 */
class XmlElement
{
public:
    explicit XmlElement(tinyxml2::XMLElement* element);

    DGEX_API const char* Name() const;
    DGEX_API void SetName(const char* name) const;

    /**
     * @brief Get attribute with the specified name.
     *
     * @param name Name of the attribute.
     * @param defaultValue Default value if the attribute is not found.
     * @return The attribute.
     */
    DGEX_API const char* Attribute(const char* name, const char* defaultValue = nullptr) const;

    /**
     * @brief Query attribute with the specified name.
     *
     * @param name Name of the attribute.
     * @param value Output attribute value.
     * @return true if attribute exists, false if not found.
     */
    DGEX_API bool QueryAttribute(const char* name, const char** value) const;

    /**
     * @brief Set attribute value.
     *
     * @param name Name of the attribute.
     * @param value The attribute value to set.
     */
    DGEX_API void SetAttribute(const char* name, const char* value);

    DGEX_API int IntAttribute(const char* name, int defaultValue = 0) const;
    DGEX_API bool QueryIntAttribute(const char* name, int* value) const;
    DGEX_API void SetAttribute(const char* name, int value);

    DGEX_API float FloatAttribute(const char* name, float defaultValue = 0.0f) const;
    DGEX_API bool QueryFloatAttribute(const char* name, float* value) const;
    DGEX_API void SetAttribute(const char* name, float value);

    DGEX_API bool BoolAttribute(const char* name, bool defaultValue = false) const;
    DGEX_API bool QueryBoolAttribute(const char* name, bool* value) const;
    DGEX_API void SetAttribute(const char* name, bool value);

    /**
     * @brief Get attribute and convert to custom type.
     *
     * It requires the custom type T to have a constructor that accepts a const char*.
     *
     * @tparam T Custom attribute type.
     * @param name Name of the attribute.
     * @param defaultValue Default value if attribute not found.
     * @return The value in custom type.
     */
    template <typename T>
    DGEX_API T AttributeAs(const char* name, const T& defaultValue = T()) const;

    /**
     * @brief
     * @tparam T Custom attribute type.
     * @param name Name of the attribute.
     * @param value
     * @return
     */
    template <typename T>
    DGEX_API bool QueryAttributeAs(const char* name, T* value) const;
    template <typename T>
    DGEX_API void SetAttributeAs(const char* name, const T& value) const;

    /**
     * @brief Get the first attribute of the element.
     *
     * An invalid attribute indicates the end.
     *
     * @return The first attribute.
     */
    DGEX_API XmlAttribute FirstAttribute() const;

    /**
     * @brief Get the parent element.

     * @return The parent XML element, or an invalid XmlElement if there is no parent.
     */
    DGEX_API XmlElement Parent() const;

    /**
     * @brief Get the first child element.
     *
     * @param name If provided, will return the first child with the given name.
     * @return The first child element.
     */
    DGEX_API XmlElement FirstChild(const char* name = nullptr) const;

    /**
     * @brief Get the last child element.
     *
     * @param name If provided, will return the last child with the given name.
     * @return The last child element.
     */
    DGEX_API XmlElement LastChild(const char* name = nullptr) const;

    /**
     * @brief Get the next sibling element.
     *
     * @param name If provided, will return the next sibling with the given name.
     * @return The next sibling element.
     */
    DGEX_API XmlElement NextSibling(const char* name = nullptr) const;

    /**
     * @brief Get the previous sibling element.
     *
     * @param name If provided, will return the previous sibling with the given name.
     * @return The previous sibling element.
     */
    DGEX_API XmlElement PreviousSibling(const char* name = nullptr) const;

    /**
     * @brief Insert a new child element at the end.
     *
     * @param element The XmlElement to insert.
     * @return The inserted XmlElement.
     */
    DGEX_API XmlElement InsertEndChild(XmlElement element) const;

    /**
     * @brief Insert a new child element with the specified name at the end.
     *
     * @param name The name of the new element to insert.
     * @return The inserted XmlElement.
     */
    DGEX_API XmlElement InsertEndChild(const char* name) const;

    /**
     * @brief Check if the XmlElement is valid.
     *
     * @return Whether the XmlElement points to a valid tinyxml2::XMLElement.
     */
    DGEX_API bool IsValid() const;

public:
    DGEX_API bool operator==(const XmlElement& other) const;
    DGEX_API bool operator!=(const XmlElement& other) const;

    DGEX_API operator bool() const;

private:
    tinyxml2::XMLElement* _impl;
};

template <typename T>
T XmlAttribute::ValueAs(const T& defaultValue) const
{
    if (_impl)
    {
        return T(_impl->Value());
    }
    return defaultValue;
}

template <typename T>
T XmlElement::AttributeAs(const char* name, const T& defaultValue) const
{
    if (const char* attribute = Attribute(name))
    {
        return T(attribute);
    }
    return defaultValue;
}

template <typename T>
bool XmlElement::QueryAttributeAs(const char* name, T* value) const
{
    if (const char* attribute = Attribute(name))
    {
        *value = T(attribute);
        return true;
    }
    return false;
}

template <typename T>
void XmlElement::SetAttributeAs(const char* name, const T& value) const
{
    SetAttribute(name, value.ToString().c_str());
}

/**
 * @brief Wrapper class for tinyxml2::XMLDocument.
 */
class XmlDocument
{
public:
    DGEX_API XmlDocument() = default;

    /**
     * @brief Create an XML document from a file.
     *
     * If opening the file fails, the document will be empty, i.e. no root element.
     *
     * @param filename Filename of the XML file.
     */
    DGEX_API XmlDocument(const std::string& filename);

    DGEX_API XmlDocument(const XmlDocument&) = delete;
    DGEX_API XmlDocument(XmlDocument&&) = delete;
    DGEX_API XmlDocument& operator=(const XmlDocument&) = delete;
    DGEX_API XmlDocument& operator=(XmlDocument&&) = delete;

    DGEX_API bool LoadFromFile(const std::string& filename);
    DGEX_API bool LoadFromBuffer(const char* buffer, size_t size);

    DGEX_API XmlElement GetRoot();
    DGEX_API XmlElement NewElement(const char* name);

private:
    tinyxml2::XMLDocument _impl;
};

} // namespace Ext

DGEX_END
