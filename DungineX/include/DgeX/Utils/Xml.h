#pragma once

#include "DgeX/Core/Macros.h"

#include <string>
#include <tinyxml2.h>
#include <vector>

DGEX_BEGIN

namespace Utils::Xml
{

class XmlElementAdapter
{
public:
    XmlElementAdapter(const tinyxml2::XMLElement* element);

    const tinyxml2::XMLElement* Native();

    /**
     * @brief Wrap around a given XML element.
     * @param element XML element to wrap.
     */
    void Attach(const tinyxml2::XMLElement* element);

    /**
     * @brief Go to the first child or next sibling XML element.
     * @return Whether it still points to valid XML element.
     */
    bool GoToFirstChild();
    bool GoToNextSibling();

    /**
     * @brief Get XML element attribute.
     * @param name Attribute name.
     * @param defaultValue Default value.
     * @return If found, return the attribute. Otherwise,default value is returned.
     */
    bool Attribute(const char* name, bool defaultValue);
    int Attribute(const char* name, int defaultValue);
    float Attribute(const char* name, float defaultValue);
    const char* Attribute(const char* name, const char* defaultValue);
    std::string Attribute(const char* name, const std::string& defaultValue);

private:
    const tinyxml2::XMLElement* _element;
};

/**
 * @brief Parse XML element class list.
 * @return Class list.
 */
std::vector<std::string> ParseClassList(const char* value);

bool OpenDocument(const char* filename, tinyxml2::XMLDocument& document);

} // namespace Utils::Xml

DGEX_END
