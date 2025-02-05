#pragma once

#include "DgeX/Application/Widget/DomNode.h"

#include <string>
#include <tinyxml2.h>

DGEX_BEGIN

class Stylesheet;
class DomElement;

/**
 * @brief
 * A DOM context takes hold of one XML page. It contains one DOM tree, and
 * may include multiple XML stylesheets.
 */
class DomContext : public DomNode
{
public:
    DomContext();

    DomContext(const DomContext&) = delete;
    DomContext(DomContext&& other) noexcept = default;
    DomContext& operator=(const DomContext&) = delete;
    DomContext& operator=(DomContext&& other) noexcept = default;

    Ptr<DomContext> AsContext() override;

public:
    void OnUpdate(DeltaTime delta) override;
    void OnEvent(const Ref<Event>& event) override;
    void OnRender() override;

    void ComputeProperties() override;
    void ComputeLayout() override;

public:
    /**
     * @brief Load DOM from an XML file.
     * @param filename XML page file path.
     * @note This will clear the current DOM tree.
     */
    bool Load(const char* filename);

    /**
     * @brief Load DOM from and XML element.
     * @param element XML element.
     * @note This will clear the current DOM tree.
     */
    void Load(const tinyxml2::XMLElement& element);

public:
    Ref<DomElement> CreateElement(const std::string& tagName, const std::string& id = "");

public:
    Ref<DomElement> GetRootElement() const;
    Ref<Stylesheet> GetStylesheet() const;

private:
    void _UnLoad();
    Ref<DomElement> _LoadElement(const tinyxml2::XMLElement& element);

private:
    Ref<DomElement> _root;
    Ref<Stylesheet> _stylesheet;
};

DGEX_END
