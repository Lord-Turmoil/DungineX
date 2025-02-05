#include "DgeX/Utils/Xml.h"

#include "DgeX/Core/Assert.h"
#include "DgeX/Core/Logger.h"

DGEX_BEGIN

namespace Utils::Xml
{

XmlElementAdapter::XmlElementAdapter(const tinyxml2::XMLElement* element) : _element(element)
{
    DGEX_ASSERT(_element);
}

const tinyxml2::XMLElement* XmlElementAdapter::Native()
{
    return _element;
}

void XmlElementAdapter::Attach(const tinyxml2::XMLElement* element)
{
    DGEX_ASSERT(element);
    _element = element;
}

bool XmlElementAdapter::GoToFirstChild()
{
    _element = _element->FirstChildElement();
    return _element;
}

bool XmlElementAdapter::GoToNextSibling()
{
    _element = _element->NextSiblingElement();
    return _element;
}

bool XmlElementAdapter::Attribute(const char* name, bool defaultValue)
{
    bool value;
    if (_element->QueryBoolAttribute(name, &value) == tinyxml2::XML_SUCCESS)
    {
        return value;
    }
    return defaultValue;
}

int XmlElementAdapter::Attribute(const char* name, int defaultValue)
{
    int value;
    if (_element->QueryIntAttribute(name, &value) == tinyxml2::XML_SUCCESS)
    {
        return value;
    }
    return defaultValue;
}

float XmlElementAdapter::Attribute(const char* name, float defaultValue)
{
    float value;
    if (_element->QueryFloatAttribute(name, &value) == tinyxml2::XML_SUCCESS)
    {
        return value;
    }
    return defaultValue;
}

const char* XmlElementAdapter::Attribute(const char* name, const char* defaultValue)
{
    const char* value;
    if (_element->QueryStringAttribute(name, &value) == tinyxml2::XML_SUCCESS)
    {
        return value;
    }
    return defaultValue;
}

std::string XmlElementAdapter::Attribute(const char* name, const std::string& defaultValue)
{
    const char* value;
    if (_element->QueryStringAttribute(name, &value) == tinyxml2::XML_SUCCESS)
    {
        return value;
    }
    return defaultValue;
}

std::vector<std::string> ParseClassList(const char* value)
{
    std::vector<std::string> classList;

    if (!value)
    {
        return classList;
    }

    const char* left = value;
    const char* right = value;

    bool found = false;
    while (*left)
    {
        if (found)
        {
            if ((*right == ' ') || (*right == '\0'))
            {
                classList.emplace_back(left, right);
                left = right;
                found = false;
            }
            else
            {
                right++;
            }
        }
        else
        {
            if (*left != ' ')
            {
                right = left + 1;
                found = true;
            }
            else
            {
                left++;
            }
        }
    }

    return classList;
}

bool OpenDocument(const char* filename, tinyxml2::XMLDocument& document)
{
    tinyxml2::XMLError result = document.LoadFile(filename);
    if (result == tinyxml2::XML_SUCCESS)
    {
        return true;
    }

    DGEX_LOG_ERROR("Failed to open XML file '{0}': {1}", filename, document.ErrorStr());

    return false;
}

} // namespace Utils::Xml

DGEX_END
