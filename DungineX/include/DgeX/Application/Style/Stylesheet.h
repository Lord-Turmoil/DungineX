#pragma once

#include "DgeX/Application/Style/Style.h"
#include "DgeX/Core/Base.h"

#include <tinyxml2.h>

#include <string>
#include <unordered_map>
#include <vector>

DGEX_BEGIN

/**
 * @brief Holds the global stylesheet for the application.
 * @paragraph
 * It contains all styles in the application lifecycle. It loads styles
 * from an XML file or <Styles> element. The XML file should contain one
 * and only one root element <Styles>. The <Styles> element is as follows.
 *
 * <Styles>
 *      <Style ...>...</Style>
 * </Styles>
 */
class Stylesheet
{
public:
    Stylesheet() = default;

    static const char* XmlTag()
    {
        return "Styles";
    }

    /**
     * @brief Load styles from an XML file.
     * @param filename Styles XML file name.
     */
    void Load(const char* filename);

    /**
     * @brief Load styles from an XML element.
     * @param element <Styles> element.
     */
    void Load(const tinyxml2::XMLElement& element);

public:
    /**
     * @brief Add a style to the stylesheet.
     * @param style The style to add.
     */
    void AddStyle(const Ref<Style>& style);

    /**
     * @brief Match the element with all styles in the stylesheet.
     * @param element DOM element to match.
     * @return All styles that matches the element.
     */
    std::vector<Ref<Style>> Match(const DomElement& element) const;

private:
    std::vector<Ref<Style>> _styles;
};

DGEX_END
