#include "DgeX/Application/Style/Stylesheet.h"

#include "DgeX/Application/Widget/DomElement.h"
#include "DgeX/Core/Assert.h"
#include "DgeX/Core/Logger.h"
#include "DgeX/Utils/String.h"
#include "DgeX/Utils/Xml.h"

DGEX_BEGIN

void Stylesheet::Load(const char* filename)
{
    tinyxml2::XMLDocument doc;
    if (!Utils::Xml::OpenDocument(filename, doc))
    {
        DGEX_LOG_ERROR("Failed to load stylesheet from '{0}'", filename);
        return;
    }

    auto styles = doc.FirstChildElement("Styles");
    if (!styles)
    {
        DGEX_LOG_WARN("Missing <Styles> element in stylesheet");
        return;
    }

    Load(*styles);
}

void Stylesheet::Load(const tinyxml2::XMLElement& element)
{
    DGEX_ASSERT(Utils::String::Equals(element.Name(), XmlTag()));

    auto include = element.FirstChildElement("Include");
    while (include)
    {
        if (auto src = include->Attribute("src"))
        {
            Load(src);
        }
    }

    auto style = element.FirstChildElement(Style::XmlTag());
    while (style)
    {
        AddStyle(CreateRef<Style>(*style));
        style = style->NextSiblingElement(Style::XmlTag());
    }
}

void Stylesheet::AddStyle(const Ref<Style>& style)
{
    _styles.push_back(style);
}

std::vector<Ref<Style>> Stylesheet::Match(const DomElement& element) const
{
    std::vector<Ref<Style>> matches;

    for (const auto& style : _styles)
    {
        if (style->Match(element))
        {
            matches.push_back(style);
        }
    }

    return matches;
}

DGEX_END
