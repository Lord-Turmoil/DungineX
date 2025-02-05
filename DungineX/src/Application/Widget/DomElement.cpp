#include "DgeX/Application/Widget/DomElement.h"

#include "DgeX/Application/Event/MouseEvent.h"
#include "DgeX/Application/Style/Property/Property.h"
#include "DgeX/Application/Style/Property/PropertyGroup.h"
#include "DgeX/Application/Style/Stylesheet.h"
#include "DgeX/Application/Widget/DomContext.h"
#include "DgeX/Core/Assert.h"
#include "DgeX/Utils/Xml.h"

DGEX_BEGIN

DomElementState DomElementStateFromString(const std::string& state)
{
    if (state == "hover")
    {
        return DomElementState::Hover;
    }
    if (state == "active")
    {
        return DomElementState::Active;
    }
    if (state == "focus")
    {
        return DomElementState::Focus;
    }
    if (state == "disabled")
    {
        return DomElementState::Disabled;
    }
    return DomElementState::Normal;
}

std::string DomElementStateToString(DomElementState state)
{
    switch (state)
    {
    case DomElementState::Normal:
        return "normal";
    case DomElementState::Hover:
        return "hover";
    case DomElementState::Active:
        return "active";
    case DomElementState::Focus:
        return "focus";
    case DomElementState::Disabled:
        return "disabled";
    }
    return "normal";
}

DomElement::DomElement(Ptr<DomContext> context, std::string tagName, std::string id)
    : _tagName(std::move(tagName)), _id(std::move(id)), _state(DomElementState::Normal)
{
    DGEX_ASSERT(context, "DOM Element must have valid context");

    _context = context;

    _properties->AddProperty<SizeProperty>();
    _properties->AddProperty<PositionProperty>();
    _properties->AddProperty<TranslateProperty>();
    _properties->AddProperty<ColorProperty>();
    _properties->AddProperty<OpacityProperty>();

    AddEventListener<MouseMovedEvent>("mousemove", DGEX_BIND_EVENT_FN(_OnMouseMoved));
    AddEventListener<MouseButtonPressedEvent>("mousedown", DGEX_BIND_EVENT_FN(_OnMousePressed));
    AddEventListener<MouseButtonReleasedEvent>("mouseup", DGEX_BIND_EVENT_FN(_OnMouseReleased));
}

Ptr<DomElement> DomElement::AsElement()
{
    return this;
}

void DomElement::Load(const tinyxml2::XMLElement& element)
{
    _classList = Utils::Xml::ParseClassList(element.Attribute("class"));
    _properties->Load(element);
}

void DomElement::OnUpdate(DeltaTime delta)
{
    for (auto& child : _children)
    {
        child->OnUpdate(delta);
    }
}

void DomElement::OnEvent(const Ref<Event>& event)
{
    if (event->Handled)
    {
        return;
    }

    for (const auto& [_, listener] : _eventListeners)
    {
        if (!event->Handled)
        {
            listener->OnEvent(event);
        }
    }

    for (auto& child : _children)
    {
        child->OnEvent(event);
    }
}

void DomElement::OnRender()
{
    for (auto& child : _children)
    {
        child->OnRender();
    }
}

const std::string& DomElement::TagName() const
{
    return _tagName;
}

const std::string& DomElement::Id() const
{
    return _id;
}

Ref<DomElement> DomElement::CreateElement(const std::string& name, const std::string& id) const
{
    return GetContext()->CreateElement(name, id);
}

void DomElement::RemoveChild(const Ref<DomElement>& element)
{
    for (auto it = _children.begin(); it != _children.end(); ++it)
    {
        if (*it == element)
        {
            _children.erase(it);
            element->_parent = nullptr;
            return;
        }
    }
}

void DomElement::InsertEndChild(const Ref<DomElement>& element)
{
    DGEX_ASSERT(_context == element->_context);

    if (element->GetParentElement())
    {
        element->GetParentElement()->RemoveChild(element);
    }

    element->_parent = this;
    _children.push_back(element);

    ComputeLayout();
}

const std::vector<Ref<DomElement>>& DomElement::Children() const
{
    return _children;
}

size_t DomElement::Size() const
{
    return _children.size();
}

Ref<DomElement> DomElement::FirstChild() const
{
    return _children.empty() ? nullptr : _children.front();
}

Ref<DomElement> DomElement::LastChild() const
{
    return _children.empty() ? nullptr : _children.back();
}

Ref<DomElement> DomElement::PrevSibling() const
{
    if (!_parent)
    {
        return nullptr;
    }

    const auto& children = GetParentElement()->Children();
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (it->get() == this)
        {
            return (it == children.begin()) ? nullptr : *(it - 1);
        }
    }

    DGEX_LOG_ERROR("Element not in parent's children");

    return nullptr;
}

Ref<DomElement> DomElement::NextSibling() const
{
    if (!GetParentElement())
    {
        return nullptr;
    }

    const auto& children = GetParentElement()->Children();
    for (auto it = children.begin(); it != children.end(); ++it)
    {
        if (it->get() == this)
        {
            return (it + 1 == children.end()) ? nullptr : *(it + 1);
        }
    }

    DGEX_LOG_ERROR("Element not in parent's children");

    return nullptr;
}

Ref<DomElement> DomElement::GetElementById(const std::string& id)
{
    for (const auto& child : _children)
    {
        if (child->Id() == id)
        {
            return child;
        }
    }

    for (const auto& child : _children)
    {
        if (auto found = child->GetElementById(id))
        {
            return found;
        }
    }

    return nullptr;
}

static void GetElementsByTagNameRecursive(const Ref<DomElement>& element, const std::string& tagName,
                                          std::vector<Ref<DomElement>>& result)
{
    if (element->TagName() == tagName)
    {
        result.push_back(element);
    }

    for (auto& child : *element)
    {
        GetElementsByTagNameRecursive(child, tagName, result);
    }
}

std::vector<Ref<DomElement>> DomElement::GetElementsByTagName(const std::string& tagName)
{
    std::vector<Ref<DomElement>> result;
    GetElementsByTagNameRecursive(shared_from_this(), tagName, result);
    return result;
}

static void GetElementsByClassNameRecursive(const Ref<DomElement>& element, const std::string& className,
                                            std::vector<Ref<DomElement>>& result)
{
    if (element->HasClass(className))
    {
        result.push_back(element);
    }

    for (auto& child : *element)
    {
        GetElementsByClassNameRecursive(child, className, result);
    }
}

std::vector<Ref<DomElement>> DomElement::GetElementsByClassName(const std::string& className)
{
    std::vector<Ref<DomElement>> result;
    GetElementsByClassNameRecursive(shared_from_this(), className, result);
    return result;
}

void DomElement::SetState(DomElementState state)
{
    _OnStateChange(state);
}

DomElementState DomElement::GetState() const
{
    return _state;
}

void DomElement::AddClass(const std::string& className)
{
    if (!HasClass(className))
    {
        _classList.push_back(className);
        ComputeProperties();
    }
}

bool DomElement::HasClass(const std::string& className) const
{
    return std::find(_classList.begin(), _classList.end(), className) != _classList.end();
}

void DomElement::RemoveClass(const std::string& className)
{
    if (HasClass(className))
    {
        _classList.erase(std::remove(_classList.begin(), _classList.end(), className), _classList.end());
        ComputeProperties();
    }
}

void DomElement::RemoveEventListener(const std::string& name)
{
    _eventListeners.erase(name);
}

bool DomElement::_InRange(const MousePosition& position)
{
    return (_x <= position.X) && (position.X <= _x + _width) && (_y <= position.Y) && (position.Y <= _y + _height);
}

bool DomElement::_OnMouseMoved(Ptr<DomElement> element, const MouseMovedEvent& event)
{
    if (element != this)
    {
        return false;
    }

    _lastMousePosition = event.GetMousePosition();
    bool inRange = _InRange(_lastMousePosition);

    switch (GetState())
    {
    case DomElementState::Normal:
        if (inRange)
        {
            _OnStateChange(DomElementState::Hover);
        }
        break;
    case DomElementState::Hover:
        if (!inRange)
        {
            _OnStateChange(DomElementState::Normal);
        }
        break;
    case DomElementState::Active:
    case DomElementState::Focus:
    case DomElementState::Disabled:
        break;
    }

    return false;
}

bool DomElement::_OnMousePressed(Ptr<DomElement> element, const MouseButtonPressedEvent& event)
{
    if (element != this)
    {
        return false;
    }

    switch (GetState())
    {
    case DomElementState::Hover:
        _lastMouseCode = event.GetMouseCode();
        SetState(DomElementState::Active);
        return true;
    case DomElementState::Normal:
    case DomElementState::Active:
    case DomElementState::Focus:
    case DomElementState::Disabled:
        break;
    }

    return false;
}

bool DomElement::_OnMouseReleased(Ptr<DomElement> element, const MouseButtonReleasedEvent& event)
{
    if (element != this)
    {
        return false;
    }

    bool inRange = _InRange(_lastMousePosition);

    switch (GetState())
    {
    case DomElementState::Active:
        if (event.GetMouseCode() == _lastMouseCode)
        {
            SetState(inRange ? DomElementState::Hover : DomElementState::Normal);
            return true;
        }
        break;
    case DomElementState::Normal:
    case DomElementState::Hover:
    case DomElementState::Focus:
    case DomElementState::Disabled:
        break;
    }

    return false;
}

void DomElement::_OnStateChange(DomElementState newState)
{
    if (_state == DomElementState::Active && newState == DomElementState::Hover)
    {
        OnEvent(MouseButtonClickedEvent::Create(_lastMouseCode));
    }
    _state = newState;
    ComputeProperties();
}

void DomElement::ComputeProperties()
{
    Ref<Stylesheet> stylesheet = _context->GetStylesheet();

    // Load properties from the stylesheet.
    std::vector<Ref<Style>> styles = stylesheet->Match(*this);
    for (const auto& style : styles)
    {
        _properties->Load(style);
    }

    // Update properties.
    auto sizeProp = _properties->GetProperty<SizeProperty>();
    _width = sizeProp->GetWidth().ToPixel(GetParentWidth());
    _height = sizeProp->GetHeight().ToPixel(GetParentHeight());

    // Update child properties recursively.
    for (auto& child : _children)
    {
        child->ComputeProperties();
    }
}

void DomElement::ComputeLayout()
{
}

DGEX_END
