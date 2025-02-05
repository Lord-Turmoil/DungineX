#include "DgeX/Application/Widget/DomContext.h"

#include "DgeX/Application/Style/Stylesheet.h"
#include "DgeX/Application/Widget/DomElement.h"
#include "DgeX/Core/Log.h"
#include "DgeX/Core/UUID.h"
#include "DgeX/Utils/String.h"
#include "DgeX/Utils/Xml.h"

DGEX_BEGIN

DomContext::DomContext() : _stylesheet(CreateRef<Stylesheet>())
{
    _context = this;
}

Ptr<DomContext> DomContext::AsContext()
{
    return this;
}

void DomContext::OnUpdate(DeltaTime delta)
{
    if (_root)
    {
        _root->OnUpdate(delta);
    }
}

void DomContext::OnEvent(const Ref<Event>& event)
{
    if (_root)
    {
        _root->OnEvent(event);
    }
}

void DomContext::OnRender()
{
    if (_root)
    {
        _root->OnRender();
    }
}

void DomContext::ComputeProperties()
{
    if (_root)
    {
        _root->ComputeProperties();
    }
}

void DomContext::ComputeLayout()
{
    if (_root)
    {
        _root->ComputeLayout();
    }
}

bool DomContext::Load(const char* filename)
{
    tinyxml2::XMLDocument doc;

    if (!Utils::Xml::OpenDocument(filename, doc))
    {
        DGEX_LOG_ERROR("Failed to load XML page from '{0}'", filename);
        return false;
    }

    Load(*doc.RootElement());

    return true;
}

void DomContext::Load(const tinyxml2::XMLElement& element)
{
    _UnLoad();

    _root = _LoadElement(element);

    ComputeProperties();
    ComputeLayout();
}

Ref<DomElement> DomContext::CreateElement(const std::string& tagName, const std::string& id)
{
    return CreateRef<DomElement>(this, tagName, id.empty() ? UUID().ToString() : id);
}

Ref<DomElement> DomContext::GetRootElement() const
{
    return _root;
}

Ref<Stylesheet> DomContext::GetStylesheet() const
{
    return _stylesheet;
}

void DomContext::_UnLoad()
{
    _root.reset();
    _stylesheet = CreateRef<Stylesheet>();
}

Ref<DomElement> DomContext::_LoadElement(const tinyxml2::XMLElement& element)
{
    auto adapter = Utils::Xml::XmlElementAdapter(&element);
    Ref<DomElement> domElement = CreateElement(element.Name(), adapter.Attribute("id", UUID().ToString()));
    domElement->Load(element);

    auto child = element.FirstChildElement();
    while (child)
    {
        if (Utils::String::Equals(child->Name(), Stylesheet::XmlTag()))
        {
            _stylesheet->Load(*child);
        }
        else
        {
            domElement->InsertEndChild(_LoadElement(*child));
        }
        child = child->NextSiblingElement();
    }

    return domElement;
}

DGEX_END
